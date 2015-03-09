#include "gbufferpass.h"

#include <KMacros>
#include <OpenGLMesh>
#include <OpenGLTexture>
#include <OpenGLShaderProgram>
#include <OpenGLFramebufferObject>
#include <OpenGLRenderer>

class GBufferPassPrivate
{
public:
  void constructTexture(OpenGLTexture &t, OpenGLInternalFormat f, int width, int height);

  // Render Helpers
  OpenGLMesh m_quad;
  OpenGLShaderProgram *m_program;
  OpenGLFramebufferObject m_gFbo;

  // GBuffer
  OpenGLTexture m_gDepth;    // depth
  OpenGLTexture m_gGeometry; // normal normal vel vel
  OpenGLTexture m_gMaterial; // diff diff diff spec
  OpenGLTexture m_gSurface;  // exp
};

void GBufferPassPrivate::constructTexture(OpenGLTexture &t, OpenGLInternalFormat f, int width, int height)
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

GBufferPass::GBufferPass() :
  m_private(0)
{
  // Intentionally Empty
}

void GBufferPass::initialize()
{
  m_private = new GBufferPassPrivate;
  P(GBufferPassPrivate);

  // Create Shader for GBuffer Pass
  p.m_program = new OpenGLShaderProgram();
  p.m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/resources/shaders/gbuffer.vert");
  p.m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/resources/shaders/gbuffer.frag");
  p.m_program->link();

  // Create Framebuffer Object
  p.m_gFbo.create();
}

void GBufferPass::resize(int width, int height)
{
  P(GBufferPassPrivate);

  // GBuffer Texture Storage
  p.constructTexture(p.m_gDepth, OpenGLInternalFormat::Depth32F, width, height);   // Depth
  p.constructTexture(p.m_gGeometry, OpenGLInternalFormat::Rgba32F, width, height); // Normal Normal Velocity Velocity
  p.constructTexture(p.m_gMaterial, OpenGLInternalFormat::Rgba8, width, height);   // Diffuse Diffuse Diffuse SpecularColor
  p.constructTexture(p.m_gSurface, OpenGLInternalFormat::R8, width, height);       // SpecularExp

  // GBuffer Framebuffer
  p.m_gFbo.bind();
  p.m_gFbo.attachTexture2D(OpenGLFramebufferObject::TargetDraw, OpenGLFramebufferObject::ColorAttachment0, p.m_gGeometry);
  p.m_gFbo.attachTexture2D(OpenGLFramebufferObject::TargetDraw, OpenGLFramebufferObject::ColorAttachment1, p.m_gMaterial);
  p.m_gFbo.attachTexture2D(OpenGLFramebufferObject::TargetDraw, OpenGLFramebufferObject::ColorAttachment2, p.m_gSurface);
  p.m_gFbo.attachTexture2D(OpenGLFramebufferObject::TargetDraw, OpenGLFramebufferObject::DepthAttachment,  p.m_gDepth);
  p.m_gFbo.drawBuffers(OpenGLFramebufferObject::ColorAttachment0, OpenGLFramebufferObject::ColorAttachment1, OpenGLFramebufferObject::ColorAttachment2);
  p.m_gFbo.validate();
  p.m_gFbo.release();

  // Activate Backbuffers
  GL::glActiveTexture(OpenGLTexture::endTextureUnits() - 1);
  p.m_gDepth.bind();
  GL::glActiveTexture(OpenGLTexture::endTextureUnits() - 2);
  p.m_gGeometry.bind();
  GL::glActiveTexture(OpenGLTexture::endTextureUnits() - 3);
  p.m_gMaterial.bind();
  GL::glActiveTexture(OpenGLTexture::endTextureUnits() - 4);
  p.m_gSurface.bind();
  GL::glActiveTexture(OpenGLTexture::beginTextureUnits());
}

void GBufferPass::stage()
{
  // Unused
}

void GBufferPass::commit(OpenGLRenderBlock &current, OpenGLRenderBlock &previous)
{
  // Unused
}

void GBufferPass::render(OpenGLRenderer &renderer)
{
  P(GBufferPassPrivate);

  OpenGLMarkerScoped _("Generate G Buffer");
  p.m_program->bind();
  p.m_gFbo.bind();
  GL::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  renderer.renderGeometry();
  p.m_gFbo.release();
  p.m_program->release();
}

void GBufferPass::teardown()
{
  P(GBufferPassPrivate);
  delete p.m_program;
  delete m_private;
}
