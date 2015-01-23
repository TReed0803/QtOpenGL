#include "qinput.h"
#include <QCursor>
#include <vector>
#include <algorithm>

/*******************************************************************************
 * Static Helper Structs
 ******************************************************************************/
template <typename T>
struct InputInstance : std::pair<T, QInput::InputState>
{
  typedef std::pair<T, QInput::InputState> base_class;
  inline InputInstance(T value) : base_class(value, QInput::InputInvalid) {}
  inline InputInstance(T value, QInput::InputState state) : base_class(value, state) {}
  inline bool operator==(const InputInstance &rhs) const
  {
    return this->first == rhs.first;
  }
};

// Instance types
typedef InputInstance<Qt::Key> KeyInstance;
typedef InputInstance<Qt::MouseButton> ButtonInstance;

// Container types
typedef std::vector<KeyInstance> KeyContainer;
typedef std::vector<ButtonInstance> ButtonContainer;

// Globals
static KeyContainer sg_keyInstances;
static ButtonContainer sg_buttonInstances;
static QPoint sg_mouseCurrPosition;
static QPoint sg_mousePrevPosition;
static QPoint sg_mouseDelta;

/*******************************************************************************
 * Static Helper Fucntions
 ******************************************************************************/
static inline KeyContainer::iterator FindKey(Qt::Key value)
{
  return std::find(sg_keyInstances.begin(), sg_keyInstances.end(), value);
}

static inline ButtonContainer::iterator FindButton(Qt::MouseButton value)
{
  return std::find(sg_buttonInstances.begin(), sg_buttonInstances.end(), value);
}

template <typename TPair>
static inline void UpdateStates(TPair &instance)
{
  switch (instance.second)
  {
  case QInput::InputRegistered:
    instance.second = QInput::InputTriggered;
    break;
  case QInput::InputTriggered:
    instance.second = QInput::InputPressed;
    break;
  case QInput::InputUnregistered:
    instance.second = QInput::InputReleased;
    break;
  default:
    break;
  }
}

template <typename TPair>
static inline bool CheckReleased(const TPair &instance)
{
  return instance.second == QInput::InputReleased;
}

template <typename Container>
static inline void Update(Container &container)
{
  typedef typename Container::iterator Iter;
  typedef typename Container::value_type TPair;

  // Remove old data
  Iter remove = std::remove_if(container.begin(), container.end(), &CheckReleased<TPair>);
  container.erase(remove, container.end());

  // Update existing data
  std::for_each(container.begin(), container.end(), &UpdateStates<TPair>);
}

/*******************************************************************************
 * QInput Implementation
 ******************************************************************************/
QInput::InputState QInput::keyState(Qt::Key key)
{
  KeyContainer::iterator it = FindKey(key);
  return (it != sg_keyInstances.end()) ? it->second : InputInvalid;
}

QInput::InputState QInput::buttonState(Qt::MouseButton button)
{
  ButtonContainer::iterator it = FindButton(button);
  return (it != sg_buttonInstances.end()) ? it->second : InputInvalid;
}

QPoint QInput::mousePosition()
{
  return QCursor::pos();
}

QPoint QInput::mouseDelta()
{
  return sg_mouseDelta;
}

void QInput::update()
{
  // Update Mouse Delta
  sg_mousePrevPosition = sg_mouseCurrPosition;
  sg_mouseCurrPosition = QCursor::pos();
  sg_mouseDelta = sg_mouseCurrPosition - sg_mousePrevPosition;

  // Update KeyState values
  Update(sg_buttonInstances);
  Update(sg_keyInstances);
}

void QInput::registerKeyPress(int key)
{
  KeyContainer::iterator it = FindKey((Qt::Key)key);
  if (it == sg_keyInstances.end())
  {
    sg_keyInstances.push_back(KeyInstance((Qt::Key)key, InputRegistered));
  }
}

void QInput::registerKeyRelease(int key)
{
  KeyContainer::iterator it = FindKey((Qt::Key)key);
  if (it != sg_keyInstances.end())
  {
    it->second = InputUnregistered;
  }
}

void QInput::registerMousePress(Qt::MouseButton button)
{
  ButtonContainer::iterator it = FindButton(button);
  if (it == sg_buttonInstances.end())
  {
    sg_buttonInstances.push_back(ButtonInstance(button, InputRegistered));
  }
}

void QInput::registerMouseRelease(Qt::MouseButton button)
{
  ButtonContainer::iterator it = FindButton(button);
  if (it != sg_buttonInstances.end())
  {
    it->second = InputUnregistered;
  }
}

void QInput::reset()
{
  sg_keyInstances.clear();
  sg_buttonInstances.clear();
}
