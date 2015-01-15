#include <QPair>
#include <QMap>
#include "QKeyMap.h"

enum KeyState
{
  KeyInactive,
  KeyTriggered,
  KeyPressed,
  KeyReleased
};

typedef QMap<int, KeyState> KeyContainer;
static KeyContainer sg_keyInstances;

bool QKeyMap::keyTriggered(Qt::Key k)
{
  return sg_keyInstances[k] == KeyTriggered;
}

bool QKeyMap::keyPressed(Qt::Key k)
{
  return sg_keyInstances[k] == KeyPressed;
}

bool QKeyMap::keyReleased(Qt::Key k)
{
  return sg_keyInstances[k] == KeyReleased;
}

void QKeyMap::update()
{
  KeyContainer::iterator it = sg_keyInstances.begin();
  while (it != sg_keyInstances.end())
  {
    switch (*it)
    {
      case KeyTriggered:
        (*it) = KeyPressed;
        break;
      case KeyReleased:
        (*it) = KeyInactive;
        continue;
      default:
        break;
    }
    ++it;
  }
}

void QKeyMap::registerKeyPress(int k)
{
  sg_keyInstances[k] = KeyTriggered;
}

void QKeyMap::registerKeyRelease(int k)
{
  sg_keyInstances[k] = KeyReleased;
}

void QKeyMap::reset()
{
  sg_keyInstances.clear();
}
