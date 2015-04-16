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
  float R;
  float C;
  float T;
  float K;
  float R2;
  int width, height;
  int NumSamples;
  bool OnOff;
  OpenGLMesh m_quadGL;
  OpenGLTexture m_texture;
  OpenGLTexture m_working;
  OpenGLFramebufferObject m_fbo;
  OpenGLShaderProgram *m_ssaoPass;
  OpenGLShaderProgram *m_blurProgram;
  OpenGLUniformBufferObject m_blurData;
  void constructTexture(OpenGLTexture &t, OpenGLInternalFormat f, int width, int height);
};

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

  p.C = 1.3;
  p.R = 1.0;
  p.T = 0.0025;
  p.K = 0.5;
  p.NumSamples = 20;
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
}

void ScreenSpaceAmbientOcclusion::commit(OpenGLViewport &view)
{
  P(ScreenSpaceAmbientOcclusionPrivate);
  GL::glActiveTexture(OpenGLTexture::beginTextureUnits() + K_AMBIENT_OCCLUSION_BINDING);
  p.m_texture.bind();
  GL::glActiveTexture(OpenGLTexture::beginTextureUnits());
}

void ScreenSpaceAmbientOcclusion::render(OpenGLScene &scene)
{
  P(ScreenSpaceAmbientOcclusionPrivate);
  OpenGLMarkerScoped _("Screen Space Ambient Occlusion");

  float dir = 1.0;
  if (KInputManager::keyPressed(Qt::Key_Shift))
  {
    dir = -1.0;
  }

  if (KInputManager::keyPressed(Qt::Key_R))
  {
    p.R += 0.016 * dir;
  }
  if (KInputManager::keyPressed(Qt::Key_Y))
  {
    p.R2 += 0.016 * dir;
  }
  if (KInputManager::keyPressed(Qt::Key_T))
  {
    p.T += 0.00016 * dir;
  }
  if (KInputManager::keyPressed(Qt::Key_C))
  {
    p.C += 0.016 * dir;
  }
  if (KInputManager::keyPressed(Qt::Key_N))
  {
    p.NumSamples += int(1.0 * dir);
  }
  if (KInputManager::keyPressed(Qt::Key_K))
  {
    p.K += 0.016 * dir;
  }

  if (KInputManager::keyTriggered(Qt::Key_X))
  {
    p.OnOff = !p.OnOff;
  }

  OpenGLFramebufferObject::push();
  p.m_fbo.bind();
  if (p.OnOff)
  {
    p.m_ssaoPass->bind();
    p.m_ssaoPass->setUniformValue("SampleRadius", p.R);
    p.m_ssaoPass->setUniformValue("ShadowScalar", p.C * p.R);
    p.m_ssaoPass->setUniformValue("DepthThreshold", p.T);
    p.m_ssaoPass->setUniformValue("ShadowContrast", p.K);
    p.m_ssaoPass->setUniformValue("NumSamples", p.NumSamples);
    p.m_quadGL.draw();
    p.m_ssaoPass->release();

    int W = p.width;
    int H = p.height;

    // Next: Blur the SSAO
    if (!KInputManager::keyPressed(Qt::Key_B))
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
      GL::glDispatchCompute(std::ceil(float(W) / 128), H, 1);
      GL::glBindImageTexture(0, p.m_working.textureId(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32F);
      GL::glBindImageTexture(1, p.m_texture.textureId(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);
      GL::glUniform2i(loc, 0, 1);
      GL::glDispatchCompute(std::ceil(float(H) / 128), W, 1);
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

  if (KInputManager::keyPressed(Qt::Key_L))
  {
    OpenGLDebugDraw::Screen::drawTexture(KRectF(0.0f, 0.0f, 1.0f, 1.0f), p.m_texture);
  }
}

void ScreenSpaceAmbientOcclusion::teardown()
{
  delete m_private->m_ssaoPass;
  delete m_private;
}
