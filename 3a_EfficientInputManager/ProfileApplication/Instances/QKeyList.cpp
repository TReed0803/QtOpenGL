#include <QPair>
#include <QList>
#include "QKeyList.h"

enum KeyState
{
  KeyTriggered,
  KeyPressed,
  KeyReleased
};

typedef QPair<int, KeyState> KeyInstance;
typedef QList<KeyInstance> KeyContainer;
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

bool QKeyList::keyTriggered(Qt::Key k)
{
  KeyInstance *instance;
  if (FindInstance(k, instance))
  {
    return instance->second == KeyTriggered;
  }
  return false;
}

bool QKeyList::keyPressed(Qt::Key k)
{
  KeyInstance *instance;
  if (FindInstance(k, instance))
  {
    return instance->second == KeyPressed;
  }
  return false;
}

bool QKeyList::keyReleased(Qt::Key k)
{
  KeyInstance *instance;
  if (FindInstance(k, instance))
  {
    return instance->second == KeyReleased;
  }
  return false;
}

void QKeyList::update()
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

void QKeyList::registerKeyPress(int k)
{
  KeyInstance *instance;
  if (!FindInstance((Qt::Key)k, instance))
  {
    sg_keyInstances.append(KeyInstance(k, KeyTriggered));
  }
}

void QKeyList::registerKeyRelease(int k)
{
  KeyInstance *instance;
  if (FindInstance((Qt::Key)k, instance))
  {
    instance->second = KeyReleased;
  }
}

void QKeyList::reset()
{
  sg_keyInstances.clear();
}
