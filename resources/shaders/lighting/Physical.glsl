/*******************************************************************************
 * lighting/Physical.glsl
 *------------------------------------------------------------------------------
 * Physically-Based lighting calculations.
 ******************************************************************************/

 #ifndef PHYSICAL_GLSL
 #define PHYSICAL_GLSL

#include <Math.glsl>

// Forward Declarations
vec3 F(float NoL);

////////////////////////////////////////////////////////////////////////////////
// F: Fresnel Factors
// Notes: fresnel() = F0(0 degrees)
//   The Fresnel is the amount
////////////////////////////////////////////////////////////////////////////////
vec3 FNone(float NoL)
{
  return fresnel();
}

vec3 FSchlick(float NoL)
{
  return fresnel() + (vec3(1.0) - fresnel()) * pow(1.0 - NoL, 5.0);
}

vec3 FCookTorrance(float NoL)
{
  // Preparation
  float c = NoL;
  float n = (1.0 + sqrt(fresnel().x)) / (1.0 - sqrt(fresnel().x));
  float g = sqrt(n * n - 1.0 + c * c);

  // Helpers
  float gMc  = g - c;
  float gPc  = g + c;
  float gMc1 = gMc * c + 1.0;
  float gPc1 = gPc * c - 1.0;
  float factor0 = (gMc * gMc) / (gPc * gPc);
  float factor1 = 1 + (gPc1 * gPc1) / (gMc1 * gMc1);

  // End Result
  return vec3(0.5 * factor0 * factor1);
}

////////////////////////////////////////////////////////////////////////////////
// G: Geometry Occlusion
// Notes:
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
  float Krough2 = roughness() * roughness();
  float c = 1.0 / (Krough2 * (NoV - 1.0));
  float c2 = c * c;
  float final = (3.535 * c + 2.181 * c2) / (1.0 + 2.276 * c + 2.577 * c2);

  // If c < 1.6, return 1.0, otherwise return 0.0
  float ifCheck = 1.0 - step(1.6, c);
  return (ifCheck * final) + (1.0 - ifCheck);
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
// D: Microfacet Normal Distribution Functions (NDF)
// Notes: Krough = Roughness parameter
////////////////////////////////////////////////////////////////////////////////
float DPhong(float NoH)
{
  float Krough2 = roughness() * roughness();
  float ap = (2.0 / Krough2) - 2.0;
  return NoH * ((ap + 2.0) / pi2) * pow(NoH, ap);
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
// DSample: NDF Sampling Functions
// Notes:
////////////////////////////////////////////////////////////////////////////////
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
  float ap = (2.0 / (roughness() * roughness())) - 2.0;
  float Theta = acos(pow(E.x, 2.0 / (ap + 2.0))) / pi2;
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
// Kd: Diffuse Energy Ratio
////////////////////////////////////////////////////////////////////////////////
vec3 KNone(float NoL)
{
  return vec3(1.0);
}

vec3 KActual(float NoL)
{
  return vec3(1.0) - F(NoL);
}

vec3 KFresnel(float NoL)
{
  return vec3(1.0) - fresnel();
}

////////////////////////////////////////////////////////////////////////////////
// Subroutines
// Note: This block of code should be commented out on GLES 3.1.
////////////////////////////////////////////////////////////////////////////////
#ifdef   GL_ES

// Embedded System GL does not support subroutines
# define uFresnel FSchlick
# define uGeometry GSmithSchlickBeckmann
# define uDistribution DGgx
# define uDistributionSample DGgxSample

#else // GL_ES

// Declaration
subroutine vec3 FresnelSubroutine(float LoH);
subroutine float GeometrySubroutine(float NoL, float NoV, float NoH, float VoH);
subroutine float DistributionSubroutine(float NoH);
subroutine vec3 DistributionSampleSubroutine(vec2 lightDir);
subroutine vec3 DiffuseSubroutine(float NoL);
subroutine uniform FresnelSubroutine uFresnel;
subroutine uniform GeometrySubroutine uGeometry;
subroutine uniform DistributionSubroutine uDistribution;
subroutine uniform DistributionSampleSubroutine uDistributionSample;

// Fresnel Definitions
subroutine(FresnelSubroutine)
vec3 sFNone(float LoH)
{
  return FNone(LoH);
}
subroutine(FresnelSubroutine)
vec3 sFSchlick(float LoH)
{
  return FSchlick(LoH);
}
subroutine(FresnelSubroutine)
vec3 sFCookTorrance(float LoH)
{
  return FCookTorrance(LoH);
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

#endif // GL_ES

////////////////////////////////////////////////////////////////////////////////
// Selected BRDF:
// Notes: The following defines will be set dynamically, to allow for selecting
//        different combinations of different factors through C++ code.
////////////////////////////////////////////////////////////////////////////////
vec3 F(float NoL)
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

vec3 K(float NoL)
{
  return KActual(NoL);
}

vec3 f(float NoL, float NoV, float NoH, float VoH)
{
  return F(NoL) * G(NoL, NoV, NoH, VoH) * D(NoH);
}

float Pdf(float NoL, float NoV)
{
  return (4.0 * NoL * NoV);
}

////////////////////////////////////////////////////////////////////////////////
// Complete Brdf
////////////////////////////////////////////////////////////////////////////////
vec3 Brdf(vec3 Kd, vec3 Li, float NoL, float NoV, float NoH, float VoH)
{
  return K(NoL) * Kd / pi + f(NoL, NoV, NoH, VoH) * Li * NoL / Pdf(NoL, NoV);
}

vec3 Brdf(vec3 Kd, vec3 Li, vec3 L, vec3 V, vec3 N)
{
  vec3    H = normalize(L + V);
  float NoL = saturate(dot(N, L));
  float NoV = saturate(dot(N, V));
  float NoH = saturate(dot(N, H));
  float VoH = saturate(dot(V, H));
  return Brdf(Kd, Li, NoL, NoV, NoH, VoH);
}

#endif // PHYSICAL_GLSL
