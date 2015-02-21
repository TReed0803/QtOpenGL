#ifndef KTOUCHPOINT_H
#define KTOUCHPOINT_H

#include <QTouchEvent>

class KTouchPoint : public QTouchEvent::TouchPoint
{
public:
  explicit KTouchPoint(int id = -1) : QTouchEvent::TouchPoint(id) {}
  KTouchPoint(const QTouchEvent::TouchPoint &other) : QTouchEvent::TouchPoint(other) {}
};

#endif // KTOUCHPOINT_H
