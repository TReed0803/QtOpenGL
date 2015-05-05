/*******************************************************************************
 * lighting/Physical.glsl
 *------------------------------------------------------------------------------
 * Physically-Based lighting calculations.
 ******************************************************************************/

#ifndef PHYSICAL_GLSL
#define PHYSICAL_GLSL

#include <Math.glsl>

////////////////////////////////////////////////////////////////////////////////
// F: Fresnel Factors
// Notes:
//   F0 is the reflectance at an angle of incidence 0 degrees.
//   (eg. the light is directly hitting the material)
//   In our physically-based shading model, we represent metallic() as the F0
//   term. We could also call it fresnel, but these terms are somewhatly
//   interchangeable. As F0 goes up, we have more metallic objects.
//
// General Equation for Fresnel:
     float F(float VoH);
//
////////////////////////////////////////////////////////////////////////////////

float FNone(float VoH)
{
  return metallic();
}

float FSchlick(float VoH)
{
  float Kmetallic = metallic();
  return Kmetallic + (1.0 - Kmetallic) * pow(1.0 - VoH, 5.0);
}

float FCookTorrance(float VoH)
{
  // Preparation
  float c = VoH;
  float Kmetallic = sqrt(metallic());
  float n = (1.0 + Kmetallic) / (1.0 - Kmetallic);
  float g = sqrt(n * n - 1.0 + c * c);

  // Helpers
  float gMc  = g - c;
  float gPc  = g + c;
  float gMc1 = gMc * c + 1.0;
  float gPc1 = gPc * c - 1.0;
  float factor0 = (gMc * gMc) / (gPc * gPc);
  float factor1 = 1 + (gPc1 * gPc1) / (gMc1 * gMc1);

  // End Result
  return 0.5 * factor0 * factor1;
}

float FSphericalGaussian(float VoH)
{
  float Kmetallic = metallic();
  return Kmetallic + (1.0 - Kmetallic) * pow(2.0, (-5.55473 * VoH - 6.98316) * VoH);
}

////////////////////////////////////////////////////////////////////////////////
// G: Geometry Occlusion
// Notes:
//   Geometry Occlusion is a microfacet approximation to the amount of the
//   surface which undergoes self-shadowing. As you could imagine, some of the
//   more complete approximations take roughness() of the material into
//   consideration. However, the more complete a calculation is the more
//   expensive it becomes (though it looks more physically accurate). I've found
//   Beckmann to be the best looking approximation, but also the most expensive.
//   Generally GKelemen looks pretty similar to Smith-Ggx, but it is not
//   continuous in the second-derivative. So instead I recommend Smith.
//
// General Equation for Geometry Occlusion:
     float G(float NoL, float NoV, float NoH, float VoH);
//
////////////////////////////////////////////////////////////////////////////////
float GImplicit(float NoL, float NoV, float NoH, float VoH)
{
  return NoL * NoV;
}

float GNeumann(float NoL, float NoV, float NoH, float VoH)
{
  return (NoL * NoV) / max(NoL, NoV);
}

float GCookTorrance(float NoL, float NoV, float NoH, float VoH)
{
  float orig = (2 * NoH) / VoH;
  float minT = NoV * orig;
  float maxT = NoL * orig;

  return min(1.0, min(minT, maxT));
}

float GKelemen(float NoL, float NoV, float NoH, float VoH)
{
  return min(2.0 * (NoH * NoV) / VoH, 1.0);
}

float GSmithBeckmann_(float NoV, float VoH)
{
  float c = 1.0 / (roughness() * sqrt(1.0 + 1.0 / NoV));
  float c2 = c * c;
  float final = (3.535 * c + 2.181 * c2) / (1.0 + 2.276 * c + 2.577 * c2);
  float base = saturate(VoH / NoV);
  if (c < 1.6) return base * final;
  return base;
}

float GSmithBeckmann(float NoL, float NoV, float NoH, float VoH)
{
  return GSmithBeckmann_(NoL, VoH) * GSmithBeckmann_(NoV, VoH);
}

float GSmithGgx_(float NoV)
{
  float numerator = 2.0 * NoV;
  float NoV2 = NoV * NoV;
  float Krough2 = roughness() * roughness();
  float denominator = NoV + sqrt(NoV2 + Krough2 * (1.0 - NoV2));

  return numerator / denominator;
}

float GSmithGgx(float NoL, float NoV, float NoH, float VoH)
{
  return GSmithGgx_(NoL) * GSmithGgx_(NoV);
}

