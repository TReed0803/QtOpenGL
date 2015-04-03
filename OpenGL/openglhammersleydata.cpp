#include "openglhammersleydata.h"

#include <Qt>

OpenGLHammersleyData::OpenGLHammersleyData(int n) :
  N(float(n))
{
  float p = 0;
  float u = 0;
  int pos = 0;int kk;

  for (int k=0;  k<N;  k++) {
      for (p=0.5f, kk=k, u=0.0f;  kk;  p*=0.5f, kk >>= 1)
          if (kk & 1)
               u += p;
      float v = (k + 0.5) / N;
      data[pos].setX(u);
      data[pos].setY(v);
      data[pos].setZ(0.0f);
      data[pos].setW(0.0f);
      ++pos;
  }
  Q_ASSERT(pos == n);
}
