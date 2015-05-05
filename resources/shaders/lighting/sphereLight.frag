/*******************************************************************************
 * lighting/pointLight.frag
 *------------------------------------------------------------------------------
 * Apply the lighting calculation to a given fragment of incident light.
 * Uses GBuffer information to access statistics about the scene itself.
 ******************************************************************************/

#include <GBuffer.ubo>
#include <Math.glsl> // saturate
#include <Physical.glsl>
#include <AreaLightBuffer.ubo>

// Light Output
layout(location = 0) out vec4 fFragColor;

void main()
{
  // GBuffer Access
  float sphereRadius = AreaLight.Data0.x;
  vec3 viewPos  = viewPosition();
  vec3 normal   = normal();
  vec3 baseColor= baseColor();

  // Light Information
  vec3  lightVec    = AreaLight.ViewPosition - viewPos;
  float sqrLightDist= dot(lightVec, lightVec);
  float lightDist   = sqrt(sqrLightDist);
  float lightRadius = sphereRadius + AreaLight.Intensity;
  float sqrLightRad = sphereRadius * sphereRadius;

  // From the equation: Luminance = φ / (Aπ);
  float luminance   = AreaLight.Intensity / (2 * pi * sqrLightRad * pi);
  // Inverse Square for attenuation Intensity = Luminance / (dist * dist)
  float invSqrAtt   = 1.0 / max(sqrLightDist, 0.001);
  // Smooth attenuation formed by inverse square
  float attenuation = pow(saturate(1.0 - pow(lightDist / lightRadius, 4.0)), 2.0) / (sqrLightDist + 1.0);
  float surfaceLum  = luminance * pow(saturate(1.0 - pow(sphereRadius / lightRadius, 4.0)), 2.0) / (sqrLightDist + 1.0);

  // Debug Lighting
  vec3  viewRay  = normalize(viewPos);
  float distArea = length(cross(viewRay, AreaLight.ViewPosition));
  if (distArea < sphereRadius && AreaLight.ViewPosition.z > viewPos.z)
  {
    fFragColor = vec4(AreaLight.Color * 1000.0, 1.0);
  }
  else
  {
    fFragColor = vec4(0.0, 0.0, 0.0, 1.0);
    vec3  viewDir     = normalize(-viewPos);
    vec3  lightDir    = lightVec / lightDist;
    vec3  r           = normalize(reflect(viewDir, normal));

    // Illuminance (Correct horizon handling)
    float Beta = acos(dot(normal, lightDir));
    float H = lightDist;
    float h = H / sphereRadius;
    float x = sqrt(h * h - 1.0);
    float y = -x / tan(Beta);

    float illuminance = 0;
    if (h * cos(Beta) > 1)
    {
      illuminance = cos(Beta) / (h * h);
    }
    else
    {
      illuminance = (1.0 / (pi * h * h)) *
        (cos(Beta) * acos(y) - x * sin(Beta) * sqrt(1.0 - y * y)) +
        atan(sin(Beta) * sqrt(1.0 - y * y) / x) / pi;
    }
    illuminance *= pi;
    vec3 Kdiff = AreaLight.Color * luminance * max(baseColor * illuminance * attenuation, 0.0);

    // Specular Contribution
    {
      vec3 L = lightVec;
      vec3 centerToRay = dot(L, r) * r - L;
      vec3 closestPoint = L + centerToRay * saturate(sphereRadius/length(centerToRay));
      vec3 l = normalize(closestPoint);

      vec3    H = normalize(l + viewDir);
      float NoL = saturate(dot(normal, l));
      float NoV = saturate(dot(normal, viewDir));
      float NoH = saturate(dot(normal, H));
      float VoH = saturate(dot(viewDir, H));
      float specular = saturate(Specular(NoL, NoV, NoH, VoH) * NoL);

      vec3 KSpec = AreaLight.Color * max(specular * baseColor * surfaceLum, 0.0);
      fFragColor = vec4(BlendMaterial(Kdiff, KSpec), 1.0);
    }
  }
}
