#include "opengltonemappingfunction.h"

OpenGLStandardToneMapping::OpenGLStandardToneMapping(float exposure, float contrast) :
  m_exposure(exposure), m_contrast(contrast)
{
  // Intentionally Empty
}

RgbF OpenGLStandardToneMapping::operator()(RgbF input) const
{
  RgbF eC = m_exposure * input;
  return std::pow(eC / (eC + 1.0f), m_contrast / 2.2);
}



RgbF OpenGLDefaultToneMapping::operator()(RgbF input) const
{
  return input;
}
