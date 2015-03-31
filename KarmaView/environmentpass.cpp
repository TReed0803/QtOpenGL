#include "environmentpass.h"

#include <KMacros>
#include <OpenGLMesh>
#include <OpenGLShaderProgram>
#include <OpenGLViewport>
#include <OpenGLScene>
#include <OpenGLEnvironment>
#include <OpenGLTexture>
#include <OpenGLBindings>

class EnvironmentPassPrivate
{
public:
  OpenGLMesh m_quadGL;
  OpenGLShaderProgram *m_environmentPass;
};

EnvironmentPass::EnvironmentPass() :
  m_private(0)
{
  // Intentionally Empty
}

void EnvironmentPass::initialize()
{
  m_private = new EnvironmentPassPrivate;
  P(EnvironmentPassPrivate);

  p.m_environmentPass = new OpenGLShaderProgram();
  p.m_environmentPass->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/resources/shaders/lighting/environment.vert");
  p.m_environmentPass->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/resources/shaders/lighting/environment.frag");
  p.m_environmentPass->link();

  p.m_quadGL.create(":/resources/objects/quad.obj");
}

void EnvironmentPass::resize(int width, int height)
{
  // Intentionally Empty
}

void EnvironmentPass::commit(OpenGLViewport &view)
{
  // Unused
  (void)view;
}

void EnvironmentPass::render(OpenGLScene &scene)
{
  (void)scene;
  P(EnvironmentPassPrivate);
  OpenGLMarkerScoped _("Light Accumulation Pass");

  GL::glDisable(GL_DEPTH_TEST);
  GL::glDepthMask(GL_FALSE);
  GL::glActiveTexture(OpenGLTexture::beginTextureUnits() + K_TEXTURE_0);
  scene.environment()->direct().bind();
  GL::glActiveTexture(OpenGLTexture::beginTextureUnits() + K_TEXTURE_1);
  scene.environment()->indirect().bind();
  p.m_environmentPass->bind();
  p.m_quadGL.draw();
  p.m_environmentPass->release();
  GL::glDepthMask(GL_TRUE);
  GL::glEnable(GL_DEPTH_TEST);
}

void EnvironmentPass::teardown()
{
  delete m_private;
}

