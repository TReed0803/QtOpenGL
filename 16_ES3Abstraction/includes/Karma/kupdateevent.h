#ifndef KUPDATEEVENT_H
#define KUPDATEEVENT_H

#include <QEvent>

class KUpdateEvent : public QEvent
{
public:
  KUpdateEvent();
  // Static Access
  static QEvent::Type type();
};

inline KUpdateEvent::KUpdateEvent() : QEvent(KUpdateEvent::type()) {}

#endif // KUPDATEEVENT_H
