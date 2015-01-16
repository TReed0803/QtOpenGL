#ifndef QINPUT_H
#define QINPUT_H

#include <Qt>
#include <QPoint>

class QInput
{
public:

  // Possible key states
  enum InputState
  {
    InputInvalid,
    InputRegistered,
    InputUnregistered,
    InputTriggered,
    InputPressed,
    InputReleased
  };

  // State checking
  static InputState keyState(Qt::Key k);
  static bool keyTriggered(Qt::Key k);
  static bool keyPressed(Qt::Key k);
  static bool keyReleased(Qt::Key k);
  static InputState buttonState(Qt::MouseButton k);
  static bool buttonTriggered(Qt::MouseButton k);
  static bool buttonPressed(Qt::MouseButton k);
  static bool buttonReleased(Qt::MouseButton k);
  static QPoint mousePosition();
  static QPoint mouseDelta();

private:

  // State updating
  static void update();
  static void registerKeyPress(int k);
  static void registerKeyRelease(int k);
  static void registerMousePress(Qt::MouseButton btn);
  static void registerMouseRelease(Qt::MouseButton btn);
  static void reset();
  friend class Window;
};

inline bool QInput::keyTriggered(Qt::Key k)
{
  return keyState(k) == InputTriggered;
}

inline bool QInput::keyPressed(Qt::Key k)
{
  return keyState(k) == InputPressed;
}

inline bool QInput::keyReleased(Qt::Key k)
{
  return keyState(k) == InputReleased;
}

inline bool QInput::buttonTriggered(Qt::MouseButton k)
{
  return buttonState(k) == InputTriggered;
}

inline bool QInput::buttonPressed(Qt::MouseButton k)
{
  return buttonState(k) == InputPressed;
}

inline bool QInput::buttonReleased(Qt::MouseButton k)
{
  return buttonState(k) == InputReleased;
}

#endif // QINPUT_H
