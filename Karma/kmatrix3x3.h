#ifndef KMATRIX3X3_H
#define KMATRIX3X3_H KMatrix3x3

#include <QMatrix3x3>
#include <KVector3D>

class KMatrix3x3 : public QMatrix3x3
{
public:
  inline KMatrix3x3() : QMatrix3x3() {}
  inline KMatrix3x3(QMatrix3x3 const &rhs) : QMatrix3x3(rhs) {}
  explicit KMatrix3x3(const float *values) : QMatrix3x3(values) {}
  explicit KMatrix3x3(QMatrix4x4 const &rhs);

  template <int N, int M>
  explicit KMatrix3x3(const QGenericMatrix<N, M, float>& matrix) : QMatrix3x3(matrix) {}

  float* operator[](int i) { return data() + i * 3; }
  float const* operator[](int i) const { return constData() + i * 3; }
  void operator*=(KMatrix3x3 const &rhs) { (*this) = (*this) * rhs; }
  KVector3D operator*(KVector3D const &rhs) const
  {
    return
      KVector3D(
        (*this)[0][0] * rhs.x() + (*this)[0][1] * rhs.y() + (*this)[0][2] * rhs.z(),
        (*this)[1][0] * rhs.x() + (*this)[1][1] * rhs.y() + (*this)[1][2] * rhs.z(),
        (*this)[2][0] * rhs.x() + (*this)[2][1] * rhs.y() + (*this)[2][2] * rhs.z()
      );
  }
};

#endif // KMATRIX3X3_H

