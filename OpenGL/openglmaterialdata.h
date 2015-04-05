#ifndef OPENGLMATERIALDATA_H
#define OPENGLMATERIALDATA_H OpenGLMaterialData

#include <glm/glm.hpp>

class OpenGLMaterialData
{
public:
  glm::vec3 m_baseColor;
  float m_metallic;
  float m_roughness;
  float padding0;
  float padding1;
  float padding2;
};

#endif // OPENGLMATERIALDATA_H

