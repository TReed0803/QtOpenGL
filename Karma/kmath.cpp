#include "kmath.h"

const float Karma::Pi       = 3.1415926535897932384626433832795028841971693993751058f;
const float Karma::PiHalf   = 1.5707963267948966192313216916397514420985846996875529f;
const float Karma::TwoPi    = 6.2831853071795864769252867665590057683943387987502116f;

void Karma::symSchur2(const KMatrix3x3 &symMtx, int p, int q, float *cosine, float *sine)
{
  if (std::abs(symMtx[p][q] > 0.0001f))
  {
    float r = (symMtx[q][q] - symMtx[p][p]) / (2.0f * symMtx[p][q]);
    float t;
    if (r >= 0.0f)
    {
      t = 1.0f / (r + std::sqrt(1.0f + r * r));
    }
    else
    {
      t = 1.0f / (r - std::sqrt(1.0f + r * r));
    }
    (*cosine) = 1.0f / std::sqrt(1.0f + t * t);
    (*sine) = t * (*cosine);
  }
  else
  {
    (*cosine) = 1.0f;
    (*sine) = 0.0f;
  }
}


KMatrix3x3 Karma::jacobi(KMatrix3x3 covar, int iterations)
{
  int p, q;
  float prevOff, c, s;
  KMatrix3x3 jacobiMtx;
  KMatrix3x3 eigen;
  eigen.setToIdentity();

  for (int n = 0; n < iterations; ++n)
  {

    // Largest off-diagonal element
    p = 0; q = 1;
    for (int i = 0; i < 3; ++i)
    {
      for (int j = 0; j < 3; ++j)
      {
        if (i == j) continue;
        if (std::abs(covar[i][j]) > std::abs(covar[p][q]))
        {
          p = i;
          q = j;
        }
      }
    }

    // Compute the jacobi rotation
    Karma::symSchur2(covar, p, q, &c, &s);
    jacobiMtx.setToIdentity();
    jacobiMtx[p][p] = jacobiMtx[q][q] = c;
    jacobiMtx[p][q] = s; jacobiMtx[q][p] = -s;

    // Cumulate rotations
    eigen *= jacobiMtx;
    covar = (jacobiMtx.transposed() * covar) * jacobiMtx;
    float off = 0.0f;
    for (int i = 0; i < 3; ++i)
    {
      for (int j = 0; j < 3; ++j)
      {
        if (i == j) continue;
        off += covar[i][j] * covar[i][j];
      }
    }

    // Stop when no longer decreasing
    if (n > 2 && off >= prevOff)
      return eigen;

    prevOff = off;
  }
  return eigen;
}



void Karma::extractColumnVectors(const KMatrix3x3 &eigenVecs, KVector3D axes[])
{
  axes[0] = KVector3D(eigenVecs[0][0], eigenVecs[1][0], eigenVecs[2][0]);
  axes[1] = KVector3D(eigenVecs[0][1], eigenVecs[1][1], eigenVecs[2][1]);
  axes[2] = KVector3D(eigenVecs[0][2], eigenVecs[1][2], eigenVecs[2][2]);
}


KVector3D Karma::minEigenExtents(const KMatrix3x3 &eigenVecs)
{
  int maxc = 0;
  float maxf, maxe = std::abs(eigenVecs[0][0]);
  maxf = std::abs(eigenVecs[1][1]);
  if (maxf < maxe) maxc = 1, maxe = maxf;
  maxf = std::abs(eigenVecs[2][2]);
  if (maxf < maxe) maxc = 2, maxe = maxf;
  return KVector3D(eigenVecs[0][maxc], eigenVecs[1][maxc], eigenVecs[2][maxc]);
}

KVector3D Karma::maxEigenExtents(const KMatrix3x3 &eigenVecs)
{
  int maxc = 0;
  float maxf, maxe = std::abs(eigenVecs[0][0]);
  maxf = std::abs(eigenVecs[1][1]);
  if (maxf > maxe) maxc = 1, maxe = maxf;
  maxf = std::abs(eigenVecs[2][2]);
  if (maxf > maxe) maxc = 2, maxe = maxf;
  return KVector3D(eigenVecs[0][maxc], eigenVecs[1][maxc], eigenVecs[2][maxc]);
}
