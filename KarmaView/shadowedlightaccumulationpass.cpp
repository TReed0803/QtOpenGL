#include "shadowedlightaccumulationpass.h"

#include <KMacros>
#include <OpenGLMarkerScoped>
#include <OpenGLFunctions>
#include <OpenGLScene>

class ShadowedLightAccumulationPassPrivate
{
public:
};

ShadowedLightAccumulationPass::ShadowedLightAccumulationPass() :
  m_private(0)
{
  // Intentionally Empty
}

void ShadowedLightAccumulationPass::initialize()
{

}

void ShadowedLightAccumulationPass::resize(int width, int height)
{

}

void ShadowedLightAccumulationPass::commit(OpenGLViewport &view)
{

}

void ShadowedLightAccumulationPass::render(OpenGLScene &scene)
{
  P(ShadowedLightAccumulationPassPrivate);
  OpenGLMarkerScoped _("Shadow-Casting Light Accumulation Pass");

  GL::glDisable(GL_DEPTH_TEST);
  GL::glDepthMask(GL_FALSE);
  GL::glEnable(GL_BLEND);
  GL::glBlendFunc(GL_ONE, GL_ONE);

  scene.renderShadowedLights();

  GL::glDisable(GL_BLEND);
  GL::glDepthMask(GL_TRUE);
  GL::glEnable(GL_DEPTH_TEST);
}

void ShadowedLightAccumulationPass::teardown()
{

}
