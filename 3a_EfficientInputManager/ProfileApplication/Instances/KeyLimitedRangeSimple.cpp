#include "KeyLimitedRangeSimple.h"
#include <QDebug>

enum KeyState
{
  KeyInactive,
  KeyTriggered,
  KeyPressed,
  KeyReleased
};

#define MAX_RANGE 255
static KeyState sg_keyStates[MAX_RANGE] = { KeyInactive };

bool KeyLimitedRangeSimple::keyTriggered(Qt::Key k)
{
  if (k >= MAX_RANGE) return false;
  return sg_keyStates[k] == KeyTriggered;
}

bool KeyLimitedRangeSimple::keyPressed(Qt::Key k)
{
  if (k >= MAX_RANGE) return false;
  return sg_keyStates[k] == KeyPressed;
}

bool KeyLimitedRangeSimple::keyReleased(Qt::Key k)
{
  if (k >= MAX_RANGE) return false;
  return sg_keyStates[k] == KeyReleased;
}

void KeyLimitedRangeSimple::registerKeyPress(int k)
{
  if (k >= MAX_RANGE) return;
  sg_keyStates[k] = KeyTriggered;
}

void KeyLimitedRangeSimple::registerKeyRelease(int k)
{
  if (k >= MAX_RANGE) return;
  sg_keyStates[k] = KeyReleased;
}

void KeyLimitedRangeSimple::update()
{
  for (int i = 0; i < MAX_RANGE; ++i)
  {
    switch (sg_keyStates[i])
    {
      case KeyTriggered:
        sg_keyStates[i] = KeyPressed;
        break;
      case KeyReleased:
        sg_keyStates[i] = KeyInactive;
        break;
      default:
        break;
    }
  }
}

void KeyLimitedRangeSimple::reset()
{
  memset(sg_keyStates, KeyInactive, sizeof(bool) * MAX_RANGE);
}
