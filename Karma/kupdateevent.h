#ifndef KUPDATEEVENT_H
#define KUPDATEEVENT_H KUpdateEvent

#include <QEvent>

class KUpdateEvent : public QEvent
{
public:
  KUpdateEvent();
  // Static Access
  static QEvent::Type type();
};

inline QEvent::Type KUpdateEvent::type()
{
  static QEvent::Type customEventType =
    static_cast<QEvent::Type>(QEvent::registerEventType());
  return customEventType;
}

inline KUpdateEvent::KUpdateEvent() : QEvent(KUpdateEvent::type()) {}

#endif // KUPDATEEVENT_H
