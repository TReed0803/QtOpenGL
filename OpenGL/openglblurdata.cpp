#include "openglblurdata.h"

#include <KMath>

OpenGLBlurData::OpenGLBlurData(int width, float spread) :
  m_blurWidth(width), m_blurWidth2(2 * width)
{
  float total = 0.0f;
  int maxValue = 2*width;
  for (int i = 0; i < width; ++i)
  {
    float scalar = std::pow(spread, float(width - i) / (width + 1));
    m_weights[i] = m_weights[maxValue - i] = scalar * Karma::normalDist(float(width - i));
    total += 2.0f * m_weights[i];
  }
  m_weights[width] = Karma::normalDist(0.0f);
  total += m_weights[width];
  for (int i = 0; i < maxValue + 1; ++i)
  {
    m_weights[i] /= total;
  }
}
