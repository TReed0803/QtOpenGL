#include "opengldirectionlightgroup.h"

#include <KMatrix4x4>
#include <KMath>
#include <OpenGLMesh>
#include <QVector4D>

void OpenGLDirectionLightGroup::initializeMesh(OpenGLMesh &mesh)
{
  mesh.vertexAttribPointerDivisor(1, 3, OpenGLElementType::Float, false, sizeof(DataType), DataType::DirectionOffset() , 1);
  mesh.vertexAttribPointerDivisor(2, 3, OpenGLElementType::Float, false, sizeof(DataType), DataType::DiffuseOffset()   , 1);
  mesh.vertexAttribPointerDivisor(3, 3, OpenGLElementType::Float, false, sizeof(DataType), DataType::SpecularOffset()  , 1);
}

void OpenGLDirectionLightGroup::translateData(const KMatrix4x4 &perspective, const KMatrix4x4 &view, DataPointer data)
{
  (void)perspective;

  // Upload data to GPU
  DataPointer lightDest;
  LightPointer lightSource;
  KMatrix3x3 viewRotScale = KMatrix3x3(view);
  for (int i = 0; i < m_lights.size(); ++i)
  {
    lightDest   = &data[i];
    lightSource = m_lights[i];
    lightDest->m_direction    = glm::normalize(Karma::ToGlm(viewRotScale * lightSource->direction()));
    lightDest->m_diffuse      = Karma::ToGlm(lightSource->diffuse());
    lightDest->m_specular     = Karma::ToGlm(lightSource->specular());
  }

}

void OpenGLDirectionLightGroup::draw()
{
  m_mesh.bind();
  m_mesh.drawInstanced(0, m_lights.size());
  m_mesh.release();
}
