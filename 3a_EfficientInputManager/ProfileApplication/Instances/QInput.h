#ifndef QINPUT_H
#define QINPUT_H

#include <Qt>
#include <QPoint>

class Input
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

  // State updating
  static void update();
  static void registerKeyPress(int k);
  static void registerKeyRelease(int k);
  static void registerMousePress(Qt::MouseButton btn);
  static void registerMouseRelease(Qt::MouseButton btn);
  static void reset();
  friend class Window;
};

inline bool Input::keyTriggered(Qt::Key k)
{
  return keyState(k) == InputTriggered;
}

inline bool Input::keyPressed(Qt::Key k)
{
  return keyState(k) == InputPressed;
}

inline bool Input::keyReleased(Qt::Key k)
{
  return keyState(k) == InputReleased;
}

inline bool Input::buttonTriggered(Qt::MouseButton k)
{
  return buttonState(k) == InputTriggered;
}

inline bool Input::buttonPressed(Qt::MouseButton k)
{
  return buttonState(k) == InputPressed;
}

inline bool Input::buttonReleased(Qt::MouseButton k)
{
  return buttonState(k) == InputReleased;
}

#endif // QINPUT_H
