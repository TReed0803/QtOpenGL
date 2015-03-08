#ifndef OPENGLLIGHT_H
#define OPENGLLIGHT_H OpenGLLight

#include <KColor>
#include <KVector3D>

class OpenGLLight
{
public:
  OpenGLLight();

  void setAttenuation(float c, float ex1, float ex2);
  void setAttenuation(KVector3D const &attn);
  KVector3D const &attenuation() const;
  void setDiffuse(float r, float g, float b);
  void setDiffuse(KColor const &rgb);
  KColor const &diffuse() const;
  void setSpecular(float r, float g, float b);
  void setSpecular(KColor const &rgb);
  KColor const &specular() const;
  void setShadowCasting(bool sc);
  bool shadowCasting() const;

  template <bool pointer>
  struct ShadowCastingPred;

  template <>
  struct ShadowCastingPred<true> : public std::unary_function<bool, const OpenGLLight*>
  {
    bool operator()(const OpenGLLight *light) const
    {
      return light->shadowCasting();
    }
  };

  template <>
  struct ShadowCastingPred<false> : public std::unary_function<bool, const OpenGLLight&>
  {
    bool operator()(const OpenGLLight &light) const
    {
      return light.shadowCasting();
    }
  };

protected:
  bool m_shadowCasting;
  KVector3D m_attenuation;
  KColor m_diffuse;
  KColor m_specular;
};

inline void OpenGLLight::setAttenuation(float c, float ex1, float ex2)
{
  setAttenuation(KVector3D(c, ex1, ex2));
}

inline void OpenGLLight::setAttenuation(KVector3D const &attn)
{
  m_attenuation = attn;
}

inline KVector3D const &OpenGLLight::attenuation() const
{
  return m_attenuation;
}

inline void OpenGLLight::setDiffuse(float r, float g, float b)
{
  setDiffuse(KColor(r, g, b));
}

inline void OpenGLLight::setDiffuse(KColor const &rgb)
{
  m_diffuse = rgb;
}

inline KColor const &OpenGLLight::diffuse() const
{
  return m_diffuse;
}

inline void OpenGLLight::setSpecular(float r, float g, float b)
{
  setSpecular(KColor(r, g, b));
}

inline void OpenGLLight::setSpecular(KColor const &rgb)
{
  m_specular = rgb;
}

inline KColor const &OpenGLLight::specular() const
{
  return m_specular;
}

inline void OpenGLLight::setShadowCasting(bool sc)
{
  m_shadowCasting = sc;
}

inline bool OpenGLLight::shadowCasting() const
{
  return m_shadowCasting;
}

#endif // OPENGLLIGHT_H
