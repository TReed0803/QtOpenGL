#ifndef KPOINT_H
#define KPOINT_H

#include <QPoint>

class KPoint : public QPoint
{
public:
  Q_DECL_CONSTEXPR KPoint() : QPoint() {}
  Q_DECL_CONSTEXPR KPoint(QPoint const &rhs) : QPoint(rhs) {}
  Q_DECL_CONSTEXPR KPoint(int xpos, int ypos) : QPoint(xpos, ypos) {}
};

#endif // KPOINT_H

