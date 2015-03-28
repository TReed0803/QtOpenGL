#ifndef   OPENGLMATERIAL_H
#define   OPENGLMATERIAL_H OpenGLMaterial

class KColor;
class KVector2D;
#include <KSharedPointer>

class OpenGLMaterialPrivate;
class OpenGLMaterial
{
public:

  OpenGLMaterial();
  ~OpenGLMaterial();

  // OpenGL
  void create();
  void bind();
  void commit();
  void release();
  int objectId() const;

  // Diffuse
  void setDiffuse(char r, char g, char b);
  void setDiffuse(float r, float g, float b);
  void setDiffuse(KColor const &color);
  KColor const &diffuse() const;

  // Fresnel
  void setFresnel(float rgb);
  void setFresnel(char r, char g, char b);
  void setFresnel(float r, float g, float b);
  void setFresnel(KColor const &color);
  KColor const &color() const;

  // Roughness
  void setRoughness(float xy);
  void setRoughness(float x, float y);
  KVector2D const &roughness() const;

private:
  KSharedPointer<OpenGLMaterialPrivate> m_private;
};

#endif // OPENGLMATERIAL_H
