#ifndef QINPUT_H
#define QINPUT_H

#include <Qt>

class QInput
{
public:

  // Possible key states
  enum KeyState
  {
    KeyInvalid,
    KeyTriggered,
    KeyPressed,
    KeyReleased
  };

  // State checking
  static KeyState keyState(Qt::Key k);
  static bool keyTriggered(Qt::Key k);
  static bool keyPressed(Qt::Key k);
  static bool keyReleased(Qt::Key k);

  // State updating
  static void update();
  static void registerKeyPress(int k);
  static void registerKeyRelease(int k);
  static void reset();
  friend class Window;
};

inline bool QInput::keyTriggered(Qt::Key k)
{
  return keyState(k) == KeyTriggered;
}

inline bool QInput::keyPressed(Qt::Key k)
{
  return keyState(k) == KeyPressed;
}

inline bool QInput::keyReleased(Qt::Key k)
{
  return keyState(k) == KeyReleased;
}

#endif // QINPUT_H
