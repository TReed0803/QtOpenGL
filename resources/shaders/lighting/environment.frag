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
#include <Physical.glsl>
#include <Hammersley.ubo>

layout(binding = K_TEXTURE_0)
uniform sampler2D environment;
layout(binding = K_TEXTURE_1)
uniform sampler2D irradiance;
uniform uvec2 Dimensions = uvec2(2400, 1200);

// Light Output
layout(location = 0) out highp vec4 fFragColor;

vec3 rEnv(vec3 N)
{
  return vec3(N.x, N.z, -N.y);
}

float compute_lod(uint NumSamples, float NoH)
{
  return 0.5 * (log2(float(Dimensions.x * Dimensions.y) / NumSamples) - log2(D(NoH)));
}

vec4 radiance(vec3 N, vec3 V)
{
  // Precalculate Helpers (Note: Must be in World Space)
  vec3 UpVector = abs(N.z) < 0.999 ? ZAxis : XAxis;
  vec3 TangentX = normalize(cross( UpVector, N ));
  vec3 TangentY = cross(N, TangentX);
  float NoV = saturate(dot(N, V));

  vec3 fColor = vec3(0.0);
  const uint NumSamples = 40;
  for (uint i = 0; i < NumSamples; ++i)
  {
    vec2 Xi = Hammersley(i, NumSamples);
    vec3 Li = S(Xi);
    vec3 H  = normalize(Li.x * TangentX + Li.y * TangentY + Li.z * N);
    vec3 L  = normalize(-reflect(V, H));

    float NoL = abs(dot(N, L));
    float NoH = saturate(dot(N, H));
    float VoH = saturate(dot(V, H));
    float lod = compute_lod(NumSamples, NoH);

      vec3  F_ = F(NoL);
      float G_ = G(NoL, NoV, NoH, VoH);
      vec3 LColor = textureSphereLod(environment, rEnv(L), lod).rgb;
      fColor += abs(F_ * G_ * LColor * VoH / (NoH * NoV));
  }

  // Average the results
  return vec4(fColor / float(NumSamples), 1.0);
}

void main()
{
  // Rendering the Environment (Background)
  if (depth() == 1.0)
  {
    highp vec3 viewDir = normalize(worldPosition());
    fFragColor = textureSphereLod(environment, rEnv(viewDir), 0.0);
  }

  // Rendering an Object (Foreground)
  else
  {
    highp vec3 worldNormal = normalize((Current.ViewToWorld * vec4(normal(), 0.0)).xyz);
    highp vec3 V = normalize((Current.ViewToWorld * vec4(-viewPosition(), 0.0)).xyz);
    fFragColor = vec4(K(saturate(dot(worldNormal, V))), 1.0) * vec4(textureSphere(irradiance, rEnv(worldNormal)).rgb * diffuse() / pi, 1.0);
    fFragColor += radiance(worldNormal, V);
  }
  fFragColor = l2rgb(fFragColor);
}