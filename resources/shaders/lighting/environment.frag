/*******************************************************************************
 * lighting/environment.frag
 *------------------------------------------------------------------------------
 * Apply the lighting calculation to a given fragment of incident light.
 * Uses GBuffer information to access statistics about the scene itself.
 ******************************************************************************/

#include <GBuffer.ubo>
#include <Math.glsl>
#include <GlobalBuffer.ubo>
#include <Bindings.glsl>

layout(binding = K_TEXTURE_0)
uniform sampler2D environment;
layout(binding = K_TEXTURE_1)
uniform sampler2D irradiance;

// Light Output
layout(location = 0) out highp vec4 fFragColor;

vec3 radiance()
{
  vec3 color = vec3(0.0);
  for (int sample = 0; sample < 40; ++sample)
  {

  }
  return color / float(40);
}

void main()
{
  if (depth() >= 1.0)
  {
    highp vec3 viewDir  = normalize(worldPosition());
    viewDir = vec3(viewDir.x, viewDir.z, -viewDir.y);
    highp vec2 uV = vec2(0.5 + atan(viewDir.y, viewDir.x)/(2.0*M_PI), acos(viewDir.z) / M_PI);
    fFragColor = vec4(texture(environment, uV).rgb, 1.0);
  }
  else
  {
    highp vec4 normal = Current.ViewToWorld * vec4(normal(), 0.0);
    highp vec3 viewDir = normalize(normal.xyz);
    viewDir = vec3(viewDir.x, viewDir.z, -viewDir.y);
    highp vec2 uV = vec2(0.5 + atan(viewDir.y, viewDir.x)/(2.0*M_PI), acos(viewDir.z) / M_PI);
    fFragColor = vec4((diffuse() / M_PI) * texture(irradiance, uV).rgb, 1.0);
    fFragColor += vec4(radiance(), 0.0);
  }
}