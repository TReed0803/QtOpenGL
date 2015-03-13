/*******************************************************************************
 * lighting/pointLight.frag
 *------------------------------------------------------------------------------
 * Apply the lighting calculation to a given fragment of incident light.
 * Uses GBuffer information to access statistics about the scene itself.
 ******************************************************************************/

#include <LightBuffer.ubo>
#include <GBuffer.ubo>

// Light Output
layout(location = 0) out highp vec4 fFragColor;

void main()
{
  // GBuffer Access
  highp vec3 viewPos = viewPosition();
  highp vec3 normal   = normal();
  highp vec3 diffuse  = diffuse();
  highp vec4 specular = specular();

  // Blinn Phong
  highp float lambertian = max(dot(Light.ViewDirection, normal), 0.0);
  highp vec3  viewDir    = normalize(-viewPos);
  highp vec3  halfDir    = normalize(Light.ViewDirection + viewDir);
  highp float specAngle  = max(dot(halfDir, normal), 0.0);
  highp float specFactor = pow(specAngle, specular.w);

  // Construct Lighting Terms
  highp vec3 diffuseTerm  = Light.Diffuse  * diffuse      * lambertian;
  highp vec3 specularTerm = Light.Specular * specular.xyz * specFactor;
  fFragColor = vec4((diffuseTerm + specularTerm), 1.0);
}
