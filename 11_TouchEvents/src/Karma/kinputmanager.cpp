#include "kinputmanager.h"
#include "kpangesture.h"
#include "kpinchgesture.h"

#include <algorithm>
#include <vector>

#include <QCursor>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QTouchEvent>

/*******************************************************************************
 * Static Helper Structs
 ******************************************************************************/
template <typename T>
struct InputInstance : std::pair<T, KInputManager::InputState>
{
  typedef std::pair<T, KInputManager::InputState> base_class;
  inline InputInstance(T value) : base_class(value, KInputManager::InputInvalid) {}
  inline InputInstance(T value, KInputManager::InputState state) : base_class(value, state) {}
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
static QList<QTouchEvent::TouchPoint> sg_currTouchPoints;
static QList<QTouchEvent::TouchPoint> sg_prevTouchPoints;
static KPoint sg_mouseCurrPosition;
static KPoint sg_mousePrevPosition;
static KPoint sg_mouseDelta;
static KPinchGesture sg_pinchGesture;
static KPanGesture sg_panGesture;

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
  case KInputManager::InputRegistered:
    instance.second = KInputManager::InputTriggered;
    break;
  case KInputManager::InputTriggered:
    instance.second = KInputManager::InputPressed;
    break;
  case KInputManager::InputUnregistered:
    instance.second = KInputManager::InputReleased;
    break;
  default:
    break;
  }
}

template <typename TPair>
static inline bool CheckReleased(const TPair &instance)
{
  return instance.second == KInputManager::InputReleased;
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
KInputManager::InputState KInputManager::keyState(Qt::Key key)
{
  KeyContainer::iterator it = FindKey(key);
  return (it != sg_keyInstances.end()) ? it->second : InputInvalid;
}

KInputManager::InputState KInputManager::buttonState(Qt::MouseButton button)
{
  ButtonContainer::iterator it = FindButton(button);
  return (it != sg_buttonInstances.end()) ? it->second : InputInvalid;
}

KPoint KInputManager::mousePosition()
{
  return QCursor::pos();
}

KPoint KInputManager::mouseDelta()
{
  return sg_mouseDelta;
}

int KInputManager::touchCount()
{
  return sg_prevTouchPoints.size();
}

KTouchPoint KInputManager::touchPoint(int idx)
{
  return sg_prevTouchPoints[idx];
}

bool KInputManager::pinchGesture(KPinchGesture *info)
{
  if (sg_pinchGesture.state() == Qt::GestureUpdated)
  {
    if (info)
      *info = sg_pinchGesture;
    return true;
  }
  return false;
}

bool KInputManager::panGesture(KPanGesture *info)
{
  if (sg_panGesture.state() == Qt::GestureUpdated)
  {
    if (info)
      *info = sg_panGesture;
    return true;
  }
  return false;
}

void KInputManager::update()
{
  // Update Mouse Delta
  sg_mousePrevPosition = sg_mouseCurrPosition;
  sg_mouseCurrPosition = QCursor::pos();
  sg_mouseDelta = sg_mouseCurrPosition - sg_mousePrevPosition;

  // Update KeyState values
  Update(sg_buttonInstances);
  Update(sg_keyInstances);

  // Update Touch Points
  sg_prevTouchPoints = sg_currTouchPoints;
  sg_currTouchPoints.clear();
}

void KInputManager::registerKeyPressEvent(QKeyEvent *event)
{
  if (event->isAutoRepeat()) return;
  KeyContainer::iterator it = FindKey((Qt::Key)event->key());
  if (it == sg_keyInstances.end())
  {
    sg_keyInstances.push_back(KeyInstance((Qt::Key)event->key(), InputRegistered));
  }
}

void KInputManager::registerKeyReleaseEvent(QKeyEvent *event)
{
  if (event->isAutoRepeat()) return;
  KeyContainer::iterator it = FindKey((Qt::Key)event->key());
  if (it != sg_keyInstances.end())
  {
    it->second = InputUnregistered;
  }
}

void KInputManager::registerMousePressEvent(QMouseEvent *event)
{
  ButtonContainer::iterator it = FindButton(event->button());
  if (it == sg_buttonInstances.end())
  {
    sg_buttonInstances.push_back(ButtonInstance(event->button(), InputRegistered));
  }
}

void KInputManager::registerMouseReleaseEvent(QMouseEvent *event)
{
  ButtonContainer::iterator it = FindButton(event->button());
  if (it != sg_buttonInstances.end())
  {
    it->second = InputUnregistered;
  }
}

void KInputManager::registerTouchEvent(QTouchEvent *event)
{
  sg_currTouchPoints = event->touchPoints();
}

static void UpdatePanGesture(QPanGesture *gesture)
{
  sg_panGesture = KPanGesture(*gesture);
}

static void UpdatePinchGesture(QPinchGesture *gesture)
{
  sg_pinchGesture = KPinchGesture(*gesture);
}

void KInputManager::registerGestureEvent(QGestureEvent *event)
{
  QGesture *gesture;
  if ((gesture = event->gesture(Qt::PanGesture)))
    UpdatePanGesture(static_cast<QPanGesture*>(gesture));
  if ((gesture = event->gesture(Qt::PinchGesture)))
    UpdatePinchGesture(static_cast<QPinchGesture*>(gesture));
}

void KInputManager::reset()
{
  sg_keyInstances.clear();
  sg_buttonInstances.clear();
}
