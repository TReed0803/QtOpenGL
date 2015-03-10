#ifndef OPENGLMESHMANAGER_H
#define OPENGLMESHMANAGER_H OpenGLMeshManager

#include <string>
class OpenGLMesh;

class OpenGLMeshManager
{
public:
  static const OpenGLMesh &mesh(const std::string &name);
  static void setMesh(const std::string &name, const OpenGLMesh &mesh);
};

#endif // OPENGLMESHMANAGER_H
