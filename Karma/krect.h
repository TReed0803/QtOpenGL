#ifndef KRECT_H
#define KRECT_H KRect

#include <QRect>

class KRect : public QRect
{
public:
  Q_DECL_CONSTEXPR KRect() : QRect() {}
  Q_DECL_CONSTEXPR KRect(const QPoint &topleft, const QPoint &bottomright) : QRect(topleft, bottomright) {}
  Q_DECL_CONSTEXPR KRect(const QPoint &topleft, const QSize &size) : QRect(topleft, size) {}
  Q_DECL_CONSTEXPR KRect(int left, int top, int width, int height) : QRect(left, top, width, height) {}
};

#endif // KRECT_H

