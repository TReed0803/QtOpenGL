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

void ShadowedLightAccumulationPass::resize(int, int)
{

}

void ShadowedLightAccumulationPass::commit(OpenGLViewport &)
{

}

void ShadowedLightAccumulationPass::render(OpenGLScene &scene)
{
  OpenGLMarkerScoped _("Shadow-Casting Light Accumulation Pass");
  scene.renderShadowedLights();
}

void ShadowedLightAccumulationPass::teardown()
{

}
