#ifndef KEYCONVERTEDCUSTOM_H
#define KEYCONVERTEDCUSTOM_H

#include <Qt>

class KeyConvertedCustom
{
public:
  static bool keyTriggered(Qt::Key k);
  static bool keyPressed(Qt::Key k);
  static bool keyReleased(Qt::Key k);

  static void registerKeyPress(int k);
  static void registerKeyRelease(int k);
  static void update();
  static void reset();
};

#endif // KEYCONVERTEDCUSTOM_H
