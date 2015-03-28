/*******************************************************************************
 * lighting/pointLight.frag
 *------------------------------------------------------------------------------
 * Apply the lighting calculation to a given fragment of incident light.
 * Uses GBuffer information to access statistics about the scene itself.
 ******************************************************************************/

#include <GBuffer.ubo>
#include <Math.glsl> // saturate
#include <Physical.glsl>

// Light Properties
flat in highp vec4 vLightAttenuation;
flat in highp vec3 vLightViewPosition;
flat in highp vec3 vLightDiffuse;
flat in highp vec3 vLightSpecular;

// Light Output
layout(location = 0) out highp vec4 fFragColor;

void main()
{
  // GBuffer Access
  highp vec3 viewPos = viewPosition();
  highp vec3 normal   = normal();
  highp vec3 diffuse  = diffuse();
  highp vec4 specular = specular();

  // Light Information
  highp vec3  lightVec   = vLightViewPosition - viewPos;
  highp float lightDist  = length(lightVec);

  // Construct a finite attenuation
  highp vec3  lightDir   = lightVec / lightDist;
  highp vec3  polynomial = vec3(1.0, lightDist, lightDist * lightDist);
  highp float attenuation = 1.0 / dot(polynomial,vLightAttenuation.xyz);
  attenuation *= saturate(1.0 - (lightDist / vLightAttenuation.w));

  // Construct Half-Vector
  highp vec3 viewDir = normalize(viewPos);
  highp vec3 halfDir = normalize(lightDir + viewDir);

  // Microfacet BRDF
  vec3 color = F(lightDir, halfDir) * G(lightDir, viewDir, halfDir) * D(halfDir);
  color /= (4 * dot(normal, lightDir) * dot(normal, viewDir));

  // Commit final color
  fFragColor = vec4(attenuation * color, 1.0);
}
