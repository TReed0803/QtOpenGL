#include "screenspaceambientocclusion.h"

#include <KMacros>
#include <KSize>
#include <OpenGLMesh>
#include <OpenGLShaderProgram>
#include <OpenGLViewport>
#include <OpenGLScene>
#include <OpenGLEnvironment>
#include <OpenGLTexture>
#include <OpenGLBindings>
#include <OpenGLUniformBufferObject>
#include <OpenGLHammersleyData>
#include <OpenGLAbstractLightGroup>
#include <OpenGLDebugDraw>
#include <KRectF>
#include <KInputManager>
#include <OpenGLBlurData>

class ScreenSpaceAmbientOcclusionPrivate
{
public:

  // Settings
  bool m_dirty;
  float m_radius;
  float m_power;
  float m_threshold;
  float m_contrast;
  int m_samples;
  bool m_blur;

  // Properties
  int width, height;
  bool m_lastActive;

  // Working
  OpenGLMesh m_quadGL;
  OpenGLTexture m_texture;
  OpenGLTexture m_working;
  OpenGLFramebufferObject m_fbo;
  OpenGLShaderProgram *m_ssaoPass;
  OpenGLShaderProgram *m_blurProgram;
  OpenGLUniformBufferObject m_blurData;

  // Helper functions
  ScreenSpaceAmbientOcclusionPrivate();
  void constructTexture(OpenGLTexture &t, OpenGLInternalFormat f, int width, int height);
};

ScreenSpaceAmbientOcclusionPrivate::ScreenSpaceAmbientOcclusionPrivate() :
  m_lastActive(false), m_dirty(true), m_blur(true)
{
  // Intentionally Empty
}

void ScreenSpaceAmbientOcclusionPrivate::constructTexture(OpenGLTexture &t, OpenGLInternalFormat f, int width, int height)
{
  t.create(OpenGLTexture::Texture2D);
  t.bind();
  t.setInternalFormat(f);
  t.setWrapMode(OpenGLTexture::DirectionS, OpenGLTexture::ClampToEdge);
  t.setWrapMode(OpenGLTexture::DirectionT, OpenGLTexture::ClampToEdge);
  t.setFilter(OpenGLTexture::Magnification, OpenGLTexture::Nearest);
  t.setFilter(OpenGLTexture::Minification, OpenGLTexture::Nearest);
  t.setSize(width, height);
  t.allocate();
  t.release();
}

ScreenSpaceAmbientOcclusion::ScreenSpaceAmbientOcclusion() :
  m_private(0)
{
  // Intentionally Empty
}

void ScreenSpaceAmbientOcclusion::initialize()
{
  m_private = new ScreenSpaceAmbientOcclusionPrivate;
  P(ScreenSpaceAmbientOcclusionPrivate);

  p.m_ssaoPass = new OpenGLShaderProgram();
  p.m_ssaoPass->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/resources/shaders/lighting/ambientOcclusion.vert");
  p.m_ssaoPass->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/resources/shaders/lighting/ambientOcclusion.frag");
  p.m_ssaoPass->link();

  p.m_fbo.create();
  p.m_quadGL.create(":/resources/objects/quad.obj");

  // Create the Compute Blur Program
  p.m_blurProgram = new OpenGLShaderProgram;
  p.m_blurProgram->addShaderFromSourceFile(QOpenGLShader::Compute, ":/resources/shaders/compute/bilateralBlur.comp");
  p.m_blurProgram->link();
  p.m_blurProgram->bind();
  p.m_blurProgram->setUniformValue("src", 0);
  p.m_blurProgram->setUniformValue("dst", 1);
  p.m_blurProgram->release();

  // Setup blur data
  OpenGLBlurData data(8, 8.0f);
  p.m_blurData.create();
  p.m_blurData.bind();
  p.m_blurData.allocate(&data, sizeof(OpenGLBlurData));
  p.m_blurData.release();
}

void ScreenSpaceAmbientOcclusion::resize(int width, int height)
{
  P(ScreenSpaceAmbientOcclusionPrivate);
  p.width = width;
  p.height = height;

  // Other Texture Storage
  p.constructTexture(p.m_texture, OpenGLInternalFormat::R32F, width, height);
  p.constructTexture(p.m_working, OpenGLInternalFormat::R32F, width, height);
  p.m_texture.setSwizzle(OpenGLTexture::Red, OpenGLTexture::Red, OpenGLTexture::Red, OpenGLTexture::One);

  // Light Buffer
  p.m_fbo.bind();
  p.m_fbo.attachTexture2D(OpenGLFramebufferObject::TargetDraw, OpenGLFramebufferObject::ColorAttachment0, p.m_texture);
  p.m_fbo.drawBuffers(OpenGLFramebufferObject::ColorAttachment0);
  p.m_fbo.validate();
  p.m_fbo.release();

  // Force a pass to clear SSAO buffer
  p.m_lastActive = true;
}

