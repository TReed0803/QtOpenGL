#ifndef OPENGLPOINTLIGHTDATA_H
#define OPENGLPOINTLIGHTDATA_H OpenGLPointLightData

#include <glm/glm.hpp>

class OpenGLPointLightData
{
public:
  static int TranslationOffset();
  static int AttenuationOffset();
  static int DiffuseOffset();
  static int SpecularOffset();
  static int PerpectiveOffset();
  glm::vec3 m_viewTrans;   // { x, y, z }
  glm::vec4 m_attenuation; // { k, d, d^2, radius }
  glm::vec3 m_diffuse;     // { r, g, b }
  glm::vec3 m_specular;    // { r, g, b }
  glm::mat4 m_perspTrans;  // { Model->View->Persp }
};

inline int OpenGLPointLightData::TranslationOffset()
{
  return offsetof(OpenGLPointLightData, m_viewTrans);
}

inline int OpenGLPointLightData::AttenuationOffset()
{
  return offsetof(OpenGLPointLightData, m_attenuation);
}

inline int OpenGLPointLightData::DiffuseOffset()
{
  return offsetof(OpenGLPointLightData, m_diffuse);
}

inline int OpenGLPointLightData::SpecularOffset()
{
  return offsetof(OpenGLPointLightData, m_specular);
}

inline int OpenGLPointLightData::PerpectiveOffset()
{
  return offsetof(OpenGLPointLightData, m_perspTrans);
}

#endif // OPENGLPOINTLIGHTDATA_H
