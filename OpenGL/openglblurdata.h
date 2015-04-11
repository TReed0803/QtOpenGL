#ifndef OPENGLBLURDATA_H
#define OPENGLBLURDATA_H OpenGLBlurData

class OpenGLBlurData
{
public:
  OpenGLBlurData(int width, float deviation = 1.0f);
//private:
  int m_blurWidth;
  int m_blurWidth2;
  float m_weights[65];
};

#endif // OPENGLBLURDATA_H

