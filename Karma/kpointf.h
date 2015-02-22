#ifndef KPOINTF_H
#define KPOINTF_H KPointF

#include <QPointF>

class KPointF : public QPointF
{
public:
  Q_DECL_CONSTEXPR KPointF() : QPointF() {}
  Q_DECL_CONSTEXPR KPointF(const QPoint &p) : QPointF(p) {}
  Q_DECL_CONSTEXPR KPointF(const QPointF &p) : QPointF(p) {}
  Q_DECL_CONSTEXPR KPointF(qreal xpos, qreal ypos) : QPointF(xpos, ypos) {}
};

#endif // KPOINTF_H
