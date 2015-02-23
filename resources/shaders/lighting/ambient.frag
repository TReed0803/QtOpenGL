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
  highp float linearDepth = linearDepth();
  fAmbient = vec4(diffuse(), 1.0) * ambientColor;
  fAmbient = fAmbient * (1.0 - linearDepth) + linearDepth * atmosphericColor;
}
