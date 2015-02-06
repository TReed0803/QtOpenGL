#ifndef KPANGESTURE_H
#define KPANGESTURE_H

#include <KGesture>
#include <QPanGesture>

class KPanGesture : public KGesture
{
public:
  KPanGesture();
  KPanGesture(QPanGesture const &rhs);
  inline KPointF lastOffset() const;
  inline KPointF offset() const;
  inline KPointF delta() const;
  inline qreal acceleration() const;
private:
  qreal m_acceleration;
  KPointF m_offset;
  KPointF m_lastOffset;
};

inline KPanGesture::KPanGesture() {}
inline KPanGesture::KPanGesture(QPanGesture const &rhs) : KGesture(rhs), m_acceleration(rhs.acceleration()), m_offset(rhs.offset()), m_lastOffset(rhs.lastOffset()) {}
inline KPointF KPanGesture::lastOffset() const { return m_lastOffset; }
inline KPointF KPanGesture::offset() const { return m_offset; }
inline KPointF KPanGesture::delta() const { return m_offset - m_lastOffset; }
inline qreal KPanGesture::acceleration() const { return m_acceleration; }

#endif // KPANGESTURE_H
