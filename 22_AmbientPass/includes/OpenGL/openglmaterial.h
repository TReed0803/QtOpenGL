#ifndef   OPENGLMATERIAL_H
#define   OPENGLMATERIAL_H

class KColor;

class OpenGLMaterialPrivate;
class OpenGLMaterial
{
public:

  OpenGLMaterial();
  OpenGLMaterial(OpenGLMaterial const &rhs);
  ~OpenGLMaterial();

  void operator=(OpenGLMaterial const &rhs);

  // Diffuse
  void setDiffuse(char r, char g, char b);
  void setDiffuse(float r, float g, float b);
  void setDiffuse(KColor const &color);
  KColor const &diffuse() const;

  // Specular
  void setSpecular(char r, char g, char b, float exp);
  void setSpecular(float r, float g, float b, float exp);
  void setSpecular(KColor const &color, float exp);
  void setSpecularColor(char r, char g, char b);
  void setSpecularColor(float r, float g, float b);
  void setSpecularColor(KColor const &color);
  void setSpecularExponent(float exp);
  KColor const &specularColor() const;
  float specularExponent() const;

private:
  OpenGLMaterialPrivate *m_private;
};

#endif // OPENGLMATERIAL_H
