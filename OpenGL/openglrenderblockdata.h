#ifndef OPENGLRENDERBLOCKDATA_H
#define OPENGLRENDERBLOCKDATA_H OpenGLRenderBlockData

#include <glm/glm.hpp>

class OpenGLRenderBlockData
{
public:
  // Most frequent to change
  glm::mat4 m_view;
  glm::mat4 i_view;

  // Co-dependant
  glm::mat4 m_viewProjection;
  glm::mat4 i_viewProjection;

  // Least frequent to change
  glm::mat4 m_projection;
  glm::mat4 i_projection;

  // Render Settings
  glm::vec2 v_dimensions;
  glm::vec2 v_offset;
  float f_nearPlane;
  float f_farPlane;
  float f_diffPlane;
  float f_sumPlane;
};

#endif // OPENGLRENDERBLOCKDATA_H
