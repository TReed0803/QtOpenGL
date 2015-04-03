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
flat in vec3 vLightDirection;
flat in vec3 vLightDiffuse;
flat in vec3 vLightSpecular;

// Light Output
layout(location = 0) out highp vec4 fFragColor;

void main()
{
  vec3 viewDir  = normalize(-viewPosition());
  vec3 color = Brdf(diffuse(), vLightDiffuse, vLightDirection, viewDir, normal());
  fFragColor = vec4(color, 1.0);
}