float GSmithSchlickBeckmann_(float NoV)
{
  float k = roughness() * roughness() * sqrt(pi2);
  return NoV / (NoV * (1.0 - k) + k);
}

float GSmithSchlickBeckmann(float NoL, float NoV, float NoH, float VoH)
{
  return GSmithSchlickBeckmann_(NoL) * GSmithSchlickBeckmann_(NoV);
}

float GSmith(float NoL, float NoV, float NoH, float VoH)
{
  float Krough2 = roughness() * roughness();
  float k = sqrt((2.0 * Krough2) / pi);
  return NoV / (NoV * (1.0 - k) + k);
}

////////////////////////////////////////////////////////////////////////////////
// D: Microfacet Normal Distribution (NDF)
// Notes:
//   The other property we need to approximate is the distribution of the
//   current incident light across our microfacet surface. At any angle, we
//   should be able to integrate to get 1, which makes sense because at any
//   angle we should be able to see a whole surface (eg. never more surface than
//   we expect to see, or less for that matter). The NDF allows us to see the
//   probability that the incident light reflects back at us. As one would
//   imagine, similarly to Geometry Occlusion, the best approximations take the
//   roughness of the material into consideration. (I haven't seen one that
//   doesn't, actually.) I tend to prefer Ggx for it's simplicity. Though again,
//   Beckmann looks nicer, Ggx is just faster and not too different looking.
//
// General Equation for Micofacet Normal Distribution:
     float D(float NoH);
//
////////////////////////////////////////////////////////////////////////////////

float DPhong(float NoH)
{
  float Krough2 = roughness() * roughness();
  float ap = (2.0 / Krough2) - 2.0;
  return saturate(NoH) * ((ap + 2.0) / pi2) * pow(NoH, ap);
}

float DBeckmann(float NoH)
{
  // Recall that tan2 + 1 = sec2
  // And that sec = 1 / cos;
  // Thusly, sec2 = 1 / cos2;
  //
  // tan2 = 1 / cos2 - 1
  // Beckmann distribution w/ Ward modification.
  float Krough2 = roughness() * roughness();
  float NoH2 = NoH * NoH;
  return exp((NoH2 - 1.0) / (NoH2 * Krough2)) / (pi * Krough2 * NoH * NoH2);
}

float DGgx(float NoH)
{
  // Note: Generally sin2 + cos2 = 1
  // Also: Dgtr = c / (a * cos2 + sin2)
  // So...
  float Krough2 = roughness() * roughness();
  float NoH2 = NoH * NoH;
  float denom = 1.0 + NoH2 * (Krough2 - 1.0);
  return Krough2 / (pi * denom * denom);
}

////////////////////////////////////////////////////////////////////////////////
// S: CDF Sampling
// Notes:
//   When doing reflective materials, we tend to sample along a particular
//   cumulative distribution. This is a direct correlation to the NDFs above,
//   and can be found by integrating with respects to some point <x, y>. The
//   idea is if we have a random point E, we skew the randomness of the values
//   towards a particular distribution. Again Beckmann is best, but in this
//   case the performance difference between Beckmann and Ggx is minimal. Not to
//   mention that (in a a less dynamic application) the selection/skewing of
//   random points can be precalculated - I recommend Beckmann's integral. What
//   this means is that for IBL, you are using Beckmann distribution, but
//   regular lights use Ggx. This tends to look the most physically accurate,
//   and represents a wider spread of metal reflectiveness that appears more
//   appropriate.
//
// General Equation for CDF Sampling:
     vec3 S(vec2 E);
//
////////////////////////////////////////////////////////////////////////////////

// Helper function (make skewed point into vec3 direction)
vec3 MakeSample(float Theta, float Phi)
{
  Phi += randAngle();
  float SineTheta = sin(Theta);

  float x = cos(Phi) * SineTheta;
  float y = sin(Phi) * SineTheta;
  float z = cos(Theta);

  return vec3(x, y, z);
}

vec3 DPhongSample(vec2 E)
{
  float ap = (2.0 / roughness()) - 2.0;
  float Theta = acos(pow(E.x, 2.0 / (ap + 2.0))) / (pi2 * pi2);
  float Phi = pi2 * E.y;
  return MakeSample(Theta, Phi);
}

