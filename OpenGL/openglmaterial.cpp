#include "openglmaterial.h"

#include <KColor>
#include <KMacros>

class OpenGLMaterialPrivate
{
public:
  KColor m_diffuse;
  KColor m_specularColor;
  float m_specularExponent;
};


OpenGLMaterial::OpenGLMaterial() :
  m_private(new OpenGLMaterialPrivate)
{
  // Intentionally Empty
}

OpenGLMaterial::OpenGLMaterial(const OpenGLMaterial &rhs) :
  m_private(new OpenGLMaterialPrivate)
{
  *m_private = *rhs.m_private;
}

OpenGLMaterial::~OpenGLMaterial()
{
  delete m_private;
}

void OpenGLMaterial::operator=(const OpenGLMaterial &rhs)
{
  *m_private = *rhs.m_private;
}

void OpenGLMaterial::setDiffuse(char r, char g, char b)
{
  P(OpenGLMaterialPrivate);
  p.m_diffuse.setRgb(r, g, b);
}

void OpenGLMaterial::setDiffuse(float r, float g, float b)
{
  P(OpenGLMaterialPrivate);
  p.m_diffuse.setRgbF(r, g, b);
}

void OpenGLMaterial::setDiffuse(const KColor &color)
{
  P(OpenGLMaterialPrivate);
  p.m_diffuse = color;
}

const KColor &OpenGLMaterial::diffuse() const
{
  P(OpenGLMaterialPrivate);
  return p.m_diffuse;
}

void OpenGLMaterial::setSpecular(char r, char g, char b, float exp)
{
  P(OpenGLMaterialPrivate);
  p.m_specularColor.setRgb(r, g, b);
  p.m_specularExponent = exp;
}

void OpenGLMaterial::setSpecular(float r, float g, float b, float exp)
{
  P(OpenGLMaterialPrivate);
  p.m_specularColor.setRgbF(r, g, b);
  p.m_specularExponent = exp;
}

void OpenGLMaterial::setSpecular(const KColor &color, float exp)
{
  P(OpenGLMaterialPrivate);
  p.m_specularColor = color;
  p.m_specularExponent = exp;
}

void OpenGLMaterial::setSpecularColor(char r, char g, char b)
{
  P(OpenGLMaterialPrivate);
  p.m_specularColor.setRgb(r, g, b);
}

void OpenGLMaterial::setSpecularColor(float r, float g, float b)
{
  P(OpenGLMaterialPrivate);
  p.m_specularColor.setRgbF(r, g, b);
}

void OpenGLMaterial::setSpecularColor(const KColor &color)
{
  P(OpenGLMaterialPrivate);
  p.m_specularColor = color;
}

void OpenGLMaterial::setSpecularExponent(float exp)
{
  P(OpenGLMaterialPrivate);
  p.m_specularExponent = exp;
}

const KColor &OpenGLMaterial::specularColor() const
{
  P(OpenGLMaterialPrivate);
  return p.m_specularColor;
}

float OpenGLMaterial::specularExponent() const
{
  P(OpenGLMaterialPrivate);
  return p.m_specularExponent;
}

