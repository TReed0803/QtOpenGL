#include "openglpointlightgroup.h"

#include <KMath>
#include <KMatrix4x4>
#include <OpenGLMesh>
#include <OpenGLElementType>
#include <OpenGLRenderBlock>

bool OpenGLPointLightGroup::create()
{
  // Create Regular Shader
  m_regularLight = new OpenGLShaderProgram();
  m_regularLight->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/resources/shaders/lighting/pointLight.vert");
  m_regularLight->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/resources/shaders/lighting/pointLight.frag");
  m_regularLight->link();

  // Create Shadowed Shader
  m_shadowCastingLight = new OpenGLShaderProgram();
  m_shadowCastingLight->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/resources/shaders/lighting/shadowPointLight.vert");
  m_shadowCastingLight->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/resources/shaders/lighting/shadowPointLight.frag");
  m_shadowCastingLight->link();

  return LightGroup::create();
}

void OpenGLPointLightGroup::initializeMesh(OpenGLMesh &mesh)
{
  mesh.vertexAttribPointerDivisor(1, 3,     OpenGLElementType::Float, false, sizeof(DataType), DataType::TranslationOffset() , 1);
  mesh.vertexAttribPointerDivisor(2, 4,     OpenGLElementType::Float, false, sizeof(DataType), DataType::AttenuationOffset() , 1);
  mesh.vertexAttribPointerDivisor(3, 3,     OpenGLElementType::Float, false, sizeof(DataType), DataType::DiffuseOffset()     , 1);
  mesh.vertexAttribPointerDivisor(4, 3,     OpenGLElementType::Float, false, sizeof(DataType), DataType::SpecularOffset()    , 1);
  mesh.vertexAttribPointerDivisor(5, 4, 4,  OpenGLElementType::Float, false, sizeof(DataType), DataType::PerpectiveOffset()  , 1);
}

void OpenGLPointLightGroup::translateBuffer(const OpenGLRenderBlock &stats, DataPointer data, ConstLightIterator begin, ConstLightIterator end)
{
  // Upload data to GPU
  DataPointer lightDest;
  ConstLightPointer lightSource;
  while (begin != end)
  {
    lightDest   = data;
    lightSource = *begin;
    lightDest->m_attenuation  = Karma::ToGlm(lightSource->attenuation(), lightSource->radius());
    lightDest->m_diffuse      = Karma::ToGlm(lightSource->diffuse());
    lightDest->m_perspTrans   = stats.worldToPersp() * Karma::ToGlm(lightSource->toMatrix());
    lightDest->m_specular     = Karma::ToGlm(lightSource->specular());
    lightDest->m_viewTrans    = glm::vec3(stats.worldToView() * Karma::ToGlm(lightSource->translation(), 1.0f));
    ++data;
    ++begin;
  }
}

void OpenGLPointLightGroup::translateUniforms(const OpenGLRenderBlock &stats, Byte *data, OpenGLLightGroup::SizeType step, OpenGLLightGroup::ConstLightIterator begin, OpenGLLightGroup::ConstLightIterator end)
{
  // Upload data to GPU
  DataPointer lightDest;
  LightPointer lightSource;
  while (begin != end)
  {
    lightDest   = reinterpret_cast<DataType*>(data);
    lightSource = *begin;
    lightDest->m_attenuation  = Karma::ToGlm(lightSource->attenuation(), lightSource->radius());
    lightDest->m_diffuse      = Karma::ToGlm(lightSource->diffuse());
    lightDest->m_perspTrans   = stats.worldToPersp() * Karma::ToGlm(lightSource->toMatrix());
    lightDest->m_specular     = Karma::ToGlm(lightSource->specular());
    lightDest->m_viewTrans    = glm::vec3(stats.worldToView() * Karma::ToGlm(lightSource->translation(), 1.0f));
    data += step;
    ++begin;
  }
}
