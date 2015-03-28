#ifndef KVECTOR4D_H
#define KVECTOR4D_H KVector4D

#include <QVector4D>

class KVector4D : public QVector4D
{
public:
  KVector4D() : QVector4D() {}
  KVector4D(QVector4D const &rhs) : QVector4D(rhs) {}
  operator QVector4D() const;
  void set(float x, float y, float z, float w);
};

inline void KVector4D::set(float x, float y, float z, float w)
{
  setX(x);
  setY(y);
  setZ(z);
  setW(w);
}

inline KVector4D::operator QVector4D() const
{
  return static_cast<QVector4D const>(*this);
}

#endif // KVECTOR4D_H
