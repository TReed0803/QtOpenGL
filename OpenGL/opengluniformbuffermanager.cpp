#include "opengluniformbuffermanager.h"
#include <unordered_map>
#include <Qt>

typedef std::unordered_map<std::string, OpenGLUniformBufferObject*> UBOMap;
static UBOMap m_staticUboMap;

void OpenGLUniformBufferManager::addUniformBufferObject(std::string const &name, OpenGLUniformBufferObject *ubo)
{
  OpenGLUniformBufferObject *uboOrig = find(name);
  if (uboOrig)
  {
    qFatal("Error! A Uniform Buffer Object is allready bound to the name `%s`.", name.c_str());
  }
  m_staticUboMap[name] = ubo;
}

OpenGLUniformBufferObject *OpenGLUniformBufferManager::find(std::string const &name)
{
  UBOMap::const_iterator it = m_staticUboMap.find(name);
  if (it != m_staticUboMap.end())
  {
    return it->second;
  }
  return Q_NULLPTR;
}
