#include "opengldirectionlightgroup.h"

#include <KMatrix4x4>
#include <KMath>
#include <OpenGLMesh>
#include <QVector4D>
#include <OpenGLRenderBlock>

bool OpenGLDirectionLightGroup::create()
{
  // Create Regular Shader
  m_regularLight = new OpenGLShaderProgram();
  m_regularLight->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/resources/shaders/lighting/directionLight.vert");
  m_regularLight->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/resources/shaders/lighting/directionLight.frag");
  m_regularLight->link();

  // Create Shadowed Shader
  m_shadowCastingLight = new OpenGLShaderProgram();
  m_shadowCastingLight->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/resources/shaders/lighting/shadowDirectionLight.vert");
  m_shadowCastingLight->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/resources/shaders/lighting/shadowDirectionLight.frag");
  m_shadowCastingLight->link();

  return LightGroup::create();
}

void OpenGLDirectionLightGroup::initializeMesh(OpenGLMesh &mesh)
{
  mesh.vertexAttribPointerDivisor(1, 3, OpenGLElementType::Float, false, sizeof(DataType), DataType::DirectionOffset() , 1);
  mesh.vertexAttribPointerDivisor(2, 3, OpenGLElementType::Float, false, sizeof(DataType), DataType::DiffuseOffset()   , 1);
  mesh.vertexAttribPointerDivisor(3, 3, OpenGLElementType::Float, false, sizeof(DataType), DataType::SpecularOffset()  , 1);
}

void OpenGLDirectionLightGroup::translateBuffer(const OpenGLRenderBlock &stats, DataPointer data, ConstLightIterator begin, ConstLightIterator end)
{
  // Upload data to GPU
  DataPointer lightDest;
  ConstLightPointer lightSource;
  while (begin != end)
  {
    lightDest   = data;
    lightSource = *begin;
    lightDest->m_direction    = glm::vec3(glm::normalize(stats.worldToView() * Karma::ToGlm(lightSource->direction(), 0.0f)));
    lightDest->m_diffuse      = Karma::ToGlm(lightSource->diffuse());
    lightDest->m_specular     = Karma::ToGlm(lightSource->specular());
    ++data;
    ++begin;
  }

}

void OpenGLDirectionLightGroup::translateUniforms(const OpenGLRenderBlock &stats, Byte *data, OpenGLLightGroup::SizeType step, OpenGLLightGroup::ConstLightIterator begin, OpenGLLightGroup::ConstLightIterator end)
{
  // Upload data to GPU
  DataPointer lightDest;
  ConstLightPointer lightSource;
  while (begin != end)
  {
    lightDest   = reinterpret_cast<DataType*>(data);
    lightSource = *begin;
    lightDest->m_direction    = glm::vec3(glm::normalize(stats.worldToView() * Karma::ToGlm(lightSource->direction(), 0.0f)));
    lightDest->m_diffuse      = Karma::ToGlm(lightSource->diffuse());
    lightDest->m_specular     = Karma::ToGlm(lightSource->specular());
    data += step;
    ++begin;
  }

}
