#include <GBuffer.ubo>

const vec2 middle = vec2(0.5, 0.5);

flat in highp vec4 vLightAttenuation;
flat in highp vec3 vLightViewPosition;
flat in highp vec3 vLightDiffuse;
flat in highp vec3 vLightSpecular;

layout(location = 0) out highp vec4 fFragColor;

float saturate(float f)
{
  return clamp(f, 0.0, 1.0);
}

void main()
{
  // GBuffer Access
  vec3 viewPos = viewPosition();

  vec3  lightVec   = vLightViewPosition - viewPos;
  float lightDist  = length(lightVec);
  if (lightDist < vLightAttenuation.w)
  {
    vec3 normal   = normal();
    vec3 diffuse  = diffuse();
    vec4 specular = specular();

    vec3  lightDir   = lightVec / lightDist;
    vec3  viewDir    = normalize(-viewPos);
    vec3  polynomial = vec3(1.0f, lightDist, lightDist * lightDist);
    float attenuation = 1.0 / dot(polynomial,vLightAttenuation.xyz);
    attenuation *= saturate(1.0 - (lightDist / vLightAttenuation.w));

    // Blinn Phong
    float lambertian = max(dot(lightDir, normal), 0.0);
    vec3  halfDir    = normalize(lightDir + viewDir);
    float specAngle  = max(dot(halfDir, normal), 0.0);
    float specFactor = pow(saturate(specAngle), specular.w);

    // Apply Lighting Terms
    vec3 diffuseTerm  = vLightDiffuse  * diffuse      * lambertian;
    vec3 specularTerm = vLightSpecular * specular.xyz * specFactor;

    // Final fragment color
    fFragColor = vec4(attenuation * (diffuseTerm + specularTerm), 1.0);
  }
  else
  {
    discard;
  }
}
