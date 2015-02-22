#ifndef KPINCHGESTURE_H
#define KPINCHGESTURE_H KPinchGesture

#include <KGesture>
#include <QPinchGesture>

class KPinchGesture : public KGesture
{
public:
  KPinchGesture();
  KPinchGesture(QPinchGesture const &rhs);
  inline KPointF startCenterPoint() const;
  inline KPointF lastCenterPoint() const;
  inline KPointF centerPoint() const;
  inline qreal totalScaleFactor() const;
  inline qreal lastScaleFactor() const;
  inline qreal scaleFactor() const;
  inline qreal totalRotationAngle() const;
  inline qreal lastRotationAngle() const;
  inline qreal rotationAngle() const;
private:
  KPointF m_startCenterPoint;
  KPointF m_lastCenterPoint;
  KPointF m_centerPoint;
  qreal m_totalScaleFactor;
  qreal m_lastScaleFactor;
  qreal m_scaleFactor;
  qreal m_totalRotationAngle;
  qreal m_lastRotationAngle;
  qreal m_rotationAngle;
};

inline KPinchGesture::KPinchGesture() {}
inline KPinchGesture::KPinchGesture(QPinchGesture const &rhs) :
  KGesture(rhs),
  m_startCenterPoint(rhs.startCenterPoint()),
  m_lastCenterPoint(rhs.lastCenterPoint()),
  m_centerPoint(rhs.centerPoint()),
  m_totalScaleFactor(rhs.totalScaleFactor()),
  m_lastScaleFactor(rhs.lastScaleFactor()),
  m_scaleFactor(rhs.scaleFactor()),
  m_totalRotationAngle(rhs.totalRotationAngle()),
  m_lastRotationAngle(rhs.lastRotationAngle()),
  m_rotationAngle(rhs.rotationAngle()) {}
inline KPointF KPinchGesture::startCenterPoint() const { return m_startCenterPoint; }
inline KPointF KPinchGesture::lastCenterPoint() const { return m_lastCenterPoint; }
inline KPointF KPinchGesture::centerPoint() const { return m_centerPoint; }
inline qreal KPinchGesture::totalScaleFactor() const { return m_totalScaleFactor; }
inline qreal KPinchGesture::lastScaleFactor() const { return m_lastScaleFactor; }
inline qreal KPinchGesture::scaleFactor() const { return m_scaleFactor; }
inline qreal KPinchGesture::totalRotationAngle() const { return m_totalRotationAngle; }
inline qreal KPinchGesture::lastRotationAngle() const { return m_lastRotationAngle; }
inline qreal KPinchGesture::rotationAngle() const { return m_rotationAngle; }

#endif // KPINCHGESTURE_H
