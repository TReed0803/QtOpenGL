#include "openglpointlightgroup.h"

#include <KMath>
#include <KMatrix4x4>
#include <OpenGLMesh>
#include <OpenGLElementType>

void OpenGLPointLightGroup::setMesh(const OpenGLMesh &mesh)
{
  m_mesh = mesh;
  m_mesh.bind();
  m_buffer.bind();
  m_mesh.vertexAttribPointerDivisor(1, 3,     OpenGLElementType::Float, false, sizeof(DataType), DataType::TranslationOffset() , 1);
  m_mesh.vertexAttribPointerDivisor(2, 4,     OpenGLElementType::Float, false, sizeof(DataType), DataType::AttenuationOffset() , 1);
  m_mesh.vertexAttribPointerDivisor(3, 3,     OpenGLElementType::Float, false, sizeof(DataType), DataType::DiffuseOffset()     , 1);
  m_mesh.vertexAttribPointerDivisor(4, 3,     OpenGLElementType::Float, false, sizeof(DataType), DataType::SpecularOffset()    , 1);
  m_mesh.vertexAttribPointerDivisor(5, 4, 4,  OpenGLElementType::Float, false, sizeof(DataType), DataType::PerpectiveOffset()  , 1);
  m_mesh.release();
}

void OpenGLPointLightGroup::update(const KMatrix4x4 &perspective, const KMatrix4x4 &view)
{
  // Map Dynamic Data
  m_buffer.bind();
  m_buffer.reserve(m_lights.size());
  BufferType::RangeAccessFlags flags =
      BufferType::RangeInvalidate
    | BufferType::RangeUnsynchronized
    | BufferType::RangeWrite;
  DataPointer dest = m_buffer.mapRange(0, m_lights.size(), flags);

  if (dest == NULL)
  {
    qFatal("Failed to map the buffer range!");
  }

  // Upload data to GPU
  DataPointer lightDest;
  LightPointer lightSource;
  KMatrix4x4 worldToPersp = perspective * view;
  for (int i = 0; i < m_lights.size(); ++i)
  {
    lightDest   = &dest[i];
    lightSource = m_lights[i];
    lightDest->m_attenuation  = Karma::ToGlm(lightSource->attenuation(), lightSource->radius());
    lightDest->m_diffuse      = Karma::ToGlm(lightSource->diffuse());
    lightDest->m_perspTrans   = Karma::ToGlm(worldToPersp * lightSource->toMatrix());
    lightDest->m_specular     = Karma::ToGlm(lightSource->specular());
    lightDest->m_viewTrans    = Karma::ToGlm(view * lightSource->translation());
  }

  // Finalize Mapping
  m_buffer.unmap();
  m_buffer.release();
}

void OpenGLPointLightGroup::draw()
{
  m_mesh.bind();
  m_mesh.drawInstanced(0, m_lights.size());
  m_mesh.release();
}
