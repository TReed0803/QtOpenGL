#include "lightpass.h"

#include <KMacros>

#include <OpenGLSpotLightGroup>
#include <OpenGLPointLightGroup>
#include <OpenGLDirectionLightGroup>
#include <OpenGLTexture>
#include <OpenGLFramebufferObject>
#include <OpenGLRenderBlock>

class LightPassPrivate
{
public:
  void constructTexture(OpenGLTexture &t, OpenGLInternalFormat f, int width, int height);

  // Light Types
  OpenGLSpotLightGroup m_spotLights;
  OpenGLPointLightGroup m_pointLights;
  OpenGLDirectionLightGroup m_directionLights;

  // Light Accumulation
  OpenGLTexture m_lLighting;
  OpenGLFramebufferObject m_lFbo;
};

void LightPassPrivate::constructTexture(OpenGLTexture &t, OpenGLInternalFormat f, int width, int height)
{
  t.create(OpenGLTexture::Texture2D);
  t.bind();
  t.setInternalFormat(f);
  t.setWrapMode(OpenGLTexture::DirectionS, OpenGLTexture::ClampToEdge);
  t.setWrapMode(OpenGLTexture::DirectionT, OpenGLTexture::ClampToEdge);
  t.setFilter(OpenGLTexture::Magnification, OpenGLTexture::Nearest);
  t.setFilter(OpenGLTexture::Minification, OpenGLTexture::Nearest);
  t.setSize(width, height);
  t.allocate();
  t.release();
}

LightPass::LightPass() :
  m_private(0)
{
  // Intentionally Empty
}

void LightPass::initialize()
{
  m_private = new LightPassPrivate;
  P(LightPassPrivate);

  // Create Framebuffer Object
  p.m_lFbo.create();

  // Create Light Groups
  p.m_spotLights.create();
  p.m_spotLights.setMesh(":/resources/objects/spotLight.obj");
  p.m_pointLights.create();
  p.m_pointLights.setMesh(":/resources/objects/pointLight.obj");
  p.m_directionLights.create();
  p.m_directionLights.setMesh(":/resources/objects/quad.obj");
}

void LightPass::resize(int width, int height)
{
  P(LightPassPrivate);

  // Other Texture Storage
  p.constructTexture(p.m_lLighting, OpenGLInternalFormat::Rgba16, width, height);

  // Light Buffer
  p.m_lFbo.bind();
  p.m_lFbo.attachTexture2D(OpenGLFramebufferObject::TargetDraw, OpenGLFramebufferObject::ColorAttachment0, p.m_lLighting);
  p.m_lFbo.drawBuffers(OpenGLFramebufferObject::ColorAttachment0);
  p.m_lFbo.validate();
  p.m_lFbo.release();

  // Bind Lightbuffer
  GL::glActiveTexture(OpenGLTexture::endTextureUnits() - 5);
  p.m_lLighting.bind();
  GL::glActiveTexture(OpenGLTexture::beginTextureUnits());
}

void LightPass::stage()
{
  // Unused
}

void LightPass::commit(OpenGLRenderBlock &current, OpenGLRenderBlock &previous)
{
  P(LightPassPrivate);
  (void)previous;

  p.m_spotLights.update(current);
  p.m_pointLights.update(current);
  p.m_directionLights.update(current);
}

void LightPass::render(OpenGLRenderer &renderer)
{
  P(LightPassPrivate);
  OpenGLMarkerScoped _("Light Pass");

  GL::glDisable(GL_DEPTH_TEST);
  GL::glDepthMask(GL_FALSE);
  GL::glEnable(GL_BLEND);
  GL::glBlendFunc(GL_ONE, GL_ONE);

  p.m_lFbo.bind();
  GL::glClear(GL_COLOR_BUFFER_BIT);
  p.m_pointLights.draw();
  p.m_spotLights.draw();
  p.m_directionLights.draw();
  p.m_lFbo.release();

  GL::glDisable(GL_BLEND);
  GL::glDepthMask(GL_TRUE);
  GL::glEnable(GL_DEPTH_TEST);
}

void LightPass::teardown()
{
  delete m_private;
}

OpenGLSpotLightGroup &LightPass::spotLights()
{
  P(LightPassPrivate);
  return p.m_spotLights;
}

OpenGLPointLightGroup &LightPass::pointLights()
{
  P(LightPassPrivate);
  return p.m_pointLights;
}

OpenGLDirectionLightGroup &LightPass::directionLights()
{
  P(LightPassPrivate);
  return p.m_directionLights;
}

OpenGLSpotLight *LightPass::createSpotLight()
{
  P(LightPassPrivate);
  return p.m_spotLights.createLight();
}

OpenGLPointLight *LightPass::createPointLight()
{
  P(LightPassPrivate);
  return p.m_pointLights.createLight();
}

OpenGLDirectionLight *LightPass::createDirectionLight()
{
  P(LightPassPrivate);
  return p.m_directionLights.createLight();
}

