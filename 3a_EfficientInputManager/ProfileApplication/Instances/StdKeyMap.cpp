#include <map>
#include <algorithm>
#include "StdKeyMap.h"

enum KeyState
{
  KeyInactive,
  KeyTriggered,
  KeyPressed,
  KeyReleased
};

typedef std::map<int, KeyState> KeyContainer;
static KeyContainer sg_keyInstances;

bool StdKeyMap::keyTriggered(Qt::Key k)
{
  return sg_keyInstances[k] == KeyTriggered;
}

bool StdKeyMap::keyPressed(Qt::Key k)
{
  return sg_keyInstances[k] == KeyPressed;
}

bool StdKeyMap::keyReleased(Qt::Key k)
{
  return sg_keyInstances[k] == KeyReleased;
}

void StdKeyMap::update()
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
        it->second = KeyInactive;
        continue;
      default:
        break;
    }
    ++it;
  }
}

void StdKeyMap::registerKeyPress(int k)
{
  sg_keyInstances[k] = KeyTriggered;
}

void StdKeyMap::registerKeyRelease(int k)
{
  sg_keyInstances[k] = KeyReleased;
}

void StdKeyMap::reset()
{
  sg_keyInstances.clear();
}
