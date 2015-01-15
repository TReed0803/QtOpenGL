#include "KeyLimitedRange.h"
#include <QDebug>

#define CURR_IDX 0
#define PREV_IDX 1
#define TOTAL_STATES 2
#define MAX_RANGE 255
bool sg_keyStates[TOTAL_STATES][MAX_RANGE] = { { false } };

bool KeyLimitedRange::keyTriggered(Qt::Key k)
{
  if (k >= MAX_RANGE) return false;
  return sg_keyStates[CURR_IDX][k] && !sg_keyStates[PREV_IDX][k];
}

bool KeyLimitedRange::keyPressed(Qt::Key k)
{
  if (k >= MAX_RANGE) return false;
  return sg_keyStates[CURR_IDX][k] && sg_keyStates[PREV_IDX][k];
}

bool KeyLimitedRange::keyReleased(Qt::Key k)
{
  if (k >= MAX_RANGE) return false;
  return !sg_keyStates[CURR_IDX][k] && sg_keyStates[PREV_IDX][k];
}

void KeyLimitedRange::registerKeyPress(int k)
{
  if (k >= MAX_RANGE) return;
  sg_keyStates[CURR_IDX][(Qt::Key)k] = true;
}

void KeyLimitedRange::registerKeyRelease(int k)
{
  if (k >= MAX_RANGE) return;
  sg_keyStates[CURR_IDX][(Qt::Key)k] = false;
}

void KeyLimitedRange::update()
{
  memcpy(sg_keyStates[PREV_IDX], sg_keyStates[CURR_IDX], sizeof(bool) * MAX_RANGE);
}

void KeyLimitedRange::reset()
{
  memset(sg_keyStates, 0, sizeof(bool) * MAX_RANGE * TOTAL_STATES);
}
