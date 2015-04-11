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
#include <ToneMapping.glsl>

layout(binding = K_TEXTURE_0)
uniform sampler2D environment;
layout(binding = K_TEXTURE_1)
uniform sampler2D irradiance;
uniform uvec2 Dimensions = uvec2(1200,2400);

// Light Output
layout(location = 0) out highp vec4 fFragColor;

// Correct the environment so that it's not flipped around.
// Normally you would want to have this handled at load time.
vec3 rEnv(vec3 N)
{
  return vec3(-N.x, N.z, -N.y);
}

// Computes the exact mip-map to reference for the specular contribution.
// Accessing the proper mip-map allows us to approximate the integral for this
// angle of incidence on the current object.
float compute_lod(uint NumSamples, float NoH)
{
  return 0.5 * (log2(float(Dimensions.x * Dimensions.y) / NumSamples) - log2(D(NoH)));
}

// Calculates the specular influence for a surface at the current fragment
// location. This is an approximation of the lighting integral itself.
vec3 radiance(vec3 N, vec3 V)
{
  // Precalculate rotation for +Z Hemisphere to microfacet normal.
  vec3 UpVector = abs(N.z) < 0.999 ? ZAxis : XAxis;
  vec3 TangentX = normalize(cross( UpVector, N ));
  vec3 TangentY = cross(N, TangentX);

  // Note: I ended up using abs() for situations where the normal is
  // facing a little away from the view to still accept the approximation.
  // I believe this is due to a separate issue with normal storage, so
  // you may only need to saturate() each dot value instead of abs().
  float NoV = abs(dot(N, V));

  // Approximate the integral for lighting contribution.
  vec3 fColor = vec3(0.0);
  const uint NumSamples = 20;
  for (uint i = 0; i < NumSamples; ++i)
  {
    vec2 Xi = Hammersley(i, NumSamples);
    vec3 Li = S(Xi); // Defined elsewhere as subroutine
    vec3 H  = normalize(Li.x * TangentX + Li.y * TangentY + Li.z * N);
    vec3 L  = normalize(-reflect(V, H));

    // Calculate dot products for BRDF
    float NoL = abs(dot(N, L));
    float NoH = abs(dot(N, H));
    float VoH = abs(dot(V, H));
    float lod = compute_lod(NumSamples, NoH);

    float F_ = F(VoH); // Defined elsewhere as subroutine
    float G_ = G(NoL, NoV, NoH, VoH); // Defined elsewhere as subroutine
    vec3 LColor = textureSphereLod(environment, rEnv(L), lod).rgb;

    // Since the sample is skewed towards the Distribution, we don't need
    // to evaluate all of the factors for the lighting equation. Also note
    // that this function is calculating the specular portion, so we absolutely
    // do not add any more diffuse here.
    fColor += F_ * G_ * LColor * VoH / (NoH * NoV);
  }

  // Average the results
  return fColor / float(NumSamples);
}

void main()
{
  vec3 V = normalize((Current.ViewToWorld * vec4(-viewPosition(), 0.0)).xyz);
  vec3 color;
  if (depth() == 1.0)
  {
    color = textureSphereLod(environment, rEnv(-V), 0.0).rgb;
  }
  else
  {
    vec3 N = normalize((Current.ViewToWorld * vec4(normal(), 0.0)).xyz);
    vec3 L = normalize(-reflect(V, N));
    float NoV = saturate(dot(N, V));
    float NoL = saturate(dot(N, L));

    // Calculate the color
    vec3 irrMap = textureSphereLod(irradiance, rEnv(N), 0.0).rgb;
    vec3 Kdiff  = irrMap * baseColor() / pi;
    vec3 Kspec  = radiance(N, V);

    // Mix the materials
    color = BlendMaterial(Kdiff, Kspec);
  }
  fFragColor = vec4(color, 1.0);
}