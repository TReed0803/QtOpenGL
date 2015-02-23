/*******************************************************************************
 * Math.glsl
 *------------------------------------------------------------------------------
 * Helper math functions for GLSL.
 ******************************************************************************/

#ifndef MATH_GLSL
#define MATH_GLSL

#include <GlobalBuffer.ubo>

highp float linearizeDepth(highp float z)
{
  return (2.0 * depthNear) / (depthFar + depthNear - z * depthDiff);
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
  return gl_FragCoord.xy / vec2(viewWidth, viewHeight);
}

highp vec2 ndcCoord()
{
  return uvCoord() * 2.0 - 1.0;
}

#endif // MATH_GLSL
