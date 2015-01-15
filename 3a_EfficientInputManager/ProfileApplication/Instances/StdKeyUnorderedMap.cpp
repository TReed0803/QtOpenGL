#include <unordered_map>
#include <algorithm>
#include "StdKeyUnorderedMap.h"

enum KeyState
{
  KeyInactive,
  KeyTriggered,
  KeyPressed,
  KeyReleased
};

typedef std::unordered_map<int, KeyState> KeyContainer;
static KeyContainer sg_keyInstances;

bool StdKeyUnorderedMap::keyTriggered(Qt::Key k)
{
  return sg_keyInstances[k] == KeyTriggered;
}

bool StdKeyUnorderedMap::keyPressed(Qt::Key k)
{
  return sg_keyInstances[k] == KeyPressed;
}

bool StdKeyUnorderedMap::keyReleased(Qt::Key k)
{
  return sg_keyInstances[k] == KeyReleased;
}

void StdKeyUnorderedMap::update()
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

void StdKeyUnorderedMap::registerKeyPress(int k)
{
  sg_keyInstances[k] = KeyTriggered;
}

void StdKeyUnorderedMap::registerKeyRelease(int k)
{
  sg_keyInstances[k] = KeyReleased;
}

void StdKeyUnorderedMap::reset()
{
  sg_keyInstances.clear();
}
