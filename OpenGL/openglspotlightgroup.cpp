#include "openglspotlightgroup.h"

#include <KMath>
#include <KMatrix4x4>
#include <OpenGLMesh>
#include <OpenGLElementType>

void OpenGLSpotLightGroup::initializeMesh(OpenGLMesh &mesh)
{
  mesh.vertexAttribPointerDivisor(1, 3,     OpenGLElementType::Float, false, sizeof(DataType), DataType::TranslationOffset() , 1);
  mesh.vertexAttribPointerDivisor(2, 3,     OpenGLElementType::Float, false, sizeof(DataType), DataType::DirectionOffset()   , 1);
  mesh.vertexAttribPointerDivisor(3, 2,     OpenGLElementType::Float, false, sizeof(DataType), DataType::AnglesOffset()      , 1);
  mesh.vertexAttribPointerDivisor(4, 4,     OpenGLElementType::Float, false, sizeof(DataType), DataType::AttenuationOffset() , 1);
  mesh.vertexAttribPointerDivisor(5, 3,     OpenGLElementType::Float, false, sizeof(DataType), DataType::DiffuseOffset()     , 1);
  mesh.vertexAttribPointerDivisor(6, 3,     OpenGLElementType::Float, false, sizeof(DataType), DataType::SpecularOffset()    , 1);
  mesh.vertexAttribPointerDivisor(7, 4, 4,  OpenGLElementType::Float, false, sizeof(DataType), DataType::PerpectiveOffset()  , 1);
}

void OpenGLSpotLightGroup::translateData(const KMatrix4x4 &perspective, const KMatrix4x4 &view, DataPointer data)
{
  // Upload data to GPU
  DataPointer lightDest;
  LightPointer lightSource;
  KMatrix3x3 viewRotScale(view);
  KMatrix4x4 worldToPersp = perspective * view;
  for (int i = 0; i < m_lights.size(); ++i)
  {
    lightDest   = &data[i];
    lightSource = m_lights[i];
    lightDest->m_angles       = Karma::ToGlm(lightSource->innerAngle(), lightSource->outerAngle());
    lightDest->m_attenuation  = Karma::ToGlm(lightSource->attenuation(), lightSource->depth());
    lightDest->m_diffuse      = Karma::ToGlm(lightSource->diffuse());
    lightDest->m_direction    = glm::normalize(Karma::ToGlm(viewRotScale * lightSource->direction()));
    lightDest->m_perspTrans   = Karma::ToGlm(worldToPersp * lightSource->toMatrix());
    lightDest->m_specular     = Karma::ToGlm(lightSource->specular());
    lightDest->m_viewTrans    = Karma::ToGlm(view * lightSource->translation());
  }

}

void OpenGLSpotLightGroup::draw()
{
  m_mesh.bind();
  m_mesh.drawInstanced(0, m_lights.size());
  m_mesh.release();
}