vec3 DBeckmannSample(vec2 E)
{
  float a = roughness() * roughness();
  float Theta = atan(sqrt(-a * log(1.0 - E.x)));
  float Phi = pi2 * E.y;
  return MakeSample(Theta, Phi);
}

vec3 DGgxSample(vec2 E)
{
  float a = roughness() * roughness();
  float Theta = atan(sqrt((a * E.x) / (1.0 - E.x)));
  float Phi = pi2 * E.y;
  return MakeSample(Theta, Phi);
}

////////////////////////////////////////////////////////////////////////////////
// K: Diffuse Energy
// Notes:
//   When you have a very reflective material, diffuse tends to approach 0 since
//   all of the light energy bounces off the material. A simple approximation to
//   this is by taking (1 - F0) at different angles of incidence. The amount of
//   energy conserved is a product of the light's incident angle to/from the
//   observer, so better approximations take NoL and NoV into consideration.
//   Disney has a novel approach that basically recalculates the incident
//   light according the Schlick's Approximation using varying F0 terms based on
//   the roughness of the object. This would make sense because as an object
//   becomes more rough, the light bounces around the microfacet more, losing
//   it's  intensity ever-so slightly with each bounce.
//
// General Diffuse Energy Conservation Function:
     float K(float NoL, float NoV);
//
////////////////////////////////////////////////////////////////////////////////

// Helper for Disney's roughness conservation approximation
// Originally: Fd90 = 0.5 + 2 * cos(incident) * roughness
// But it's use within K() always subtracts one, so I simply propagated this
// subtraction into the constant 0.5 above. It's the same equation in the end.
float Fd90(float NoL)
{
  return 2.0 * NoL * roughness() + 0.4;
}

float KNone(float NoL, float NoV)
{
  return 1.0;
}

float KActual(float NoL, float NoV)
{
  return (1.0 - F(NoL)) * (1.0 - F(NoV));
}

float KFresnel(float NoL, float NoV)
{
  return 1.0 - F(NoL);
}

float KDifference(float NoL, float NoV)
{
  return 1.0 - metallic();
}

float KDisney(float NoL, float NoV)
{
  return (1.0 + Fd90(NoL) * pow(1.0 - NoL, 5.0)) *
         (1.0 + Fd90(NoV) * pow(1.0 - NoV, 5.0));
}

////////////////////////////////////////////////////////////////////////////////
// Subroutines
// Note: This block of code should be commented out on GLES 3.1.
////////////////////////////////////////////////////////////////////////////////
#if defined(GL_ES)

// Embedded System GL does not support subroutines
# define uFresnel FSchlick
# define uGeometry GSmithSchlickBeckmann
# define uDistribution DGgx
# define uDistributionSample DGgxSample

#else // GL_ES

// Declaration
subroutine float FresnelSubroutine(float LoH);
subroutine float GeometrySubroutine(float NoL, float NoV, float NoH, float VoH);
subroutine float DistributionSubroutine(float NoH);
subroutine vec3 DistributionSampleSubroutine(vec2 lightDir);
subroutine vec3 DiffuseSubroutine(float NoL);

// Fresnel Definitions
subroutine(FresnelSubroutine)
float sFNone(float LoH)
{
  return FNone(LoH);
}
subroutine(FresnelSubroutine)
float sFSchlick(float LoH)
{
  return FSchlick(LoH);
}
subroutine(FresnelSubroutine)
float sFCookTorrance(float LoH)
{
  return FCookTorrance(LoH);
}
subroutine(FresnelSubroutine)
float sFSphericalGaussian(float LoH)
{
  return FSphericalGaussian(LoH);
}

// Geometry Definitions
subroutine(GeometrySubroutine)
float sGImplicit(float NoL, float NoV, float NoH, float VoH)
{
  return GImplicit(NoL, NoV, NoH, VoH);
}
subroutine(GeometrySubroutine)
float sGNeumann(float NoL, float NoV, float NoH, float VoH)
{
  return GNeumann(NoL, NoV, NoH, VoH);
}
subroutine(GeometrySubroutine)
float sGCookTorrance(float NoL, float NoV, float NoH, float VoH)
{
  return GCookTorrance(NoL, NoV, NoH, VoH);
}
subroutine(GeometrySubroutine)
float sGKelemen(float NoL, float NoV, float NoH, float VoH)
{
  return GKelemen(NoL, NoV, NoH, VoH);
}
subroutine(GeometrySubroutine)
float sGSmithBeckmann(float NoL, float NoV, float NoH, float VoH)
{
  return GSmithBeckmann(NoL, NoV, NoH, VoH);
}
subroutine(GeometrySubroutine)
float sGSmithGgx(float NoL, float NoV, float NoH, float VoH)
{
  return GSmithGgx(NoL, NoV, NoH, VoH);
}
subroutine(GeometrySubroutine)
float sGSmithSchlickBeckmann(float NoL, float NoV, float NoH, float VoH)
{
  return GSmithSchlickBeckmann(NoL, NoV, NoH, VoH);
}
subroutine(GeometrySubroutine)
float sGSmith(float NoL, float NoV, float NoH, float VoH)
{
  return GSmith(NoL, NoV, NoH, VoH);
}

