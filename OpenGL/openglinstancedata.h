#ifndef OPENGLINSTANCEDATA_H
#define OPENGLINSTANCEDATA_H OpenGLInstanceData

#include <glm/glm.hpp>

class OpenGLInstanceData
{
public:
  static int DiffuseOffset();
  static int SpecularOffset();
  static int CurrModelViewOffset();
  static int PrevModelViewOffset();
  static int NormalTransformOffset();
  glm::vec3 m_diffuse;
  glm::vec4 m_specular;
  glm::mat4 m_currModelView;
  glm::mat4 m_prevModelView;
  glm::mat4 m_normalTransform;
};

inline int OpenGLInstanceData::DiffuseOffset()
{
  return offsetof(OpenGLInstanceData, m_diffuse);
}

inline int OpenGLInstanceData::SpecularOffset()
{
  return offsetof(OpenGLInstanceData, m_specular);
}

inline int OpenGLInstanceData::CurrModelViewOffset()
{
  return offsetof(OpenGLInstanceData, m_currModelView);
}

inline int OpenGLInstanceData::PrevModelViewOffset()
{
  return offsetof(OpenGLInstanceData, m_prevModelView);
}

inline int OpenGLInstanceData::NormalTransformOffset()
{
  return offsetof(OpenGLInstanceData, m_normalTransform);
}

#endif // OPENGLINSTANCEDATA_H

