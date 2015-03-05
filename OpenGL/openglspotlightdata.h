#ifndef OPENGLSPOTLIGHTDATA_H
#define OPENGLSPOTLIGHTDATA_H OpenGLSpotLightData

#include <glm/glm.hpp>

class OpenGLSpotLightData
{
public:
  static int TranslationOffset();
  static int DirectionOffset();
  static int AnglesOffset();
  static int AttenuationOffset();
  static int DiffuseOffset();
  static int SpecularOffset();
  static int PerpectiveOffset();
  glm::vec3 m_viewTrans;   // { x, y, z }
  glm::vec3 m_direction;   // { x, y, z }
  glm::vec2 m_angles;      // { inner, outer - inner }
  glm::vec4 m_attenuation; // { k, d, d^2, radius }
  glm::vec3 m_diffuse;     // { r, g, b }
  glm::vec3 m_specular;    // { r, g, b }
  glm::mat4 m_perspTrans;  // { Model->View->Persp }
};

inline int OpenGLSpotLightData::TranslationOffset()
{
  return offsetof(OpenGLSpotLightData, m_viewTrans);
}

inline int OpenGLSpotLightData::DirectionOffset()
{
  return offsetof(OpenGLSpotLightData, m_direction);
}

inline int OpenGLSpotLightData::AnglesOffset()
{
  return offsetof(OpenGLSpotLightData, m_angles);
}

inline int OpenGLSpotLightData::AttenuationOffset()
{
  return offsetof(OpenGLSpotLightData, m_attenuation);
}

inline int OpenGLSpotLightData::DiffuseOffset()
{
  return offsetof(OpenGLSpotLightData, m_diffuse);
}

inline int OpenGLSpotLightData::SpecularOffset()
{
  return offsetof(OpenGLSpotLightData, m_specular);
}

inline int OpenGLSpotLightData::PerpectiveOffset()
{
  return offsetof(OpenGLSpotLightData, m_perspTrans);
}

#endif // OPENGLSPOTLIGHTDATA_H
