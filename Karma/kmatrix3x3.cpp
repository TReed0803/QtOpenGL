#include "kmatrix3x3.h"

#include <KMatrix4x4>

KMatrix3x3::KMatrix3x3(const QMatrix4x4 &rhs)
{
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
      (*this)[i][j] = rhs(i, j);
}
