#include "openglfunctions.h"

OpenGLFunctions *GL::m_functions;

OpenGLFunctions *GL::getInstance()
{
  return GL::m_functions;
}

void GL::setInstance(OpenGLFunctions *f)
{
  GL::m_functions = f;
}

int GL::getInteger(GLenum property)
{
  GLint value;
  GL::glGetIntegerv(property, &value);
  return static_cast<int>(value);
}
