#include "gbufferpass.h"

#include <KMacros>
#include <OpenGLScene>
#include <OpenGLMesh>
#include <OpenGLTexture>
#include <OpenGLShaderProgram>
#include <OpenGLFramebufferObject>
#include <OpenGLRenderer>
#include <OpenGLRenderBlock>
#include <OpenGLViewport>
#include <OpenGLBindings>

class GBufferPassPrivate
{
public:
  void constructTexture(OpenGLTexture &t, OpenGLInternalFormat f, int width, int height);

  // Render Helpers
  OpenGLMesh m_quad;
  OpenGLShaderProgram *m_program;
  OpenGLFramebufferObject m_gFbo;
  int m_width, m_height;

  // GBuffer
  OpenGLTexture m_gDepth;    // depth
  OpenGLTexture m_gGeometry; // normal normal vel vel
  OpenGLTexture m_gMaterial; // diff diff diff spec
  OpenGLTexture m_gSurface;  // exp
  OpenGLTexture m_gPhysical; // fresnel fresnel fresnel roughness
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
  p.m_width = width;
  p.m_height = height;

  // GBuffer Texture Storage
  p.constructTexture(p.m_gDepth, OpenGLInternalFormat::Depth32F, width, height);   // Depth
  p.constructTexture(p.m_gGeometry, OpenGLInternalFormat::Rgba32F, width, height); // Normal Normal Velocity Velocity
  p.constructTexture(p.m_gMaterial, OpenGLInternalFormat::Rgba8, width, height);   // Diffuse Diffuse Diffuse SpecularColor
  p.constructTexture(p.m_gSurface, OpenGLInternalFormat::R8, width, height);       // SpecularExp
  p.constructTexture(p.m_gPhysical, OpenGLInternalFormat::Rgba32F, width, height); // Fresnel Fresnel Fresnel Roughness

  // GBuffer Framebuffer
  p.m_gFbo.bind();
  p.m_gFbo.attachTexture2D(OpenGLFramebufferObject::TargetDraw, OpenGLFramebufferObject::ColorAttachment0, p.m_gGeometry);
  p.m_gFbo.attachTexture2D(OpenGLFramebufferObject::TargetDraw, OpenGLFramebufferObject::ColorAttachment1, p.m_gMaterial);
  p.m_gFbo.attachTexture2D(OpenGLFramebufferObject::TargetDraw, OpenGLFramebufferObject::ColorAttachment2, p.m_gSurface);
  p.m_gFbo.attachTexture2D(OpenGLFramebufferObject::TargetDraw, OpenGLFramebufferObject::ColorAttachment3, p.m_gPhysical);
  p.m_gFbo.attachTexture2D(OpenGLFramebufferObject::TargetDraw, OpenGLFramebufferObject::DepthAttachment,  p.m_gDepth);
  p.m_gFbo.drawBuffers(OpenGLFramebufferObject::ColorAttachment0, OpenGLFramebufferObject::ColorAttachment1, OpenGLFramebufferObject::ColorAttachment2, OpenGLFramebufferObject::ColorAttachment3);
  p.m_gFbo.validate();
  p.m_gFbo.release();
}

void GBufferPass::commit(OpenGLViewport &view)
{
  P(GBufferPassPrivate);

  // Activate Backbuffers
  GL::glActiveTexture(OpenGLTexture::beginTextureUnits() + K_DEPTH_TEXTURE_BINDING);
  p.m_gDepth.bind();
  GL::glActiveTexture(OpenGLTexture::beginTextureUnits() + K_GEOMETRY_TEXTURE_BINDING);
  p.m_gGeometry.bind();
  GL::glActiveTexture(OpenGLTexture::beginTextureUnits() + K_MATERIAL_TEXTURE_BINDING);
  p.m_gMaterial.bind();
  GL::glActiveTexture(OpenGLTexture::beginTextureUnits() + K_SURFACE_TEXTURE_BINDING);
  p.m_gSurface.bind();
  GL::glActiveTexture(OpenGLTexture::beginTextureUnits() + K_PHYSICAL_TEXTURE_BINDING);
  p.m_gPhysical.bind();
  GL::glActiveTexture(OpenGLTexture::beginTextureUnits());
}

void GBufferPass::render(OpenGLScene &scene)
{
  P(GBufferPassPrivate);
  OpenGLMarkerScoped _("Generate G Buffer");

  // Generate the GBuffer
  p.m_program->bind();
  p.m_gFbo.bind();
  GL::glViewport(0, 0, p.m_width, p.m_height);
  GL::glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  GL::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  scene.renderGeometry();
  p.m_gFbo.release();
  p.m_program->release();
}

void GBufferPass::teardown()
{
  P(GBufferPassPrivate);
  delete p.m_program;
  delete m_private;
}
