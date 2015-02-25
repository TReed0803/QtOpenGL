#ifndef KMATRIX3X3_H
#define KMATRIX3X3_H KMatrix3x3

#include <QMatrix3x3>

class KMatrix3x3 : public QMatrix3x3
{
public:
  inline KMatrix3x3() : QMatrix3x3() {}
  inline KMatrix3x3(QMatrix3x3 const &rhs) : QMatrix3x3(rhs) {}
  explicit KMatrix3x3(const float *values) : QMatrix3x3(values) {}

  template <int N, int M>
  explicit KMatrix3x3(const QGenericMatrix<N, M, float>& matrix) : QMatrix3x3(matrix) {}

  float* operator[](int i) { return data() + i * 3; }
  float const* operator[](int i) const { return constData() + i * 3; }
  void operator*=(KMatrix3x3 const &rhs) { (*this) = (*this) * rhs; }
};

#endif // KMATRIX3X3_H

