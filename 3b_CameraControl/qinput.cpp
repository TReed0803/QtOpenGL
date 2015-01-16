#include "QInput.h"
#include <QCursor>
#include <algorithm>
#include <vector>
#include <limits>

/*******************************************************************************
 * Static Helper Structs
 ******************************************************************************/
struct KeyInstance : std::pair<int, QInput::InputState>
{
  inline explicit KeyInstance(int k) : std::pair<int, QInput::InputState>(k, QInput::InputInvalid) {}
  inline KeyInstance(int k, QInput::InputState s) : std::pair<int, QInput::InputState>(k, s) {}
};
typedef std::vector<KeyInstance> KeyContainer;
static KeyContainer sg_keyInstances;

struct ButtonInstance : std::pair<Qt::MouseButton, QInput::InputState>
{
  inline explicit ButtonInstance(Qt::MouseButton btn) : std::pair<Qt::MouseButton, QInput::InputState>(btn, QInput::InputInvalid) {}
  inline ButtonInstance(Qt::MouseButton btn, QInput::InputState s) : std::pair<Qt::MouseButton, QInput::InputState>(btn, s) {}
};
typedef std::vector<ButtonInstance> ButtonContainer;
static ButtonContainer sg_buttonInstances;
static QPoint sg_mousePosition;
static QPoint sg_mouseCurrPosition;
static QPoint sg_mousePrevPosition;
static QPoint sg_mouseDelta;

/*******************************************************************************
 * Static Helper Fucntions
 ******************************************************************************/
static inline KeyContainer::iterator FindKey(Qt::Key k)
{
  KeyContainer::iterator it = sg_keyInstances.begin();
  KeyContainer::iterator end = sg_keyInstances.end();
  while (it != end)
  {
    if (it->first == k)
      return it;
    ++it;
  }
  return it;
}

static inline ButtonContainer::iterator FindButton(Qt::MouseButton btn)
{
  ButtonContainer::iterator it = sg_buttonInstances.begin();
  ButtonContainer::iterator end = sg_buttonInstances.end();
  while (it != end)
  {
    if (it->first == btn)
      return it;
    ++it;
  }
  return it;
}

/*******************************************************************************
 * QInput Implementation
 ******************************************************************************/
QInput::InputState QInput::keyState(Qt::Key k)
{
  KeyContainer::iterator it = FindKey(k);
  return (it != sg_keyInstances.end()) ? it->second : InputInvalid;
}

QInput::InputState QInput::buttonState(Qt::MouseButton k)
{
  ButtonContainer::iterator it = FindButton(k);
  return (it != sg_buttonInstances.end()) ? it->second : InputInvalid;
}

QPoint QInput::mousePosition()
{
  return sg_mousePosition;
}

QPoint QInput::mouseDelta()
{
  return sg_mouseDelta;
}

void QInput::update()
{
  sg_mousePrevPosition = sg_mouseCurrPosition;
  sg_mouseCurrPosition = sg_mousePosition;
  sg_mouseDelta = sg_mouseCurrPosition - sg_mousePrevPosition;
  for (KeyContainer::iterator it = sg_keyInstances.begin(); it != sg_keyInstances.end();)
  {
    switch (it->second)
    {
      case InputRegistered:
        it->second = InputTriggered;
        ++it;
        break;
      case InputTriggered:
        it->second = InputPressed;
        ++it;
        break;
      case InputUnregistered:
        it->second = InputReleased;
        ++it;
        break;
      case InputReleased:
        it = sg_keyInstances.erase(it);
        break;
      default:
        ++it;
        break;
    }
  }
  for (ButtonContainer::iterator it = sg_buttonInstances.begin(); it != sg_buttonInstances.end();)
  {
    switch (it->second)
    {
      case InputRegistered:
        it->second = InputTriggered;
        ++it;
        break;
      case InputTriggered:
        it->second = InputPressed;
        ++it;
        break;
      case InputUnregistered:
        it->second = InputReleased;
        ++it;
        break;
      case InputReleased:
        it = sg_buttonInstances.erase(it);
        break;
      default:
        ++it;
        break;
    }
  }
}

void QInput::registerKeyPress(int k)
{
  sg_keyInstances.push_back(KeyInstance(k, InputRegistered));
}

void QInput::registerKeyRelease(int k)
{
  KeyContainer::iterator it = FindKey((Qt::Key)k);
  if (it != sg_keyInstances.end())
  {
    it->second = InputUnregistered;
  }
}

void QInput::registerMousePress(Qt::MouseButton btn)
{
  sg_buttonInstances.push_back(ButtonInstance(btn, InputRegistered));
}

void QInput::registerMouseRelease(Qt::MouseButton btn)
{
  ButtonContainer::iterator it = FindButton(btn);
  if (it != sg_buttonInstances.end())
  {
    it->second = InputUnregistered;
  }
}

void QInput::registerMouseMovement(const QPoint &pos)
{
  sg_mousePosition = pos;
}

void QInput::reset()
{
  sg_keyInstances.clear();
  sg_buttonInstances.clear();
}
