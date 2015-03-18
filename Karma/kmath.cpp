#include "kmath.h"

const float Karma::Pi       = 3.1415926535897932384626433832795028841971693993751058f;
const float Karma::PiHalf   = 1.5707963267948966192313216916397514420985846996875529f;
const float Karma::TwoPi    = 6.2831853071795864769252867665590057683943387987502116f;
const float Karma::Log2     = 0.6931471805599453094172321214581765680755001343602552f;
const float Karma::Sqrt2    = 1.4142135623730950488016887242096980785696718753769480f;
const float Karma::Sqrt2Pi  = 2.5066282746310005024157652848110452530069867406099383f;

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



void Karma::decomposeMatrixeByColumnVectors(const KMatrix3x3 &eigenVecs, KVector3D axes[])
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

std::vector<KVector3D> Karma::decomposeMatrixeByColumnVectors(const KMatrix3x3 &mtx)
{
  std::vector<KVector3D> columns;
  columns.push_back(KVector3D(mtx[0][0], mtx[1][0], mtx[2][0]));
  columns.push_back(KVector3D(mtx[0][1], mtx[1][1], mtx[2][1]));
  columns.push_back(KVector3D(mtx[0][2], mtx[1][2], mtx[2][2]));
  return std::move(columns);
}


void Karma::reconstructMatrixByColumnVectors(KMatrix3x3 *mtx, const KVector3D &a, const KVector3D &b, const KVector3D &c)
{
  (*mtx)[0][0] = a.x(); (*mtx)[1][0] = a.y(); (*mtx)[2][0] = a.z();
  (*mtx)[0][1] = b.x(); (*mtx)[1][1] = b.y(); (*mtx)[2][1] = b.z();
  (*mtx)[0][2] = c.x(); (*mtx)[1][2] = c.y(); (*mtx)[2][2] = c.z();
}


KColor Karma::colorShift(const KColor &orig, float amt)
{
  int passes = std::floor(amt);
  float delta = amt - passes;
  unsigned char deltaR = orig.red() * delta;
  unsigned char deltaG = orig.green() * delta;
  unsigned char deltaB = orig.blue() * delta;
  switch (passes % 3)
  {
  case 0:
    return
      KColor(
        orig.red()   - deltaR + deltaG,
        orig.green() - deltaG + deltaB,
        orig.blue()  - deltaB + deltaR,
        orig.alpha()
      );
  case 1:
    return
      KColor(
        orig.blue()  - deltaB + deltaR,
        orig.red()   - deltaR + deltaG,
        orig.green() - deltaG + deltaB,
        orig.alpha()
      );
  case 2:
    return
      KColor(
        orig.green() - deltaG + deltaB,
        orig.blue()  - deltaB + deltaR,
        orig.red()   - deltaR + deltaG,
        orig.alpha()
      );
  default:
    return orig;
  }
}


double Karma::sec(double x)
{
  return (2.0 * std::cos(x)) / (std::cos(2.0 * x) + 1.0);
}


float Karma::normalDist(float value)
{
  float valueSquared = value*value;
  return std::exp(-valueSquared / 2.0f) / Karma::Sqrt2Pi;
}
