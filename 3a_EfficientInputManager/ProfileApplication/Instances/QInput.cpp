#include "QInput.h"
#include <algorithm>
#include <vector>
#include <limits>

/*******************************************************************************
 * Static Helper Structs
 ******************************************************************************/
struct KeyInstance : std::pair<int, QInput::KeyState>
{
  inline explicit KeyInstance(int k) : std::pair<int, QInput::KeyState>(k, QInput::KeyInvalid) {}
  inline KeyInstance(int k, QInput::KeyState s) : std::pair<int, QInput::KeyState>(k, s) {}
  inline bool operator<(const KeyInstance &rhs) const
  {
    return first < rhs.first;
  }
  inline bool operator==(const KeyInstance &rhs) const
  {
    return first == rhs.first;
  }
};
typedef std::vector<KeyInstance> KeyContainer;
static const KeyInstance InvalidInstance = KeyInstance(std::numeric_limits<int>::max(), QInput::KeyInvalid);
static KeyContainer sg_keyInstances;

/*******************************************************************************
 * Static Helper Fucntions
 ******************************************************************************/
static inline KeyContainer::iterator FindKey(Qt::Key k)
{
  KeyContainer::iterator it = sg_keyInstances.begin();
  KeyContainer::iterator end = sg_keyInstances.end();
  while (it != end)
  {
    if (it->first == k)
      return it;
    ++it;
  }
  return it;
}

/*******************************************************************************
 * QInput Implementation
 ******************************************************************************/
QInput::KeyState QInput::keyState(Qt::Key k)
{
  KeyContainer::iterator it = FindKey(k);
  return (it != sg_keyInstances.end()) ? it->second : KeyInvalid;
}

void QInput::update()
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

void QInput::registerKeyPress(int k)
{
  sg_keyInstances.push_back(KeyInstance(k, KeyTriggered));
}

void QInput::registerKeyRelease(int k)
{
  KeyContainer::iterator it = FindKey((Qt::Key)k);
  if (it != sg_keyInstances.end())
  {
    it->second = KeyReleased;
  }
}

void QInput::reset()
{
  sg_keyInstances.clear();
}
