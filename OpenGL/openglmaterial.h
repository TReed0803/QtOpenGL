#ifndef   OPENGLMATERIAL_H
#define   OPENGLMATERIAL_H OpenGLMaterial

class KVector3D;
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

  // Base Color
  void setBaseColor(float rgb);
  void setBaseColor(float r, float g, float b);
  void setBaseColor(KVector3D const &color);
  KVector3D const &baseColor() const;

  // Metallic
  void setMetallic(float m);
  float metallic() const;

  // Roughness
  void setRoughness(float r);
  float roughness() const;


private:
  KSharedPointer<OpenGLMaterialPrivate> m_private;
};

#endif // OPENGLMATERIAL_H
