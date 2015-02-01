#ifndef KMATRIX4X4_H
#define KMATRIX4X4_H

#include <QMatrix4x4>

class KMatrix4x4 : public QMatrix4x4
{
public:
  inline KMatrix4x4() : QMatrix4x4() {}
  explicit KMatrix4x4(const float *values) : QMatrix4x4(values) {}
  inline KMatrix4x4(float m11, float m12, float m13, float m14,
                    float m21, float m22, float m23, float m24,
                    float m31, float m32, float m33, float m34,
                    float m41, float m42, float m43, float m44) :
         QMatrix4x4(m11, m12, m13, m14,
                    m21, m22, m23, m24,
                    m31, m32, m33, m34,
                    m41, m42, m43, m44) {}

  template <int N, int M>
  explicit KMatrix4x4(const QGenericMatrix<N, M, float>& matrix) : QMatrix4x4(matrix) {}

  KMatrix4x4(const float *values, int cols, int rows) : QMatrix4x4(values, cols, rows) {}
  KMatrix4x4(const QTransform& transform) : QMatrix4x4(transform) {}
  KMatrix4x4(const QMatrix& matrix) : QMatrix4x4(matrix) {}
};

#endif // KMATRIX4X4_H
