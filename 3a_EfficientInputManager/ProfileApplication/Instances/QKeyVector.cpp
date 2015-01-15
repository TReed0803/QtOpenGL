#include <QPair>
#include <QVector>
#include "QKeyVector.h"

enum KeyState
{
  KeyTriggered,
  KeyPressed,
  KeyReleased
};

typedef QPair<int, KeyState> KeyInstance;
typedef QVector<KeyInstance> KeyContainer;
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

bool QKeyVector::keyTriggered(Qt::Key k)
{
  KeyInstance *instance;
  if (FindInstance(k, instance))
  {
    return instance->second == KeyTriggered;
  }
  return false;
}

bool QKeyVector::keyPressed(Qt::Key k)
{
  KeyInstance *instance;
  if (FindInstance(k, instance))
  {
    return instance->second == KeyPressed;
  }
  return false;
}

bool QKeyVector::keyReleased(Qt::Key k)
{
  KeyInstance *instance;
  if (FindInstance(k, instance))
  {
    return instance->second == KeyReleased;
  }
  return false;
}

void QKeyVector::update()
{
  KeyContainer::iterator it = sg_keyInstances.begin();
  while (it != sg_keyInstances.end())
  {
    switch (it->second)
    {
      case KeyTriggered:
        it->second = KeyPressed;
        break;
      case KeyReleased:
        it = sg_keyInstances.erase(it);
        continue;
      default:
        break;
    }
    ++it;
  }
}

void QKeyVector::registerKeyPress(int k)
{
  KeyInstance *instance;
  if (!FindInstance((Qt::Key)k, instance))
  {
    sg_keyInstances.append(KeyInstance(k, KeyTriggered));
  }
}

void QKeyVector::registerKeyRelease(int k)
{
  KeyInstance *instance;
  if (FindInstance((Qt::Key)k, instance))
  {
    instance->second = KeyReleased;
  }
}

void QKeyVector::reset()
{
  sg_keyInstances.clear();
}
