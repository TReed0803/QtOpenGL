#ifndef OPENGLRECTANGLELIGHT_H
#define OPENGLRECTANGLELIGHT_H OpenGLRectangleLight

#include <OpenGLAreaLight>

class OpenGLRectangleLight : public OpenGLAreaLight
{
public:
  OpenGLRectangleLight();

  void setWidth(float w);
  void setHalfWidth(float hw);
  void setHeight(float h);
  void setHalfHeight(float hh);
  void setDimensions(float w, float h);
  void setHalfDimensions(float hw, float hh);
  float width() const;
  float halfWidth() const;
  float height() const;
  float halfHeight() const;

private:
  float m_halfWidth;
  float m_halfHeight;
};

#endif // OPENGLRECTANGLELIGHT_H
