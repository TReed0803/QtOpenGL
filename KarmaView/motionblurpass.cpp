#include "motionblurpass.h"

#include <OpenGLMesh>
#include <OpenGLShaderProgram>
#include <OpenGLViewport>
#include <OpenGLScene>

class MotionBlurPassPrivate
{
public:
  OpenGLMesh m_quadGL;
  OpenGLShaderProgram *m_program;
};

MotionBlurPass::MotionBlurPass()
{
  m_private = new MotionBlurPassPrivate;
  P(MotionBlurPassPrivate);
  p.m_program = new OpenGLShaderProgram();
  p.m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/resources/shaders/gbuffer/main.vert");
  p.m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/resources/shaders/gbuffer/motionBlur.frag");
  p.m_program->link();
  p.m_quadGL.create(":/resources/objects/quad.obj");
}

void MotionBlurPass::initialize()
{

}

void MotionBlurPass::resize(int width, int height)
{
  (void)width;
  (void)height;
}

void MotionBlurPass::commit(OpenGLViewport &view)
{
  (void)view;
}

void MotionBlurPass::render(OpenGLScene &scene)
{
  (void)scene;
  P(MotionBlurPassPrivate);
  OpenGLMarkerScoped _("Motion Blur Pass");

  p.m_program->bind();
  p.m_quadGL.draw();
  p.m_program->release();
}

void MotionBlurPass::teardown()
{
  P(MotionBlurPassPrivate);
  delete p.m_program;
  delete m_private;
}
