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
  // GBuffer Access
  highp vec3 viewPos  = viewPosition();
  highp vec3 normal   = normal();
  highp vec3 diffuse  = diffuse();
  highp vec4 specular = specular();

  // Light Calculations
  highp vec3 lightDir = vLightDirection;
  highp vec3 viewDir  = normalize(-viewPos);
  highp vec3 halfDir  = normalize(vLightDirection + viewDir);
  highp vec3 color    = diffuse / M_PI + Brdf(lightDir, viewDir, halfDir);

  // Output final color
  fFragColor = vec4(max(dot(normal, vLightDirection), 0.0) * color, 1.0);
}
