#include "lightaccumulationpass.h"

#include <KMacros>
#include <OpenGLScene>

#include <OpenGLSpotLightGroup>
#include <OpenGLPointLightGroup>
#include <OpenGLDirectionLightGroup>
#include <OpenGLTexture>
#include <OpenGLFramebufferObject>
#include <OpenGLViewport>

class LightAccumulationPassPrivate
{
public:
};

LightAccumulationPass::LightAccumulationPass() :
  m_private(0)
{
  // Intentionally Empty
}

void LightAccumulationPass::initialize()
{
  m_private = new LightAccumulationPassPrivate;
}

void LightAccumulationPass::resize(int width, int height)
{
  // Intentionally Empty
}

void LightAccumulationPass::commit(OpenGLViewport &view)
{
  // Unused
  (void)view;
}

void LightAccumulationPass::render(OpenGLScene &scene)
{
  (void)scene;
  P(LightAccumulationPassPrivate);
  OpenGLMarkerScoped _("Light Accumulation Pass");

  GL::glDisable(GL_DEPTH_TEST);
  GL::glDepthMask(GL_FALSE);
  GL::glEnable(GL_BLEND);
  GL::glBlendFunc(GL_ONE, GL_ONE);

  scene.renderLights();

  GL::glDisable(GL_BLEND);
  GL::glDepthMask(GL_TRUE);
  GL::glEnable(GL_DEPTH_TEST);
}

void LightAccumulationPass::teardown()
{
  delete m_private;
}
