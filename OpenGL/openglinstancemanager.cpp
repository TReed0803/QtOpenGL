#include "openglinstancemanager.h"

#include <vector>
#include <KMacros>
#include <OpenGLMeshManager>
#include <OpenGLInstance>
#include <OpenGLMesh>
#include <string>
#include <algorithm>
#include <KFrustum>
#include <OpenGLViewport>
#include <OpenGLRenderBlock>
#include <OpenGLMaterial>

struct OpenGLInstancePartitionWithinView : public std::unary_function<bool, OpenGLInstance*>
{
  OpenGLInstancePartitionWithinView(const OpenGLViewport &view) :
    m_frustum(view.frustum())
  {
    // Intentionally Empty
  }
  inline bool operator()(OpenGLInstance *instance) const
  {
    return m_frustum.intersects(instance->aabb());
  }
private:
  KFrustum m_frustum;
};

struct OpenGLInstanceSortByMeshMaterial : public std::binary_function<bool, OpenGLInstance*, OpenGLInstance*>
{
  inline bool operator()(OpenGLInstance *lhs, OpenGLInstance *rhs) const
  {
    // Primary Sort Condition
    if (lhs->mesh().objectId() < rhs->mesh().objectId()) return true;
    if (lhs->mesh().objectId() > rhs->mesh().objectId()) return false;

    // Secondary Sort Condition
    if (lhs->material().objectId() < rhs->material().objectId()) return true;
    if (lhs->material().objectId() > rhs->material().objectId()) return false;

    // Completely equal
    return false;
  }
};

class OpenGLInstanceManagerPrivate
{
public:
  typedef std::vector<OpenGLInstance*> InstanceContainer;
  typedef InstanceContainer::iterator InstanceIterator;
  InstanceContainer m_instances;
  InstanceIterator m_begin, m_end;
  void commit(const OpenGLViewport &view);
  void render() const;
  void renderAll() const;
};

void OpenGLInstanceManagerPrivate::commit(const OpenGLViewport &view)
{
  m_begin = m_instances.begin();
  m_end = m_instances.end();
  //m_end = std::partition(m_begin, m_end, OpenGLInstancePartitionWithinView(view));
  std::sort(m_begin, m_end, OpenGLInstanceSortByMeshMaterial());

  InstanceIterator it = m_begin;
  while (it != m_end)
  {
    OpenGLInstance *instance = *it;
    instance->commit(view);
    instance->material().commit();
    ++it;
  }
}

void OpenGLInstanceManagerPrivate::render() const
{
  OpenGLInstance *instance;
  InstanceIterator begin = m_begin;
  int currMat  = 0;
  int currMesh = 0;
  while (begin != m_end)
  {
    instance = *begin;
    if (instance->visible())
    {
      if (currMesh != instance->mesh().objectId())
      {
        instance->mesh().bind();
        currMesh = instance->mesh().objectId();
      }
      if (currMat != instance->material().objectId())
      {
        instance->material().bind();
        currMat = instance->material().objectId();
      }
      instance->bind();
      instance->mesh().draw();
    }
    ++begin;
  }
}

void OpenGLInstanceManagerPrivate::renderAll() const
{
  int currMat  = 0;
  int currMesh = 0;
  for (OpenGLInstance *instance : m_instances)
  {
    if (instance->visible())
    {
      if (currMesh != instance->mesh().objectId())
      {
        instance->mesh().bind();
        currMesh = instance->mesh().objectId();
      }
      if (currMat != instance->material().objectId())
      {
        instance->material().bind();
        currMat = instance->material().objectId();
      }
      instance->bind();
      instance->mesh().draw();
    }
  }
}

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
  p.commit(view);
}

void OpenGLInstanceManager::render() const
{
  P(const OpenGLInstanceManagerPrivate);
  p.render();
}

void OpenGLInstanceManager::renderAll() const
{
  P(const OpenGLInstanceManagerPrivate);
  p.renderAll();
}

OpenGLInstance *OpenGLInstanceManager::createInstance()
{
  P(OpenGLInstanceManagerPrivate);
  OpenGLInstance *instance = new OpenGLInstance;
  p.m_instances.emplace_back(instance);
  return instance;
}

