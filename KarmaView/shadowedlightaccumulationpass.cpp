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
  scene.renderShadowedLights();
}

void ShadowedLightAccumulationPass::teardown()
{

}
