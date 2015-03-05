#ifndef OPENGLDIRECTIONLIGHTDATA_H
#define OPENGLDIRECTIONLIGHTDATA_H OpenGLDirectionLightData

#include <glm/glm.hpp>

class OpenGLDirectionLightData
{
public:
  static int DirectionOffset();
  static int DiffuseOffset();
  static int SpecularOffset();
  glm::vec3 m_direction;
  glm::vec3 m_diffuse;
  glm::vec3 m_specular;
};

inline int OpenGLDirectionLightData::DirectionOffset()
{
  return offsetof(OpenGLDirectionLightData, m_direction);
}

inline int OpenGLDirectionLightData::DiffuseOffset()
{
  return offsetof(OpenGLDirectionLightData, m_diffuse);
}

inline int OpenGLDirectionLightData::SpecularOffset()
{
  return offsetof(OpenGLDirectionLightData, m_specular);
}

#endif // OPENGLDIRECTIONLIGHTDATA_H

