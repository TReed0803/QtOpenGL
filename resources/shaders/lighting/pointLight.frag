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
  vec2 uv = gl_FragCoord.xy / vec2(viewWidth, viewHeight);
  // Note: Currently storing position in world space. Will change to camera space.
  vec3 viewPos = (worldToView * vec4(positionCoord(uv),1.0)).xyz;

  vec3  lightVec   = vLightViewPosition - viewPos;
  float lightDist  = length(lightVec);
  if (lightDist < vLightAttenuation.w)
  {
    vec3 normal   = normalCoord(uv);
    vec3 diffuse  = diffuseCoord(uv);
    vec4 specular = specularCoord(uv);

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
