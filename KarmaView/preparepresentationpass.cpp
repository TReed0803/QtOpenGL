#include "preparepresentationpass.h"

#include <KMacros>
#include <OpenGLTexture>
#include <OpenGLFramebufferObject>
#include <OpenGLFunctions>
#include <OpenGLScene>

class PreparePresentationPassPrivate
{
public:
  void constructTexture(OpenGLTexture &t, OpenGLInternalFormat f, int width, int height);

  OpenGLTexture m_lLighting;
  OpenGLFramebufferObject m_lFbo;
};

void PreparePresentationPassPrivate::constructTexture(OpenGLTexture &t, OpenGLInternalFormat f, int width, int height)
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

PreparePresentationPass::PreparePresentationPass() :
  m_private(0)
{
  // Intentionally Empty
}

void PreparePresentationPass::initialize()
{
  m_private = new PreparePresentationPassPrivate;
  P(PreparePresentationPassPrivate);
  // Create Framebuffer Object
  p.m_lFbo.create();
}

void PreparePresentationPass::resize(int width, int height)
{
  P(PreparePresentationPassPrivate);

  // Other Texture Storage
  p.constructTexture(p.m_lLighting, OpenGLInternalFormat::Rgba16, width, height);

  // Light Buffer
  p.m_lFbo.bind();
  p.m_lFbo.attachTexture2D(OpenGLFramebufferObject::TargetDraw, OpenGLFramebufferObject::ColorAttachment0, p.m_lLighting);
  p.m_lFbo.drawBuffers(OpenGLFramebufferObject::ColorAttachment0);
  p.m_lFbo.validate();
  p.m_lFbo.release();
}

void PreparePresentationPass::commit(OpenGLViewport &view)
{
  P(PreparePresentationPassPrivate);
  // Bind Lightbuffer
  GL::glActiveTexture(OpenGLTexture::endTextureUnits() - 5);
  p.m_lLighting.bind();
  GL::glActiveTexture(OpenGLTexture::beginTextureUnits());
}

void PreparePresentationPass::render(OpenGLScene &scene)
{
  (void)scene;
  P(PreparePresentationPassPrivate);
  OpenGLMarkerScoped _("Presentation Preparation Pass");

  p.m_lFbo.bind();
  GL::glClear(GL_COLOR_BUFFER_BIT);
}

void PreparePresentationPass::teardown()
{
  delete m_private;
}
