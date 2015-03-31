#ifndef OPENGLENVIRONMENT_H
#define OPENGLENVIRONMENT_H OpenGLEnvironment

class OpenGLTexture;
#include <OpenGLToneMappingFunction>

class OpenGLEnvrionmentPrivate;
class OpenGLEnvironment
{
public:
  OpenGLEnvironment();
  ~OpenGLEnvironment();
  void create();
  void setDirect(char const *filePath);
  void setIndirect(char const *filePath);
  void setToneMappingFunction(OpenGLToneMappingFunction *fnc);
  OpenGLTexture &direct();
  OpenGLTexture &indirect();
private:
  OpenGLEnvrionmentPrivate *m_private;
};

#endif // OPENGLENVIRONMENT_H
