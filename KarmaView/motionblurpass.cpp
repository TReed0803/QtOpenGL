#include "motionblurpass.h"

#include <OpenGLMesh>
#include <OpenGLShaderProgram>
#include <OpenGLViewport>
#include <OpenGLScene>
#include <OpenGLTexture>
#include <OpenGLFramebufferObject>
#include <OpenGLBindings>

class MotionBlurPassPrivate
{
public:

  // Settings
  bool m_dirty;
  int m_maxSamples;
  float m_power;

  // Working
  OpenGLMesh m_quadGL;
  OpenGLShaderProgram *m_program;
  OpenGLTexture m_blurTexture;
  OpenGLFramebufferObject m_fbo;

  MotionBlurPassPrivate();
};

MotionBlurPassPrivate::MotionBlurPassPrivate()
{
  // Intentionally Empty
}

MotionBlurPass::MotionBlurPass()
{
  // Intentionally Empty
}

void MotionBlurPass::initialize()
{
  m_private = new MotionBlurPassPrivate;
  P(MotionBlurPassPrivate);

  // Motion Blur Shader
  p.m_program = new OpenGLShaderProgram();
  p.m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/resources/shaders/gbuffer/main.vert");
  p.m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/resources/shaders/gbuffer/motionBlur.frag");
  p.m_program->link();

  p.m_fbo.create();

  // Quad
  p.m_quadGL.create(":/resources/objects/quad.obj");
}

void MotionBlurPass::resize(int width, int height)
{
  P(MotionBlurPassPrivate);

  p.m_blurTexture.create(OpenGLTexture::Texture2D);
  p.m_blurTexture.bind();
  p.m_blurTexture.setInternalFormat(OpenGLInternalFormat::Rgba16F);
  p.m_blurTexture.setWrapMode(OpenGLTexture::DirectionS, OpenGLTexture::ClampToEdge);
  p.m_blurTexture.setWrapMode(OpenGLTexture::DirectionT, OpenGLTexture::ClampToEdge);
  p.m_blurTexture.setFilter(OpenGLTexture::Magnification, OpenGLTexture::Nearest);
  p.m_blurTexture.setFilter(OpenGLTexture::Minification, OpenGLTexture::Nearest);
  p.m_blurTexture.setSize(width, height);
  p.m_blurTexture.allocate();
  p.m_blurTexture.release();

  // Motion Blur FBO
  p.m_fbo.bind();
  p.m_fbo.attachTexture2D(OpenGLFramebufferObject::TargetDraw, OpenGLFramebufferObject::ColorAttachment0, p.m_blurTexture);
  p.m_fbo.drawBuffers(OpenGLFramebufferObject::ColorAttachment0);
  p.m_fbo.validate();
  p.m_fbo.release();
}

void MotionBlurPass::commit(OpenGLViewport &view)
{
  (void)view;
}

void MotionBlurPass::render(OpenGLScene &scene)
{
  (void)scene;
  if (!active()) return;
  P(MotionBlurPassPrivate);
  OpenGLMarkerScoped _("Motion Blur Pass");

  // Blur Texture
  p.m_fbo.bind();
  p.m_program->bind();
  if (p.m_dirty)
  {
    p.m_dirty = false;
    p.m_program->setUniformValue("MaxSamples", p.m_maxSamples);
    p.m_program->setUniformValue("Power", p.m_power);
  }
  p.m_quadGL.draw();
  p.m_program->release();
  p.m_fbo.release();

  // Bind Lightbuffer
  GL::glActiveTexture(OpenGLTexture::beginTextureUnits() + K_LIGHT_BUFFER_TEXTURE_BINDING);
  p.m_blurTexture.bind();
  GL::glActiveTexture(OpenGLTexture::beginTextureUnits());
}

void MotionBlurPass::teardown()
{
  P(MotionBlurPassPrivate);
  delete p.m_program;
  delete m_private;
}

void MotionBlurPass::setPower(float pwr)
{
  P(MotionBlurPassPrivate);
  p.m_power = pwr;
  p.m_dirty = true;
}

void MotionBlurPass::setMaxSamples(int s)
{
  P(MotionBlurPassPrivate);
  p.m_maxSamples = s;
  p.m_dirty = true;
}
