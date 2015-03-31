#ifndef OPENGLHDRTEXTURE_H
#define OPENGLHDRTEXTURE_H OpenGLHdrTexture

class OpenGLTexture;
class OpenGLToneMappingFunction;
#include <KAbstractHdrParser>

class OpenGLHdrTextureLoaderPrivate;
class OpenGLHdrTextureLoader : public KAbstractHdrParser
{
public:
  OpenGLHdrTextureLoader(KAbstractReader *reader, OpenGLTexture *texture);
  bool parse(OpenGLToneMappingFunction *toneMap);
protected:
  virtual void onKeyValue(char const *key, char const *value);
  virtual void onResolution(PixelOrder xOrder, PixelOrder yOrder, int width, int height);
  virtual float *beginData();
  virtual void endData();
private:
  OpenGLHdrTextureLoaderPrivate *m_private;
};

#endif // OPENGLHDRTEXTURE_H
