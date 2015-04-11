#include "viewportpresentationpass.h"

#include <KMacros>
#include <KSize>
#include <KSizeF>
#include <KVector2D>
#include <OpenGLMesh>
#include <OpenGLShaderProgram>
#include <OpenGLViewport>
#include <OpenGLScene>
#include <OpenGLFramebufferObject>
#include <KCamera3D>

class ViewportPresentationPassPrivate
{
public:
  int m_width, m_height, m_x, m_y;
  OpenGLMesh m_quadGL;
  OpenGLShaderProgram *m_program;
};

ViewportPresentationPass::ViewportPresentationPass()
{
  // Intentionally Empty
}

void ViewportPresentationPass::initialize()
{
  m_private = new ViewportPresentationPassPrivate;
  P(ViewportPresentationPassPrivate);

  p.m_program = new OpenGLShaderProgram;
  p.m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/resources/shaders/gbuffer/viewport.vert");
  p.m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/resources/shaders/gbuffer/viewport.frag");
  p.m_program->link();

  p.m_quadGL.create(":/resources/objects/quad.obj");
}

void ViewportPresentationPass::resize(int width, int height)
{
  P(ViewportPresentationPassPrivate);
  p.m_width = width;
  p.m_height = height;
}

void ViewportPresentationPass::commit(OpenGLViewport &view)
{
  P(ViewportPresentationPassPrivate);
  p.m_x = view.screenSize().width() * (view.origin().x());
  p.m_y = view.screenSize().height() * (1.0f - view.region().height() - view.origin().y());
}

void ViewportPresentationPass::render(OpenGLScene &scene)
{
  (void)scene;
  P(ViewportPresentationPassPrivate);
  OpenGLMarkerScoped _("Viewport Presentation Pass");

  OpenGLFramebufferObject::release();
  GL::glViewport(p.m_x, p.m_y, p.m_width, p.m_height);
  p.m_program->bind();
  p.m_quadGL.draw();
  p.m_program->release();
}

void ViewportPresentationPass::teardown()
{
  P(ViewportPresentationPassPrivate);
  delete p.m_program;
  delete m_private;
}

