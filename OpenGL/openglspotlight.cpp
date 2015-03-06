#include "openglspotlight.h"

#include <KMacros>
#include <KMath>
#include <KTransform3D>
#include <KVector3D>
#include <KColor>

class OpenGLSpotLightPrivate
{
public:
  OpenGLSpotLightPrivate();
  KTransform3D m_transform;
  KVector3D m_attenuation;
  float m_depth;
  float m_innerAngle;
  float m_outerAngle;
  float m_angleOfInfluence;
  KColor m_diffuse;
  KColor m_specular;
};

OpenGLSpotLightPrivate::OpenGLSpotLightPrivate() :
  m_diffuse(0.8f, 0.8f, 0.8f), m_specular(1.0f, 1.0f, 1.0f), m_depth(20.0f), m_attenuation(1.0f, 0.01f, 0.1f)
{
  // Intentionally Empty
}

OpenGLSpotLight::OpenGLSpotLight() :
  m_private(new OpenGLSpotLightPrivate)
{
  setInnerAngle(15.0f);
  setOuterAngle(30.0f);
}

OpenGLSpotLight::~OpenGLSpotLight()
{
  // Intentionally Empty
}

float OpenGLSpotLight::CalculateScalar(int segments)
{
  return
    std::sqrt
    (
      std::pow
      (
        Karma::sec
        (
          Karma::Pi / segments
        ),
        2.0f
      )
    );
}

void OpenGLSpotLight::translate(float x, float y, float z)
{
  P(OpenGLSpotLightPrivate);
  p.m_transform.translate(x, y, z);
}

void OpenGLSpotLight::translate(const KVector3D &trans)
{
  P(OpenGLSpotLightPrivate);
  p.m_transform.translate(trans);
}

void OpenGLSpotLight::setTranslation(float x, float y, float z)
{
  P(OpenGLSpotLightPrivate);
  p.m_transform.setTranslation(x, y, z);
}

void OpenGLSpotLight::setTranslation(const KVector3D &pos)
{
  P(OpenGLSpotLightPrivate);
  p.m_transform.setTranslation(pos);
}

const KVector3D &OpenGLSpotLight::translation() const
{
  P(const OpenGLSpotLightPrivate);
  return p.m_transform.translation();
}

void OpenGLSpotLight::rotate(float angle, float x, float y, float z)
{
  P(OpenGLSpotLightPrivate);
  p.m_transform.rotate(angle, x, y, z);
}

void OpenGLSpotLight::rotate(float angle, const KVector3D &axis)
{
  P(OpenGLSpotLightPrivate);
  p.m_transform.rotate(angle, axis);
}

void OpenGLSpotLight::rotate(const KQuaternion &quat)
{
  P(OpenGLSpotLightPrivate);
  p.m_transform.rotate(quat);
}

void OpenGLSpotLight::setRotation(float angle, float x, float y, float z)
{
  P(OpenGLSpotLightPrivate);
  p.m_transform.setRotation(angle, x, y, z);
}

void OpenGLSpotLight::setRotation(float angle, const KVector3D &axis)
{
  P(OpenGLSpotLightPrivate);
  p.m_transform.setRotation(angle, axis);
}

void OpenGLSpotLight::setRotation(const KQuaternion &quat)
{
  P(OpenGLSpotLightPrivate);
  p.m_transform.setRotation(quat);
}

const KQuaternion &OpenGLSpotLight::rotation() const
{
  P(const OpenGLSpotLightPrivate);
  return p.m_transform.rotation();
}

void OpenGLSpotLight::setDirection(const KVector3D &dir)
{
  P(OpenGLSpotLightPrivate);
  p.m_transform.lookTowards(dir);
}

void OpenGLSpotLight::setDirection(float x, float y, float z)
{
  P(OpenGLSpotLightPrivate);
  p.m_transform.lookTowards(KVector3D(x, y, z));
}

KVector3D OpenGLSpotLight::direction() const
{
  P(const OpenGLSpotLightPrivate);
  return p.m_transform.forward();
}

const KMatrix4x4 &OpenGLSpotLight::toMatrix()
{
  P(OpenGLSpotLightPrivate);
  return p.m_transform.toMatrix();
}

void OpenGLSpotLight::setAttenuation(float c, float ex1, float ex2)
{
  P(OpenGLSpotLightPrivate);
  p.m_attenuation = KVector3D(c, ex1, ex2);
}

void OpenGLSpotLight::setAttenuation(const KVector3D &attn)
{
  P(OpenGLSpotLightPrivate);
  p.m_attenuation = attn;
}

const KVector3D &OpenGLSpotLight::attenuation() const
{
  P(const OpenGLSpotLightPrivate);
  return p.m_attenuation;
}

void OpenGLSpotLight::setDiffuse(float r, float g, float b)
{
  P(OpenGLSpotLightPrivate);
  p.m_diffuse = KColor(r, g, b);
}

void OpenGLSpotLight::setDiffuse(const KColor &rgb)
{
  P(OpenGLSpotLightPrivate);
  p.m_diffuse = rgb;
}

const KColor &OpenGLSpotLight::diffuse() const
{
  P(const OpenGLSpotLightPrivate);
  return p.m_diffuse;
}

void OpenGLSpotLight::setSpecular(float r, float g, float b)
{
  P(OpenGLSpotLightPrivate);
  p.m_specular = KColor(r, g, b);
}

void OpenGLSpotLight::setSpecular(const KColor &rgb)
{
  P(OpenGLSpotLightPrivate);
  p.m_specular = rgb;
}

const KColor &OpenGLSpotLight::specular()
{
  P(OpenGLSpotLightPrivate);
  return p.m_specular;
}

void OpenGLSpotLight::setInnerAngle(float angle)
{
  P(OpenGLSpotLightPrivate);
  angle /= 2.0f;
  float rads = Karma::DegreesToRads(angle);
  p.m_innerAngle = std::cos(rads);
}

float OpenGLSpotLight::innerAngle() const
{
  P(const OpenGLSpotLightPrivate);
  return p.m_innerAngle;
}

void OpenGLSpotLight::setOuterAngle(float angle)
{
  P(OpenGLSpotLightPrivate);
  angle /= 2.0f;
  float rads = Karma::DegreesToRads(angle);
  p.m_outerAngle = std::cos(rads);
  p.m_angleOfInfluence = rads;
}

float OpenGLSpotLight::outerAngle() const
{
  P(const OpenGLSpotLightPrivate);
  return p.m_outerAngle;
}

void OpenGLSpotLight::setDepth(float d)
{
  P(OpenGLSpotLightPrivate);
  p.m_depth = d;
  p.m_transform.setScaleZ(d);
  float z = p.m_transform.scale().z();
  float dz = (p.m_transform.scale().z() * std::tan(p.m_angleOfInfluence)) * CalculateScalar(32);
  p.m_transform.setScaleX(dz);
  p.m_transform.setScaleY(dz);
}

float OpenGLSpotLight::depth() const
{
  P(const OpenGLSpotLightPrivate);
  return p.m_depth;
}

