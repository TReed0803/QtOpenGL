#include "KeyConverted.h"
#include "qkeyconverter.h"
#include <QDebug>

#define CURR_IDX 0
#define PREV_IDX 1
#define TOTAL_STATES 2
static bool sg_keyStates[TOTAL_STATES][QKeyConverter::KeyCount] = { { false } };

bool KeyConverted::keyTriggered(Qt::Key k)
{
  int idx = QKeyConverter::convertKey(k);
  return sg_keyStates[CURR_IDX][idx] && !sg_keyStates[PREV_IDX][idx];
}

bool KeyConverted::keyPressed(Qt::Key k)
{
  int idx = QKeyConverter::convertKey(k);
  return sg_keyStates[CURR_IDX][idx] && sg_keyStates[PREV_IDX][idx];
}

bool KeyConverted::keyReleased(Qt::Key k)
{
  int idx = QKeyConverter::convertKey(k);
  return !sg_keyStates[CURR_IDX][idx] && sg_keyStates[PREV_IDX][idx];
}

void KeyConverted::registerKeyPress(int k)
{
  sg_keyStates[CURR_IDX][QKeyConverter::convertKey((Qt::Key)k)] = true;
}

void KeyConverted::registerKeyRelease(int k)
{
  sg_keyStates[CURR_IDX][QKeyConverter::convertKey((Qt::Key)k)] = false;
}

void KeyConverted::update()
{
  memcpy(sg_keyStates[PREV_IDX], sg_keyStates[CURR_IDX], sizeof(bool) * QKeyConverter::KeyCount);
}

void KeyConverted::reset()
{
  memset(sg_keyStates, 0, sizeof(bool) * QKeyConverter::KeyCount * TOTAL_STATES);
}
