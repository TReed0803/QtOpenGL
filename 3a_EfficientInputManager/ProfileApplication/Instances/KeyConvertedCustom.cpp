#include "KeyConvertedCustom.h"
#include "../qkeyconvertercustom.h"
#include <QDebug>

#define CURR_IDX 0
#define PREV_IDX 1
#define TOTAL_STATES 2
static bool sg_keyStates[TOTAL_STATES][QKeyConverterCustom::KeyCount] = { { false } };

bool KeyConvertedCustom::keyTriggered(Qt::Key k)
{
  int idx = QKeyConverterCustom::convertKey(k);
  return sg_keyStates[CURR_IDX][idx] && !sg_keyStates[PREV_IDX][idx];
}

bool KeyConvertedCustom::keyPressed(Qt::Key k)
{
  int idx = QKeyConverterCustom::convertKey(k);
  return sg_keyStates[CURR_IDX][idx] && sg_keyStates[PREV_IDX][idx];
}

bool KeyConvertedCustom::keyReleased(Qt::Key k)
{
  int idx = QKeyConverterCustom::convertKey(k);
  return !sg_keyStates[CURR_IDX][idx] && sg_keyStates[PREV_IDX][idx];
}

void KeyConvertedCustom::registerKeyPress(int k)
{
  sg_keyStates[CURR_IDX][QKeyConverterCustom::convertKey((Qt::Key)k)] = true;
}

void KeyConvertedCustom::registerKeyRelease(int k)
{
  sg_keyStates[CURR_IDX][QKeyConverterCustom::convertKey((Qt::Key)k)] = false;
}

void KeyConvertedCustom::update()
{
  memcpy(sg_keyStates[PREV_IDX], sg_keyStates[CURR_IDX], sizeof(bool) * QKeyConverterCustom::KeyCount);
}

void KeyConvertedCustom::reset()
{
  memset(sg_keyStates, 0, sizeof(bool) * QKeyConverterCustom::KeyCount * TOTAL_STATES);
}
