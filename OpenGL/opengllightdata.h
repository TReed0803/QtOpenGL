#ifndef OPENGLLIGHTDATA_H
#define OPENGLLIGHTDATA_H OpenGLLightData

#include <glm/glm.hpp>

class OpenGLLightData
{
public:
  static int TranslationOffset();
  static int DirectionOffset();
  static int AttenuationOffset();
  static int DiffuseOffset();
  static int SpecularOffset();
  static int PerpectiveOffset();
  glm::mat4 m_perspTrans;     // { Model->View->Persp }
  glm::mat4 m_cViewToLPersp;  // { View->World->LView->LPersp }
  glm::vec4 m_attenuation;    // { k, d, d^2, radius }
  glm::vec3 m_viewTrans;      // { x, y, z }
  float m_innerAngle;         // { inner }
  glm::vec3 m_direction;      // { x, y, z }
  float m_outerAngle;         // { outer }
  glm::vec3 m_diffuse;        // { r, g, b }
  float m_diffAngle;          // { outer - inner }
  glm::vec3 m_specular;       // { r, g, b }
};

inline int OpenGLLightData::TranslationOffset()
{
  return offsetof(OpenGLLightData, m_viewTrans);
}

inline int OpenGLLightData::DirectionOffset()
{
  return offsetof(OpenGLLightData, m_direction);
}

inline int OpenGLLightData::AttenuationOffset()
{
  return offsetof(OpenGLLightData, m_attenuation);
}

inline int OpenGLLightData::DiffuseOffset()
{
  return offsetof(OpenGLLightData, m_diffuse);
}

inline int OpenGLLightData::SpecularOffset()
{
  return offsetof(OpenGLLightData, m_specular);
}

inline int OpenGLLightData::PerpectiveOffset()
{
  return offsetof(OpenGLLightData, m_perspTrans);
}

#endif // OPENGLLIGHTDATA_H

