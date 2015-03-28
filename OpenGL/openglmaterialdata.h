#ifndef OPENGLMATERIALDATA_H
#define OPENGLMATERIALDATA_H OpenGLMaterialData

#include <glm/glm.hpp>

class OpenGLMaterialData
{
public:
  glm::vec3 m_diffuse;
  float padding0;
  glm::vec3 m_fresnel;
  float padding1;
  glm::vec2 m_roughness;
  float padding2;
  float padding3;
};

#endif // OPENGLMATERIALDATA_H

