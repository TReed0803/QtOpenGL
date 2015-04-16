/*******************************************************************************
 * lighting/ambientOcclusion.frag
 *------------------------------------------------------------------------------
 * Apply the lighting calculation to a given fragment of incident light.
 * Uses GBuffer information to access statistics about the scene itself.
 ******************************************************************************/

#include <GBuffer.ubo> // viewPosition, normal
#include <Math.glsl> // Hammersley, randAngle
#include <GlobalBuffer.ubo> // Current.Dimensions

// Light Output
layout(location = 0) out highp float fFragColor;

// Note: Keep in mind that the proper values for the ambient occlusion
//       shader should change depending on the units in which your game world
//       works within. It may require dynamic modification of these terms.
uniform float SampleRadius = 1.0;
uniform float ShadowScalar = 1.3;
uniform float DepthThreshold = 0.0025;
uniform float ShadowContrast = 0.5;
uniform int NumSamples = 20;

void main()
{
  float visibility = 0.0;
  vec3 P = viewPosition();
  vec3 N = normal();
  float PerspectiveRadius = (SampleRadius / P.z);

  // Main sample loop, this is where we will preform our random
  // sampling and estimate the ambient occlusion for the current fragment.
  for (int i = 0; i < NumSamples; ++i)
  {
    // Generate Sample Position
    vec2 E = Hammersley(i, NumSamples) * vec2(pi, pi2);
    E.y += randAngle(); // Apply random angle rotation
    vec2 sE= vec2(cos(E.y), sin(E.y)) * PerspectiveRadius * cos(E.x);
    vec2 Sample = gl_FragCoord.xy / Current.Dimensions + sE;

    // Create Alchemy helper variables
    vec3 Pi         = viewPosition(Sample);
    vec3 V          = Pi - P;
    float sqrLen    = dot(V, V);
    float Heaveside = step(sqrt(sqrLen), SampleRadius);
    float dD        = DepthThreshold * P.z;

    // For arithmetically removing edge-bleeding error
    // introduced by clamping the ambient occlusion map.
    float EdgeError = step(0.0, Sample.x) * step(0.0, 1.0 - Sample.x) *
                      step(0.0, Sample.y) * step(0.0, 1.0 - Sample.y);

    // Summation of Obscurance Factor
    visibility += (max(0.0, dot(N, V) + dD) * Heaveside * EdgeError) / (sqrLen + 0.0001);
  }

  // Final scalar multiplications for averaging and intensifying shadows
  visibility *= (2 * ShadowScalar) / NumSamples;
  visibility = max(0.0, 1.0 - pow(visibility, ShadowContrast));
  fFragColor = visibility;
}