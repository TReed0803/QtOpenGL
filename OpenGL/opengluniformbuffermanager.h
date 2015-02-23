#ifndef OPENGLUNIFORMBUFFERMANAGER_H
#define OPENGLUNIFORMBUFFERMANAGER_H OpenGLUniformBufferManager

#include <string>

class OpenGLUniformBufferObject;

class OpenGLUniformBufferManager
{
public:
  static void addUniformBufferObject(std::string const &name, OpenGLUniformBufferObject *ubo);
  static OpenGLUniformBufferObject *find(std::string const &name);
};

#endif // OPENGLUNIFORMBUFFERMANAGER_H
