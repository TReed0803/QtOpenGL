#ifndef OPENGLUNIFORMBUFFERMANAGER_H
#define OPENGLUNIFORMBUFFERMANAGER_H OpenGLUniformBufferManager

#include <string>

class OpenGLShaderProgram;
class OpenGLUniformBufferObject;

class OpenGLUniformBufferManager
{
public:
  static void setBindingIndex(std::string const &name, const OpenGLUniformBufferObject &ubo);
  static void setBindingProgram(std::string const &name, unsigned location, OpenGLShaderProgram &program);
};

#endif // OPENGLUNIFORMBUFFERMANAGER_H
