#ifndef KINPUTMANAGER_H
#define KINPUTMANAGER_H

#include <KPoint>
#include <Qt>

class KInputManager
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
  static KPoint mousePosition();
  static KPoint mouseDelta();

  // State updating
  static void update();
  static void registerKeyPress(int key);
  static void registerKeyRelease(int key);
  static void registerMousePress(Qt::MouseButton button);
  static void registerMouseRelease(Qt::MouseButton button);
  static void reset();
};

inline bool KInputManager::keyTriggered(Qt::Key key)
{
  return keyState(key) == InputTriggered;
}

inline bool KInputManager::keyPressed(Qt::Key key)
{
  return keyState(key) == InputPressed;
}

inline bool KInputManager::keyReleased(Qt::Key key)
{
  return keyState(key) == InputReleased;
}

inline bool KInputManager::buttonTriggered(Qt::MouseButton button)
{
  return buttonState(button) == InputTriggered;
}

inline bool KInputManager::buttonPressed(Qt::MouseButton button)
{
  return buttonState(button) == InputPressed;
}

inline bool KInputManager::buttonReleased(Qt::MouseButton button)
{
  return buttonState(button) == InputReleased;
}

#endif // KINPUTMANAGER_H
