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
  static InputState keyState(Qt::Key key);
  static bool keyTriggered(Qt::Key key);
  static bool keyPressed(Qt::Key key);
  static bool keyReleased(Qt::Key key);
  static InputState buttonState(Qt::MouseButton button);
  static bool buttonTriggered(Qt::MouseButton button);
  static bool buttonPressed(Qt::MouseButton button);
  static bool buttonReleased(Qt::MouseButton button);
  static QPoint mousePosition();
  static QPoint mouseDelta();

private:

  // State updating
  static void update();
  static void registerKeyPress(int key);
  static void registerKeyRelease(int key);
  static void registerMousePress(Qt::MouseButton button);
  static void registerMouseRelease(Qt::MouseButton button);
  static void reset();
  friend class Window;
};

inline bool QInput::keyTriggered(Qt::Key key)
{
  return keyState(key) == InputTriggered;
}

inline bool QInput::keyPressed(Qt::Key key)
{
  return keyState(key) == InputPressed;
}

inline bool QInput::keyReleased(Qt::Key key)
{
  return keyState(key) == InputReleased;
}

inline bool QInput::buttonTriggered(Qt::MouseButton button)
{
  return buttonState(button) == InputTriggered;
}

inline bool QInput::buttonPressed(Qt::MouseButton button)
{
  return buttonState(button) == InputPressed;
}

inline bool QInput::buttonReleased(Qt::MouseButton button)
{
  return buttonState(button) == InputReleased;
}

#endif // QINPUT_H
