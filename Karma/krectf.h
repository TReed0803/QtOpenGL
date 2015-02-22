#ifndef KRECTF_H
#define KRECTF_H KRectF

#include <QRectF>

class KRectF : public QRectF
{
public:
  Q_DECL_CONSTEXPR KRectF() : QRectF() {}
  Q_DECL_CONSTEXPR KRectF(const QRectF &rect) : QRectF(rect) {}
  Q_DECL_CONSTEXPR KRectF(const QPointF &topleft, const QSizeF &size) : QRectF(topleft, size) {}
  Q_DECL_CONSTEXPR KRectF(const QPointF &topleft, const QPointF &bottomRight) : QRectF(topleft, bottomRight) {}
  Q_DECL_CONSTEXPR KRectF(qreal left, qreal top, qreal width, qreal height) : QRectF(left, top, width, height) {}
  Q_DECL_CONSTEXPR KRectF(const QRect &rect) : QRectF(rect) {}
};

#endif // KRECTF_H
