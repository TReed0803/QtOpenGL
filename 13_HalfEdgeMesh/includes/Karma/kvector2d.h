#ifndef KVECTOR2D_H
#define KVECTOR2D_H

#include <QVector2D>

class KVector2D : public QVector2D
{
public:
  Q_DECL_CONSTEXPR KVector2D() : QVector2D() {}
  Q_DECL_CONSTEXPR KVector2D(QVector2D const& rhs) : QVector2D(rhs) {}
  Q_DECL_CONSTEXPR KVector2D(float xpos, float ypos) : QVector2D(xpos, ypos) {}
};

#endif // KVECTOR2D_H