// Distribution Definitions
subroutine(DistributionSubroutine)
float sDPhong(float NoH)
{
  return DPhong(NoH);
}
subroutine(DistributionSubroutine)
float sDBeckmann(float NoH)
{
  return DBeckmann(NoH);
}
subroutine(DistributionSubroutine)
float sDGgx(float NoH)
{
  return DGgx(NoH);
}

// Distribution Samples
subroutine(DistributionSampleSubroutine)
vec3 sDPhongSample(vec2 random)
{
  return DPhongSample(random);
}
subroutine(DistributionSampleSubroutine)
vec3 sDBeckmannSample(vec2 random)
{
  return DBeckmannSample(random);
}
subroutine(DistributionSampleSubroutine)
vec3 sDGgxSample(vec2 random)
{
  return DGgxSample(random);
}

subroutine uniform FresnelSubroutine uFresnel;
subroutine uniform GeometrySubroutine uGeometry;
subroutine uniform DistributionSubroutine uDistribution;
subroutine uniform DistributionSampleSubroutine uDistributionSample;

#endif // GL_ES

////////////////////////////////////////////////////////////////////////////////
// Selected BRDF:
// Notes: The following defines will be set dynamically, to allow for selecting
//        different combinations of different factors through C++ code.
////////////////////////////////////////////////////////////////////////////////
float F(float NoL)
{
  return uFresnel(NoL);
}

float G(float NoL, float NoV, float NoH, float VoH)
{
  return uGeometry(NoL, NoV, NoH, VoH);
}

float D(float NoH)
{
  return uDistribution(NoH);
}

vec3 S(vec2 random)
{
  return uDistributionSample(random);
}

// Note: For some reason, possibly a driver error, I cannot create a subroutine
//       for this term. Hopefully after an update I will be able to do so.
float K(float NoL, float NoV)
{
  return KDisney(NoL, NoV);
}

float Pdf(float NoL, float NoV)
{
  return (4.0 * NoL * NoV);
}

////////////////////////////////////////////////////////////////////////////////
// Complete Brdf
////////////////////////////////////////////////////////////////////////////////

vec3 BlendDielectric(vec3 Kdiff, vec3 Kspec, vec3 Kbase)
{
  return Kdiff + Kspec;
}

vec3 BlendMetal(vec3 Kdiff, vec3 Kspec, vec3 Kbase)
{
  return Kspec * Kbase;
}

vec3 BlendMaterial(vec3 Kdiff, vec3 Kspec)
{
  vec3  Kbase = baseColor();
  float scRange = smoothstep(0.25, 0.45, metallic());
  vec3  dielectric = BlendDielectric(Kdiff, Kspec, Kbase);
  vec3  metal = BlendMetal(Kdiff, Kspec, Kbase);
  return mix(dielectric, metal, scRange);
}

float Specular(float NoL, float NoV, float NoH, float VoH)
{
  return F(VoH) * G(NoL, NoV, NoH, VoH) * D(NoH) / Pdf(NoL, NoV);
}

vec3 Brdf(vec3 Kd, float NoL, float NoV, float NoH, float VoH)
{
  vec3  Kdiff  = Kd / pi;
  float Kspec = Specular(NoL, NoV, NoH, VoH);
  return BlendMaterial(Kdiff, vec3(Kspec));
}

vec3 Brdf(vec3 Kd, vec3 Li, vec3 L, vec3 V, vec3 N)
{
  vec3    H = normalize(L + V);
  float NoL = saturate(dot(N, L));
  float NoV = saturate(dot(N, V));
  float NoH = saturate(dot(N, H));
  float VoH = saturate(dot(V, H));
  return Brdf(Kd, NoL, NoV, NoH, VoH) * Li * NoL;
}

#endif // PHYSICAL_GLSL
