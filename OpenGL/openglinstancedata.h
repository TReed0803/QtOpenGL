#ifndef OPENGLINSTANCEDATA_H
#define OPENGLINSTANCEDATA_H OpenGLInstanceData

#include <glm/glm.hpp>

class OpenGLInstanceData
{
public:
  glm::mat4 m_currModelView;
  glm::mat4 m_prevModelView;
  glm::mat4 m_normalTransform;
};

#endif // OPENGLINSTANCEDATA_H

