/*******************************************************************************
 * lighting/pointLight.frag
 *------------------------------------------------------------------------------
 * Apply the lighting calculation to a given fragment of incident light.
 * Uses GBuffer information to access statistics about the scene itself.
 ******************************************************************************/

#include <GBuffer.ubo>
#include <Math.glsl> // saturate

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

  highp vec3  lightVec   = vLightViewPosition - viewPos;
  highp float lightDist  = length(lightVec);
  if (lightDist < vLightAttenuation.w)
  {
    highp vec3 normal   = normal();
    highp vec3 diffuse  = diffuse();
    highp vec4 specular = specular();

    // Construct a finite attenuation
    highp vec3  lightDir   = lightVec / lightDist;
    highp vec3  polynomial = vec3(1.0, lightDist, lightDist * lightDist);
    highp float attenuation = 1.0 / dot(polynomial,vLightAttenuation.xyz);
    attenuation *= saturate(1.0 - (lightDist / vLightAttenuation.w));

    // Blinn Phong
    highp float lambertian = max(dot(lightDir, normal), 0.0);
    highp float specFactor = 0.0;
    if (lambertian > 0.0)
    {
      highp vec3  viewDir    = normalize(-viewPos);
      highp vec3  halfDir    = normalize(lightDir + viewDir);
      highp float specAngle  = max(dot(halfDir, normal), 0.0);
      specFactor = pow(specAngle, specular.w);
    }

    // Construct Lighting Terms
    highp vec3 diffuseTerm  = vLightDiffuse  * diffuse      * lambertian;
    highp vec3 specularTerm = vLightSpecular * specular.xyz * specFactor;
    fFragColor = vec4(attenuation * (diffuseTerm + specularTerm), 1.0);
  }
  else
  {
    discard;
  }
}
