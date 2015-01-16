#include <list>
#include <algorithm>
#include "StdKeyList.h"

enum KeyState
{
  KeyTriggered,
  KeyPressed,
  KeyReleased
};

typedef std::pair<int, KeyState> KeyInstance;
typedef std::list<KeyInstance> KeyContainer;
static KeyContainer sg_keyInstances;

static bool FindInstance(Qt::Key k, KeyInstance*& _inst)
{
  for(KeyContainer::iterator it = sg_keyInstances.begin();
      it != sg_keyInstances.end(); ++it)
  {
    if (it->first == k)
    {
      _inst = &*it;
      return true;
    }
  }
  return false;
}

bool StdKeyList::keyTriggered(Qt::Key k)
{
  KeyInstance *instance;
  if (FindInstance(k, instance))
  {
    return instance->second == KeyTriggered;
  }
  return false;
}

bool StdKeyList::keyPressed(Qt::Key k)
{
  KeyInstance *instance;
  if (FindInstance(k, instance))
  {
    return instance->second == KeyPressed;
  }
  return false;
}

bool StdKeyList::keyReleased(Qt::Key k)
{
  KeyInstance *instance;
  if (FindInstance(k, instance))
  {
    return instance->second == KeyReleased;
  }
  return false;
}

void StdKeyList::update()
{
  for (KeyContainer::iterator it = sg_keyInstances.begin(); it != sg_keyInstances.end();)
  {
    switch (it->second)
    {
      case KeyTriggered:
        it->second = KeyPressed;
        ++it;
        break;
      case KeyReleased:
        it = sg_keyInstances.erase(it);
        break;
      default:
        ++it;
        break;
    }
  }
}

void StdKeyList::registerKeyPress(int k)
{
  KeyInstance *instance;
  if (!FindInstance((Qt::Key)k, instance))
  {
    sg_keyInstances.push_back(KeyInstance(k, KeyTriggered));
  }
}

void StdKeyList::registerKeyRelease(int k)
{
  KeyInstance *instance;
  if (FindInstance((Qt::Key)k, instance))
  {
    instance->second = KeyReleased;
  }
}

void StdKeyList::reset()
{
  sg_keyInstances.clear();
}
