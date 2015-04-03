#ifndef OPENGLHAMMERSLEYDATA_H
#define OPENGLHAMMERSLEYDATA_H OpenGLHammersleyData

#include <KVector4D>

class OpenGLHammersleyData
{
public:
  OpenGLHammersleyData(int n);
private:
  // Note: vec4 because each element is padded
  KVector4D data[60];
  float N;
};

#endif // OPENGLHAMMERSLEYDATA_H
