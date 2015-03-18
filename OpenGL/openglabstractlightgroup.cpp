#include "openglabstractlightgroup.h"

#include <KHalfEdgeMesh>
#include <OpenGLShaderProgram>
#include <OpenGLBlurData>

bool OpenGLAbstractLightGroup::create()
{
  // Create the shadow texture
  m_shadowTexture.create(OpenGLTexture::Texture2D);
  m_shadowTexture.bind();
  m_shadowTexture.setSwizzle(OpenGLTexture::Red, OpenGLTexture::Red, OpenGLTexture::Red, OpenGLTexture::One);
  m_shadowTexture.setInternalFormat(OpenGLInternalFormat::R32F);
  m_shadowTexture.setWrapMode(OpenGLTexture::DirectionS, OpenGLTexture::ClampToEdge);
  m_shadowTexture.setWrapMode(OpenGLTexture::DirectionT, OpenGLTexture::ClampToEdge);
  m_shadowTexture.setFilter(OpenGLTexture::Magnification, OpenGLTexture::Nearest);
  m_shadowTexture.setFilter(OpenGLTexture::Minification, OpenGLTexture::Nearest);
  m_shadowTexture.setSize(800, 600);
  m_shadowTexture.allocate();
  m_shadowTexture.release();

  // Create the blur temporary texture
  m_blurTexture.create(OpenGLTexture::Texture2D);
  m_blurTexture.bind();
  m_blurTexture.setSwizzle(OpenGLTexture::Red, OpenGLTexture::Red, OpenGLTexture::Red, OpenGLTexture::One);
  m_blurTexture.setInternalFormat(OpenGLInternalFormat::R32F);
  m_blurTexture.setWrapMode(OpenGLTexture::DirectionS, OpenGLTexture::ClampToEdge);
  m_blurTexture.setWrapMode(OpenGLTexture::DirectionT, OpenGLTexture::ClampToEdge);
  m_blurTexture.setFilter(OpenGLTexture::Magnification, OpenGLTexture::Nearest);
  m_blurTexture.setFilter(OpenGLTexture::Minification, OpenGLTexture::Nearest);
  m_blurTexture.setSize(800, 600);
  m_blurTexture.allocate();
  m_blurTexture.release();

  // Create shadow depth
  m_shadowDepth.create(OpenGLTexture::Texture2D);
  m_shadowDepth.bind();
  m_shadowDepth.setSwizzle(OpenGLTexture::Red, OpenGLTexture::Red, OpenGLTexture::Red, OpenGLTexture::One);
  m_shadowDepth.setInternalFormat(OpenGLInternalFormat::Depth32F);
  m_shadowDepth.setWrapMode(OpenGLTexture::DirectionS, OpenGLTexture::ClampToEdge);
  m_shadowDepth.setWrapMode(OpenGLTexture::DirectionT, OpenGLTexture::ClampToEdge);
  m_shadowDepth.setFilter(OpenGLTexture::Magnification, OpenGLTexture::Nearest);
  m_shadowDepth.setFilter(OpenGLTexture::Minification, OpenGLTexture::Nearest);
  m_shadowDepth.setSize(800, 600);
  m_shadowDepth.allocate();
  m_shadowDepth.release();

  // Create the shadow fbo
  m_shadowMappingFbo.create();
  m_shadowMappingFbo.bind();
  m_shadowMappingFbo.attachTexture2D(OpenGLFramebufferObject::TargetDraw, OpenGLFramebufferObject::ColorAttachment0, m_shadowTexture);
  m_shadowMappingFbo.attachTexture2D(OpenGLFramebufferObject::TargetDraw, OpenGLFramebufferObject::DepthAttachment, m_shadowDepth);
  m_shadowMappingFbo.drawBuffers(OpenGLFramebufferObject::ColorAttachment0);
  bool ret = m_shadowMappingFbo.validate();
  m_shadowMappingFbo.release();

  // Setup blur data
  OpenGLBlurData data(32, 10.0f);
  m_blurData.create();
  m_blurData.bind();
  m_blurData.allocate(&data, sizeof(OpenGLBlurData));
  m_blurData.release();
  m_blurData.bindBase(4);

  // Create the Compute Blur Program
  m_blurProgram = new OpenGLShaderProgram;
  m_blurProgram->addShaderFromSourceFile(QOpenGLShader::Compute, ":/resources/shaders/compute/gaussianBlur.comp");
  m_blurProgram->link();
  m_blurProgram->bind();
  m_blurProgram->uniformBlockBinding("BlurData", 4);
  m_blurProgram->setUniformValue("src", 0);
  m_blurProgram->setUniformValue("dst", 1);
  m_blurProgram->release();

  return ret;
}

void OpenGLAbstractLightGroup::setMesh(const OpenGLMesh &mesh)
{
  m_mesh = mesh;
  m_mesh.bind();
  prepMesh(m_mesh);
  m_mesh.release();
}

void OpenGLAbstractLightGroup::setMesh(const KHalfEdgeMesh &mesh)
{
  OpenGLMesh glMesh;
  glMesh.create(mesh);
  setMesh(glMesh);
}

void OpenGLAbstractLightGroup::setMesh(const char *filepath)
{
  KHalfEdgeMesh mesh;
  mesh.create(filepath);
  setMesh(mesh);
}
