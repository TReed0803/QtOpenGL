#include "openglblurdata.h"

#include <KMath>

OpenGLBlurData::OpenGLBlurData(int width, float deviation) :
  m_blurWidth(width)
{
  float total = 0.0f, current;

  // Make sure the BlurData is within the valid range.
  if (m_blurWidth < 1) m_blurWidth = 1;
  if (m_blurWidth > 32) m_blurWidth = 32;
  m_blurWidth2 = 2 * m_blurWidth;

  // Calculate the original normal distribution.
  for (int i = 0; i < m_blurWidth; ++i)
  {
    current = Karma::normalDist(float(m_blurWidth - i), 0.0f, deviation);
    m_weights[i] = m_weights[(m_blurWidth2 - i)] = current;
    total += 2.0f * current;
  }
  m_weights[m_blurWidth] = Karma::normalDist(0.0f, 0.0f, deviation);
  total += m_weights[m_blurWidth];

  // Normalize the values so that they sum to 1
  for (int i = 0; i <= m_blurWidth2; ++i)
  {
    m_weights[i] /= total;
  }
}
