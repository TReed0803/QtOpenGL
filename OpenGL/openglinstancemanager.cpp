#include "openglinstancemanager.h"

#include <vector>
#include <KMacros>
#include <OpenGLMeshManager>
#include <OpenGLInstance>
#include <OpenGLInstanceGroup>
#include <OpenGLMesh>
#include <string>

struct OpenGLInstanceGroupPair
{
  std::string meshFile;
  OpenGLInstanceGroup group;
};

class OpenGLInstanceManagerPrivate
{
public:
  std::vector<OpenGLInstanceGroupPair*> m_groups;
  std::vector<OpenGLInstance*> m_instances;
};

OpenGLInstanceManager::OpenGLInstanceManager() :
  m_private(new OpenGLInstanceManagerPrivate)
{
  // Intentionally Empty
}

OpenGLInstanceManager::~OpenGLInstanceManager()
{
  // Intentionally Empty
}

void OpenGLInstanceManager::create()
{
  // Do nothing
}

void OpenGLInstanceManager::commit(const OpenGLViewport &view)
{
  P(OpenGLInstanceManagerPrivate);

  // Clear all drawing instances
  for (OpenGLInstanceGroupPair *pair : p.m_groups)
  {
    pair->group.clear();
  }

  // Aggregate all drawing instances
  for (OpenGLInstance *instance : p.m_instances)
  {

    // Ignore instances with no mesh set
    if (instance->mesh().empty()) continue;

    // Search for already added groups
    for (OpenGLInstanceGroupPair *pair : p.m_groups)
    {
      if (pair->meshFile == instance->mesh())
      {
        pair->group.addInstance(instance);
        goto NextInstance;
      }
    }

    // Add the new instance group
    {
      OpenGLInstanceGroupPair *pair = new OpenGLInstanceGroupPair;
      const OpenGLMesh &mesh = OpenGLMeshManager::mesh(instance->mesh());
      if (mesh.isCreated())
      {
        pair->meshFile = instance->mesh();
        pair->group.create();
        pair->group.setMesh(mesh);
        pair->group.addInstance(instance);
        p.m_groups.push_back(pair);
      }
      else
      {
        qFatal("Attempted to activate mesh which has not been created!");
      }
    }

    // Continue since the instance was already found
    NextInstance:
    continue;
  }

  // Make sure all meshes are up-to-date
  for (OpenGLInstanceGroupPair *pair : p.m_groups)
  {
    pair->group.setMesh(OpenGLMeshManager::mesh(pair->meshFile));
  }

  // Update all GPU data
  for (OpenGLInstanceGroupPair *pair : p.m_groups)
  {
    pair->group.commit(view);
  }
}

void OpenGLInstanceManager::render() const
{
  P(const OpenGLInstanceManagerPrivate);
  for (OpenGLInstanceGroupPair *pair : p.m_groups)
  {
    pair->group.draw();
  }
}

OpenGLInstance *OpenGLInstanceManager::createInstance()
{
  P(OpenGLInstanceManagerPrivate);
  OpenGLInstance *instance = new OpenGLInstance;
  p.m_instances.emplace_back(instance);
  return instance;
}

