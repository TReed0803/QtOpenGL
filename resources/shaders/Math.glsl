/*******************************************************************************
 * Math.glsl
 *------------------------------------------------------------------------------
 * Helper math functions for GLSL.
 ******************************************************************************/

#ifndef MATH_GLSL
#define MATH_GLSL

#define M_PI 3.1415926535897932384626433832795
#define pi   3.1415926535897932384626433832795
#define pi2  6.2831853071795864769252867665590
#define l2   0.6931471805599453094172321214581

#define XAxis vec3(1.0, 0.0, 0.0)
#define YAxis vec3(0.0, 1.0, 0.0)
#define ZAxis vec3(0.0, 0.0, 1.0)
#define KWhite vec3(1.0, 1.0, 1.0)

#include <GlobalBuffer.ubo>

highp float log2(float l)
{
  return log(l) / l2;
}

highp float map_01(float x, float v0, float v1)
{
  return (x - v0) / (v1 - v0);
}

highp float linearizeDepth(highp float z)
{
  return (2.0 * Current.NearPlane) / (Current.SumPlane - z * Current.DiffPlane);
}

highp float saturate(highp float v)
{
  return clamp(v, 0.0, 1.0);
}

highp vec2 saturate(highp vec2 v)
{
  return clamp(v, 0.0, 1.0);
}

highp vec3 saturate(highp vec3 v)
{
  return clamp(v, 0.0, 1.0);
}

highp vec4 saturate(highp vec4 v)
{
  return clamp(v, 0.0, 1.0);
}

highp vec2 uvCoord()
{
#ifdef COMPUTE_SHADER
  return vec2(0.0, 0.0);
#else
  return gl_FragCoord.xy / Current.Dimensions;
#endif
}


highp vec2 ndcCoord()
{
  return uvCoord() * 2.0 - 1.0;
}

highp vec4 debugExecution(float amount)
{
  if (amount > 0)
    return vec4(1.0, 0.0, 0.0, 1.0);
  else
    return vec4(1.0);
}

vec2 Hammersley(uint i, uint N)
{
  return vec2( float(i) / float(N), float(bitfieldReverse(i)) * 2.3283064365386963e-10 );
}

vec2 SphereMap(vec3 N)
{
  return vec2(0.5 - atan(N.y, N.x) / pi2, acos(N.z) / pi);
}

vec3 InvSphereMap(vec2 C)
{
  float alpha = pi2 * (0.5 - C.x);
  float theta = pi * C.y;
  float sine = sin(theta);
  return vec3(cos(alpha) * sine, sin(alpha) * sine, cos(theta));
}

vec4 textureSphere(sampler2D tex, vec3 N)
{
  return texture(tex, SphereMap(N));
}

vec4 textureSphereLod(sampler2D tex, vec3 N, float lod)
{
  return textureLod(tex, SphereMap(N), lod);
}

vec4 rgb2l(vec4 c)
{
  return pow(c, vec4(2.2f));
}

vec4 l2rgb(vec4 c)
{
  return pow(c, vec4(1.0f / 2.2f));
}

vec3 rgb2l(vec3 c)
{
  return pow(c, vec3(2.2f));
}

vec3 l2rgb(vec3 c)
{
  return pow(c, vec3(1.0f / 2.2f));
}

vec2 rgb2l(vec2 c)
{
  return pow(c, vec2(2.2f));
}

vec2 l2rgb(vec2 c)
{
  return pow(c, vec2(1.0f / 2.2f));
}

float rgb2l(float c)
{
  return pow(c, float(2.2f));
}

float l2rgb(float c)
{
  return pow(c, float(1.0f / 2.2f));
}

float randAngle()
{
#ifdef COMPUTE_SHADER
  return 0.0;
#else
  uint x = uint(gl_FragCoord.x);
  uint y = uint(gl_FragCoord.y);
  return (30u * x ^ y + 10u * x * y);
#endif
}

#endif // MATH_GLSL
