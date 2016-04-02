#include "environmentpass.h"

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

class EnvironmentPassPrivate
{
public:
  OpenGLMesh m_quadGL;
  OpenGLShaderProgram *m_environmentPass;
  KSize m_dimensions;
  int m_uFresnel, m_uGeometry, m_uDistribution, m_uDistributionSample, m_uDiffuseScalar;
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

  // Get the subroutine locations
#if !defined(QT_NO_OPENGL) && !defined(QT_OPENGL_ES_2)
  p.m_environmentPass->bind();
  p.m_uFresnel = GL::glGetSubroutineUniformLocation(p.m_environmentPass->programId(), GL_FRAGMENT_SHADER, "uFresnel");
  p.m_uGeometry = GL::glGetSubroutineUniformLocation(p.m_environmentPass->programId(), GL_FRAGMENT_SHADER, "uGeometry");
  p.m_uDistribution = GL::glGetSubroutineUniformLocation(p.m_environmentPass->programId(), GL_FRAGMENT_SHADER, "uDistribution");
  p.m_uDistributionSample = GL::glGetSubroutineUniformLocation(p.m_environmentPass->programId(), GL_FRAGMENT_SHADER, "uDistributionSample");
  p.m_uDiffuseScalar = GL::glGetSubroutineUniformLocation(p.m_environmentPass->programId(), GL_FRAGMENT_SHADER, "uDiffuse");
  p.m_environmentPass->release();
#endif

  p.m_quadGL.create(":/resources/objects/quad.obj");
}

void EnvironmentPass::resize(int, int)
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
  if (!active()) return;
  P(EnvironmentPassPrivate);
  OpenGLMarkerScoped _("Light Accumulation Pass");

  GL::glDisable(GL_DEPTH_TEST);
  GL::glDepthMask(GL_FALSE);
  GL::glActiveTexture(OpenGLTexture::beginTextureUnits() + K_TEXTURE_0);
  scene.environment()->direct().bind();
  GL::glActiveTexture(OpenGLTexture::beginTextureUnits() + K_TEXTURE_1);
  scene.environment()->indirect().bind();
  p.m_environmentPass->bind();
  //p.m_environmentPass->setUniformValue("Dimensions", scene.environment()->directSize().width(), scene.environment()->directSize().height());
#if !defined(QT_NO_OPENGL) && !defined(QT_OPENGL_ES_2)
  std::string fName = "s" + FToCStr(OpenGLAbstractLightGroup::FFactor());
  std::string gName = "s" + GToCStr(OpenGLAbstractLightGroup::GFactor());
  std::string dName = "s" + DToCStr(OpenGLAbstractLightGroup::DFactor());
  std::string sName = "s" + DToCStr(OpenGLAbstractLightGroup::SFactor()) + "Sample";
  unsigned fIndex = GL::glGetSubroutineIndex(p.m_environmentPass->programId(), GL_FRAGMENT_SHADER, fName.c_str());
  unsigned gIndex = GL::glGetSubroutineIndex(p.m_environmentPass->programId(), GL_FRAGMENT_SHADER, gName.c_str());
  unsigned dIndex = GL::glGetSubroutineIndex(p.m_environmentPass->programId(), GL_FRAGMENT_SHADER, dName.c_str());
  unsigned sIndex = GL::glGetSubroutineIndex(p.m_environmentPass->programId(), GL_FRAGMENT_SHADER, sName.c_str());
  unsigned locations[4];
  if (p.m_uFresnel != -1) locations[p.m_uFresnel] = fIndex;
  if (p.m_uGeometry != -1) locations[p.m_uGeometry] = gIndex;
  if (p.m_uDistribution != -1) locations[p.m_uDistribution] = dIndex;
  if (p.m_uDistributionSample != -1) locations[p.m_uDistributionSample] = sIndex;
  GL::glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 4, locations);
#endif
  p.m_quadGL.draw();
  p.m_environmentPass->release();
  GL::glDepthMask(GL_TRUE);
  GL::glEnable(GL_DEPTH_TEST);
}

void EnvironmentPass::teardown()
{
  delete m_private;
}

