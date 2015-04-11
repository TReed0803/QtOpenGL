/*******************************************************************************
 * lighting/shadowSpotLightExponential.frag
 *------------------------------------------------------------------------------
 * Apply the lighting calculation to a given fragment of incident light.
 * Uses GBuffer information to access statistics about the scene itself.
 ******************************************************************************/

#include <LightBuffer.ubo>
#include <GBuffer.ubo>
#include <Math.glsl> // saturate
#include <Bindings.glsl>
#include <Physical.glsl>

layout(binding = K_TEXTURE_0)
uniform sampler2D shadowMap;

// Light Input
in mat4 vViewToLightBias;
uniform float k = 60.0;

// Light Output
layout(location = 0) out vec4 fFragColor;

void main()
{
  // GBuffer Access
  vec3 viewPos  = viewPosition();
  vec3 normal   = normal();
  vec3 baseColor= rgb2l(baseColor());

  // Light Information
  vec3  lightVec   = Light.ViewPosition - viewPos;
  float lightDist  = length(lightVec);

  // Construct a finite attenuation
  vec3  lightDir   = lightVec / lightDist;
  vec3  polynomial = vec3(1.0, lightDist, lightDist * lightDist);
  float attenuation = 1.0 / dot(polynomial,Light.Attenuation);
  float linearDist  = lightDist / Light.MaxFalloff;
  attenuation *= saturate(1.0 - linearDist);

  // Blinn Phong
  float lambertian = max(dot(lightDir, normal), 0.0);
  vec3  viewDir    = normalize(-viewPos);
  vec3  halfDir    = normalize(lightDir + viewDir);
  float specAngle  = max(dot(halfDir, normal), 0.0);

  // Spotlight Factor
  float spotAngle  = dot(-lightDir, Light.ViewDirection);
  float spotFactor = smoothstep(Light.OuterAngle, Light.InnerAngle, spotAngle);

  // Shadow Effect
  vec4 shadowCoord = vViewToLightBias * vec4(viewPos, 1.0);
  float occluder = textureProj(shadowMap, shadowCoord.xyw).r;
  float reciever = map_01(shadowCoord.w, Light.NearPlane, Light.MaxFalloff);
  float visibility = saturate(occluder * exp(-Light.Exponential * reciever));

  // Construct Lighting Terms
  vec3 color = Brdf(baseColor, Light.Diffuse, lightDir, viewDir, normal);
  color *= visibility * attenuation * spotFactor;
  color = max(color, vec3(0.0));

  fFragColor = vec4(color, 1.0);

  // Debug Drawing
  //fFragColor += debugExecution(visibility * spotFactor * attenuation);
}
