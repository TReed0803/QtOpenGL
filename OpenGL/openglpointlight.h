#ifndef   OPENGLPOINTLIGHT_H
#define   OPENGLPOINTLIGHT_H OpenGLPointLight

class KColor;
class KMatrix4x4;
class KVector3D;

class OpenGLPointLightPrivate;
class OpenGLPointLight
{
public:
  OpenGLPointLight();
  ~OpenGLPointLight();

  // Static Scalar
  static float CalculateScalar(int segments, int rings);

  // Light Positioning
  void translate(float x, float y, float z);
  void translate(KVector3D const &trans);
  void setTranslation(float x, float y, float z);
  void setTranslation(KVector3D const &pos);
  KVector3D const &translation() const;
  const KMatrix4x4 &toMatrix();

  // Light Settings
  void setAttenuation(float c, float ex1, float ex2);
  void setAttenuation(KVector3D const &attn);
  KVector3D const &attenuation() const;
  void setDiffuse(float r, float g, float b);
  void setDiffuse(KColor const &rgb);
  KColor const &diffuse() const;
  void setSpecular(float r, float g, float b);
  void setSpecular(KColor const &rgb);
  KColor const &specular();
  float radius() const;
  void setRadius(float r);

private:
  OpenGLPointLightPrivate *m_private;
};


#endif // OPENGLPOINTLIGHT_H
