#ifndef MATH_GLSL
#define MATH_GLSL

#include <GlobalBuffer.ubo>

float linearizeDepth(float z)
{
  return (2.0 * depthNear) / (depthFar + depthNear - z * depthDiff);
}

/*
float saturate(float v)
{
  return clamp(v, 0.0, 1.0);
}

vec2 saturate(vec2 v)
{
  return clamp(v, 0.0, 1.0);
}

vec3 saturate(vec3 v)
{
  return clamp(v, 0.0, 1.0);
}

vec4 saturate(vec4 v)
{
  return clamp(v, 0.0, 1.0);
}
*/
vec2 uvCoord()
{
  return gl_FragCoord.xy / vec2(viewWidth, viewHeight);
}

vec2 ndcCoord()
{
  return uvCoord() * 2.0 - 1.0;
}

#endif // MATH_GLSL
