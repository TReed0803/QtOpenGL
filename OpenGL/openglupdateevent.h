#ifndef OPENGLUPDATEEVENT_H
#define OPENGLUPDATEEVENT_H OpenGLUpdateEvent

#include <QEvent>

class OpenGLUpdateEvent : public QEvent
{
public:
  OpenGLUpdateEvent();
  // Static Access
  static QEvent::Type eventType();
};

inline OpenGLUpdateEvent::OpenGLUpdateEvent() : QEvent(OpenGLUpdateEvent::eventType()) {}

#endif // OPENGLUPDATEEVENT_H
