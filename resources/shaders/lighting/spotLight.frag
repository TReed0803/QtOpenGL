/*******************************************************************************
 * lighting/pointLight.frag
 *------------------------------------------------------------------------------
 * Apply the lighting calculation to a given fragment of incident light.
 * Uses GBuffer information to access statistics about the scene itself.
 ******************************************************************************/

#include <GlobalBuffer.ubo>
#include <GBuffer.ubo>
#include <Math.glsl> // saturate

// Light Properties
flat in highp vec3 vLightViewPosition;
flat in highp vec3 vLightViewDirection;
flat in highp vec2 vLightPhiThetaAngles;
flat in highp vec4 vLightAttenuation;
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

  // Blinn Phong
  highp float lambertian = max(dot(lightDir, normal), 0.0);
  highp vec3  viewDir    = normalize(-viewPos);
  highp vec3  halfDir    = normalize(lightDir + viewDir);
  highp float specAngle  = max(dot(halfDir, normal), 0.0);
  highp float specFactor = pow(specAngle, specular.w);

  // Spotlight Factor
  highp float spotAngle  = dot(-lightDir, vLightViewDirection);
  highp float spotFactor = smoothstep(vLightPhiThetaAngles.y, vLightPhiThetaAngles.x, spotAngle);

  // Construct Lighting Terms
  highp vec3 diffuseTerm  = vLightDiffuse  * diffuse      * lambertian;
  highp vec3 specularTerm = vLightSpecular * specular.xyz * specFactor;
  fFragColor = vec4(spotFactor * attenuation * (diffuseTerm + specularTerm), 1.0);
}
