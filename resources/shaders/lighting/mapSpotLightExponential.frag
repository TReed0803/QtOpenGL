/*******************************************************************************
 * lighting/pointLight.frag
 *------------------------------------------------------------------------------
 * Apply the lighting calculation to a given fragment of incident light.
 * Uses GBuffer information to access statistics about the scene itself.
 ******************************************************************************/

#include <LightBuffer.ubo> // Light.Radius
#include <Math.glsl> // map_01

in highp vec4 vViewPosition;
layout(location = 0) out float expMap;

void main()
{
  float depthDivisor = (1.0 / gl_FragCoord.w);
  float mappedDivisor = map_01(depthDivisor, Light.NearPlane, Light.MaxFalloff);
  expMap = exp(Light.Exponential * mappedDivisor);
}
