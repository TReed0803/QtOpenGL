#ifndef OPENGLAREALIGHTDATA_H
#define OPENGLAREALIGHTDATA_H OpenGLAreaLightData

#include <glm/glm.hpp>

class OpenGLAreaLightData
{
public:
  glm::mat4 m_modelToPersp;
  glm::mat4 m_viewToLightPersp;
  glm::vec3 v_viewPosition;
  float f_radius;
  glm::vec3 v_color;
  float f_intensity;
  glm::vec4 v_data0;
  glm::vec4 v_data1;
  glm::vec4 v_data2;
  glm::vec4 v_data3;
};

#endif // OPENGLAREALIGHTDATA_H

