#ifndef KGESTURE_H
#define KGESTURE_H KGesture

#include <KPointF>
#include <QGesture>

class KGesture
{
public:
  KGesture();
  KGesture(QGesture const &rhs);
  inline Qt::GestureState state() const;
  inline KPointF hotSpot() const;
  inline bool hasHotSpot() const;
private:
  bool m_hasHotSpot;
  KPointF m_hotSpot;
  Qt::GestureState m_gestureState;
};

inline KGesture::KGesture() : m_gestureState(Qt::NoGesture) {}
inline KGesture::KGesture(QGesture const &rhs) : m_hasHotSpot(rhs.hasHotSpot()), m_hotSpot(rhs.hotSpot()), m_gestureState(rhs.state()) {}
inline Qt::GestureState KGesture::state() const { return m_gestureState; }
inline KPointF KGesture::hotSpot() const { return m_hotSpot; }
inline bool KGesture::hasHotSpot() const { return m_hasHotSpot; }

#endif // KGESTURE_H
