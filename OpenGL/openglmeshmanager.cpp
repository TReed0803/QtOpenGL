#include "openglmeshmanager.h"

#include <unordered_map>
#include <OpenGLMesh>

typedef std::unordered_map<std::string, OpenGLMesh> OpenGLMeshMap;
static OpenGLMeshMap sg_meshMap;

const OpenGLMesh &OpenGLMeshManager::mesh(const std::string &name)
{
  return sg_meshMap[name];
}

void OpenGLMeshManager::setMesh(const std::string &name, const OpenGLMesh &mesh)
{
  sg_meshMap[name] = mesh;
}
