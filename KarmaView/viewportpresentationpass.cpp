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
  bool m_dirty;
  float a, b, c, d, e, f, w, exposure, exposureBias;
  int m_width, m_height, m_x, m_y;
  OpenGLMesh m_quadGL;
  OpenGLShaderProgram *m_program;

  ViewportPresentationPassPrivate();
};

ViewportPresentationPassPrivate::ViewportPresentationPassPrivate() :
  m_dirty(true)
{
  // Intentionally Empty
}

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
  if (p.m_dirty)
  {
    p.m_dirty = false;
    p.m_program->setUniformValue("A", p.a);
    p.m_program->setUniformValue("B", p.b);
    p.m_program->setUniformValue("C", p.c);
    p.m_program->setUniformValue("D", p.d);
    p.m_program->setUniformValue("E", p.e);
    p.m_program->setUniformValue("F", p.f);
    p.m_program->setUniformValue("W", p.w);
    p.m_program->setUniformValue("Exposure", p.exposure);
    p.m_program->setUniformValue("ExposureBias", p.exposureBias);
  }
  p.m_quadGL.draw();
  p.m_program->release();
}

void ViewportPresentationPass::teardown()
{
  P(ViewportPresentationPassPrivate);
  delete p.m_program;
  delete m_private;
}

void ViewportPresentationPass::setValues(float A, float B, float C, float D, float E, float F, float W)
{
  P(ViewportPresentationPassPrivate);
  p.a = A;
  p.b = B;
  p.c = C;
  p.d = D;
  p.e = E;
  p.f = F;
  p.w = W;
  p.m_dirty = true;
}

void ViewportPresentationPass::setExposureBias(float eb)
{
  P(ViewportPresentationPassPrivate);
  p.exposureBias = eb;
  p.m_dirty = true;
}

void ViewportPresentationPass::setExposure(float e)
{
  P(ViewportPresentationPassPrivate);
  p.exposure = e;
  p.m_dirty = true;
}
