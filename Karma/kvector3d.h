#ifndef KVECTOR3D_H
#define KVECTOR3D_H KVector3D

#include <QVector3D>

class KVector3D : public QVector3D
{
public:
  Q_DECL_CONSTEXPR KVector3D() : QVector3D() {}
  Q_DECL_CONSTEXPR KVector3D(QVector3D const& rhs) : QVector3D(rhs) {}
  Q_DECL_CONSTEXPR KVector3D(float xpos, float ypos, float zpos) : QVector3D(xpos, ypos, zpos) {}
};

#endif // KVECTOR3D_H

