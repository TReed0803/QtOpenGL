#ifndef OPENGLUNIFORMMANAGER_H
#define OPENGLUNIFORMMANAGER_H OpenGLUniformManager

#include <string>

class OpenGLShaderProgram;
class OpenGLUniformBufferObject;

class OpenGLUniformManager
{
public:
  static void setTextureSampler(std::string const &name, unsigned textureId);
  static void setUniformBufferIndex(std::string const &name, unsigned index);
  static void registerTextureSamplerCallbacks(std::string const &name, OpenGLShaderProgram &program);
  static void registerUniformBufferCallbacks(std::string const &name, OpenGLShaderProgram &program);
};

#endif // OPENGLUNIFORMMANAGER_H
