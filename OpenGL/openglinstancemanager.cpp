#include "openglinstancemanager.h"

#include <vector>
#include <KMacros>
#include <OpenGLInstance>
#include <OpenGLInstanceGroup>
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

void OpenGLInstanceManager::update(OpenGLRenderBlock &current, OpenGLRenderBlock &previous)
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
      pair->meshFile = instance->mesh();
      pair->group.create();
      pair->group.setMesh(pair->meshFile);
      pair->group.addInstance(instance);
      p.m_groups.push_back(pair);
    }

    // Continue since the instance was already found
    NextInstance:
    continue;
  }

  // Update all GPU data
  for (OpenGLInstanceGroupPair *pair : p.m_groups)
  {
    pair->group.update(current, previous);
  }
}

void OpenGLInstanceManager::render()
{
  P(OpenGLInstanceManagerPrivate);
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

