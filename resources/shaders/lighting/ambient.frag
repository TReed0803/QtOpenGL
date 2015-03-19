/*******************************************************************************
 * lighting/ambient.frag
 *------------------------------------------------------------------------------
 * Meant to render with a full screen quad.
 * Applies the atmospheric attenuation using the stored GBuffer values.
 ******************************************************************************/

#include <GBuffer.ubo>
#include <Math.glsl>

out highp vec4 fAmbient;

void main()
{
  highp vec4 atmosphericColor = vec4(0.0, 0.0, 0.0, 1.0);
  highp vec4 ambientColor = vec4(0.0, 0.0, 0.0, 1.0);
  highp float linearDepth = linearDepth();
  fAmbient = vec4(diffuse(), 1.0) * ambientColor;
  fAmbient = fAmbient * (1.0 - linearDepth) + linearDepth * atmosphericColor;
  fAmbient = vec4(pow(fAmbient.xyz,vec3(2.2)),1.0);
}
