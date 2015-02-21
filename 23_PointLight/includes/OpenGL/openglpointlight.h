#ifndef   OPENGLPOINTLIGHT_H
#define   OPENGLPOINTLIGHT_H

class KColor;
class KVector3D;

class OpenGLPointLightPrivate;
class OpenGLPointLight
{
public:
  OpenGLPointLight();
  ~OpenGLPointLight();

  // Light Positioning
  void translate(float x, float y, float z);
  void translate(KVector3D const &trans);
  void setTranslation(float x, float y, float z);
  void setTranslation(KVector3D const &pos);
  KVector3D const &translation() const;

  // Light Settings
  void setAttenuation(float c, float ex1, float ex2);
  void setAttenuation(KVector3D const &attn);
  KVector3D const &attenuation() const;
  void setAmbient(float r, float g, float b);
  void setAmbient(KColor const &rgb);
  KColor const &ambient() const;
  void setDiffuse(float r, float g, float b);
  void setDiffuse(KColor const &rgb);
  KColor const &diffuse() const;
  void setSpecular(float r, float g, float b);
  void setSpecular(KColor const &rgb);
  KColor const &specular();
  float radius() const;
  void setRadius(float r);

  // Stride Information
  static int Stride();
  static int PositionOffset();
  static int AttenuationOffset();
  static int AmbientOffset();
  static int DiffuseOffset();
  static int SpecularOffset();
  static int TransformOffset();

private:
  OpenGLPointLightPrivate *m_private;
};

inline int OpenGLPointLight::Stride()
{
  return sizeof(float) * (3 + 4 + 3 + 3 + 3 + 16);
}

inline int OpenGLPointLight::PositionOffset()
{
  return 0;
}

inline int OpenGLPointLight::AttenuationOffset()
{
  return sizeof(float) * 3;
}

inline int OpenGLPointLight::AmbientOffset()
{
  return sizeof(float) * (3 + 4);
}

inline int OpenGLPointLight::DiffuseOffset()
{
  return sizeof(float) * (3 + 4 + 3);
}

inline int OpenGLPointLight::SpecularOffset()
{
  return sizeof(float) * (3 + 4 + 3 + 3);
}

inline int OpenGLPointLight::TransformOffset()
{
  return sizeof(float) * (3 + 4 + 3 + 3 + 3);
}

#endif // OPENGLPOINTLIGHT_H