void ScreenSpaceAmbientOcclusion::commit(OpenGLViewport &view)
{
  P(ScreenSpaceAmbientOcclusionPrivate);
  if (active() || p.m_lastActive)
  {
    GL::glActiveTexture(OpenGLTexture::beginTextureUnits() + K_AMBIENT_OCCLUSION_BINDING);
    p.m_texture.bind();
    GL::glActiveTexture(OpenGLTexture::beginTextureUnits());
  }
}

void ScreenSpaceAmbientOcclusion::render(OpenGLScene &scene)
{
  P(ScreenSpaceAmbientOcclusionPrivate);
  if (!active() && !p.m_lastActive) return;

  OpenGLMarkerScoped _("Screen Space Ambient Occlusion");
  OpenGLFramebufferObject::push();
  p.m_fbo.bind();
  if (active())
  {

    // Create the SSAO Buffer
    p.m_ssaoPass->bind();
    if (p.m_dirty)
    {
      p.m_dirty = false;
      p.m_ssaoPass->setUniformValue("SampleRadius", p.m_radius);
      p.m_ssaoPass->setUniformValue("ShadowScalar", p.m_power);
      p.m_ssaoPass->setUniformValue("DepthThreshold", p.m_threshold);
      p.m_ssaoPass->setUniformValue("ShadowContrast", p.m_contrast);
      p.m_ssaoPass->setUniformValue("NumSamples", p.m_samples);
    }
    p.m_quadGL.draw();
    p.m_ssaoPass->release();

    // Next: Blur the SSAO
    if (p.m_blur)
    {
      OpenGLBlurData data(5, 5.0f);
      p.m_blurData.bind();
      p.m_blurData.allocate(&data, sizeof(OpenGLBlurData));
      p.m_blurData.release();
      GLint loc = p.m_blurProgram->uniformLocation("Direction");
      p.m_blurProgram->bind();
      p.m_blurData.bindBase(K_BLUR_BINDING);
      GL::glBindImageTexture(0, p.m_texture.textureId(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32F);
      GL::glBindImageTexture(1, p.m_working.textureId(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);
      GL::glUniform2i(loc, 1, 0);
      GL::glDispatchCompute(std::ceil(float(p.width) / 128), p.height, 1);
      GL::glBindImageTexture(0, p.m_working.textureId(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32F);
      GL::glBindImageTexture(1, p.m_texture.textureId(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);
      GL::glUniform2i(loc, 0, 1);
      GL::glDispatchCompute(std::ceil(float(p.height) / 128), p.width, 1);
      p.m_blurProgram->release();
    }
  }
  else
  {
    GL::glClearColor(1.0, 1.0, 1.0, 1.0);
    GL::glClear(GL_COLOR_BUFFER_BIT);
  }
  p.m_fbo.release();
  OpenGLFramebufferObject::pop();

  p.m_lastActive = active();
}

void ScreenSpaceAmbientOcclusion::teardown()
{
  delete m_private->m_ssaoPass;
  delete m_private;
}

void ScreenSpaceAmbientOcclusion::setRadius(float r)
{
  P(ScreenSpaceAmbientOcclusionPrivate);
  p.m_radius = r;
  p.m_dirty = true;
}

void ScreenSpaceAmbientOcclusion::setThreshold(float t)
{
  P(ScreenSpaceAmbientOcclusionPrivate);
  p.m_threshold = t;
  p.m_dirty = true;
}

void ScreenSpaceAmbientOcclusion::setSamples(int s)
{
  P(ScreenSpaceAmbientOcclusionPrivate);
  p.m_samples = s;
  p.m_dirty = true;
}

void ScreenSpaceAmbientOcclusion::setBlur(bool b)
{
  P(ScreenSpaceAmbientOcclusionPrivate);
  p.m_blur = b;
}

void ScreenSpaceAmbientOcclusion::setPower(float c)
{
  P(ScreenSpaceAmbientOcclusionPrivate);
  p.m_power = c;
  p.m_dirty = true;
}

void ScreenSpaceAmbientOcclusion::setContrast(float k)
{
  P(ScreenSpaceAmbientOcclusionPrivate);
  p.m_contrast = k;
  p.m_dirty = true;
}
