/*******************************************************************************
 * lighting/pointLight.frag
 *------------------------------------------------------------------------------
 * Apply the lighting calculation to a given fragment of incident light.
 * Uses GBuffer information to access statistics about the scene itself.
 ******************************************************************************/

#include <GBuffer.ubo>
#include <Physical.glsl>
#include <Math.glsl>

// Light Properties
flat in highp vec3 vLightDirection;
flat in highp vec3 vLightDiffuse;
flat in highp vec3 vLightSpecular;

// Light Output
layout(location = 0) out highp vec4 fFragColor;

void main()
{
  highp vec3 viewDir  = normalize(-viewPosition());
  highp vec3 color = L(vLightDiffuse, vLightDirection, viewDir);
  fFragColor = vec4(color, 1.0);
}
