/*******************************************************************************
 * lighting/Physical.glsl
 *------------------------------------------------------------------------------
 * Physically-Based lighting calculations.
 ******************************************************************************/

 #ifndef PHYSICAL_GLSL
 #define PHYSICAL_GLSL

#include <Math.glsl>

float roughness2()
{
  return pow(roughness(), 2.0);
}

////////////////////////////////////////////////////////////////////////////////
// F: Fresnel Factors
// Notes: fresnel() = F0(0 degrees)
//   The Fresnel is the amount
////////////////////////////////////////////////////////////////////////////////
vec3 FNone(vec3 lightDir, vec3 halfDir)
{
  return fresnel();
}

vec3 FSchlick2(vec3 lightDir, vec3 halfDir)
{
  return (vec3(1.0) - fresnel()) * pow(1.0 - dot(lightDir, halfDir), 5.0);
}

vec3 FSchlick(vec3 lightDir, vec3 halfDir)
{
  return fresnel() + FSchlick2(lightDir, halfDir);
}

vec3 FCookTorrance(vec3 lightDir, vec3 halfDir)
{
  // Preparation
  vec3 n = (1.0 + sqrt(fresnel())) / (1.0 - sqrt(fresnel()));
  vec3 c = vec3(max(dot(lightDir, halfDir), 0.0));
  vec3 g = sqrt(n * n + c * c - vec3(1.0));

  // Helpers
  vec3 gMc = g - c;
  vec3 gPc = g + c;
  vec3 gMc1= gMc * c + 1.0;
  vec3 gPc1= gPc * c - 1.0;

  // Factors
  vec3 factor0 = vec3(0.5);
  vec3 factor1 = pow(gMc / gPc, vec3(2.0));
  vec3 factor2 = 1.0 + pow(gPc1 / gMc1, vec3(2.0));

  // End Result
  return  max(factor0 * factor1 * factor2, 1.0);
}

////////////////////////////////////////////////////////////////////////////////
// G: Geometry Occlusion
// Notes:
////////////////////////////////////////////////////////////////////////////////
float GImplicit(vec3 lightDir, vec3 viewDir, vec3 halfDir)
{
  return max(dot(normal(), lightDir), 0.0) * max(dot(normal(), viewDir), 0.0);
}

float GNeumann(vec3 lightDir, vec3 viewDir, vec3 halfDir)
{
  float nDotL = dot(normal(), lightDir);
  float nDotV = dot(normal(), viewDir);
  return (nDotL * nDotV) / max(nDotL, nDotV);
}

float GCookTorrance(vec3 lightDir, vec3 viewDir, vec3 halfDir)
{
  float nDotH = max(dot(normal(), halfDir),0.0);
  float nDotL = max(dot(normal(), lightDir),0.0);
  float nDotV = max(dot(normal(), viewDir),0.0);
  float vDotH = max(dot(viewDir, halfDir),0.0);

  float orig = (2 * nDotH) / vDotH;
  float minT = nDotV * orig;
  float maxT = nDotL * orig;

  return min(1.0, min(minT, maxT));
}

float GKelemen(vec3 lightDir, vec3 viewDir, vec3 halfDir)
{
  float nDotL = dot(normal(), lightDir);
  float nDotV = dot(normal(), viewDir);
  float vDotH = dot(viewDir, halfDir);

  return (nDotL * nDotV) / pow(vDotH, 2.0);
}

float GSmithBeckmann_(vec3 viewDir)
{
  float nDotV = max(dot(normal(), viewDir), 0.0);
  float c = nDotV / (roughness() * roughness() - nDotV * nDotV);
  float final = (3.535 * c + 2.181 * c * c) / (1.0 + 2.276 * c + 2.577 * c * c);

  // If c < 1.6, return 1.0, otherwise return 0.0
  float ifCheck = 1.0 - step(1.6, c);
  return (ifCheck * final) + (1.0 - ifCheck);
}

float GSmithBeckmann(vec3 lightDir, vec3 viewDir, vec3 halfDir)
{
  return GSmithBeckmann_(lightDir) * GSmithBeckmann_(viewDir);
}

float GSmithGgx_(vec3 viewDir)
{
  float nDotV = dot(normal(), viewDir);
  float numerator = 2.0 * nDotV;
  float roughness4 = pow(roughness(),4.0);
  float denominator = nDotV + sqrt(roughness4 + (1.0 - roughness4) * nDotV * nDotV);

  return numerator / denominator;
}

float GSmithGgx(vec3 lightDir, vec3 viewDir, vec3 halfDir)
{
  return GSmithGgx_(lightDir) * GSmithGgx_(viewDir);
}

float GSmithSchlickBeckmann_(vec3 viewDir)
{
  float k = roughness() * roughness() * sqrt(2.0 / M_PI);
  float nDotV = max(dot(normal(), viewDir), 0.0);
  return nDotV / (nDotV * (1.0 - k) + k);
}

float GSmithSchlickBeckmann(vec3 lightDir, vec3 viewDir, vec3 halfDir)
{
  return GSmithSchlickBeckmann_(lightDir) * GSmithSchlickBeckmann_(viewDir);
}

////////////////////////////////////////////////////////////////////////////////
// D: Microfacet Normal Distribution Functions (NDF)
// Notes: Krough = Roughness parameter
////////////////////////////////////////////////////////////////////////////////
float DPhong(vec3 halfDir)
{
  float factor0 = (roughness2() + 2.0) / (2.0 * M_PI);
  float factor1 = pow(dot(normal(), halfDir), roughness2());

  return factor0 * factor1;
}

float DBlinnPhong(vec3 halfDir)
{
  float Krough2 = roughness2() * roughness2();
  float exponent = (2.0 / Krough2) - 2.0;

  return pow(dot(normal(), halfDir), exponent) / (M_PI * Krough2);
}

float DBeckmann(vec3 halfDir)
{
  float Krough2 = roughness2() * roughness2();
  float nDotH = max(dot(normal(), halfDir),0.0);
  float nDotH2 = nDotH * nDotH;

  float factor0 = 1.0 / (M_PI * Krough2 * nDotH2 * nDotH2);
  float factor1 = exp((nDotH2 - 1.0) / (Krough2 * nDotH2));

  return factor0 * factor1;
}

float DGgx(vec3 halfDir)
{
  float Krough2 = roughness2() * roughness2();
  float nDotH = max(dot(normal(), halfDir), 0.0);
  float nDotH2= nDotH * nDotH;

  return Krough2 / (M_PI * pow(nDotH2 * (Krough2 - 1.0) + 1.0, 2.0));
}

float DGgxAnisotropic(vec3 halfDir)
{
  return 0.0; // Not Implemented
}

////////////////////////////////////////////////////////////////////////////////
// Subroutines
// Note: This block of code should be commented out on GLES 3.1.
////////////////////////////////////////////////////////////////////////////////
#ifdef   GL_ES

// Embedded System GL does not support subroutines
# define F_FACTOR FSchlick
# define G_FACTOR GSmithSchlickBeckmann
# define D_FACTOR DGgx

#else // GL_ES

// Declaration
subroutine vec3 FresnelSubroutine(vec3 lightDir, vec3 halfDir);
subroutine float GeometrySubroutine(vec3 lightDir, vec3 viewDir, vec3 halfDir);
subroutine float DistributionSubroutine(vec3 halfDir);
subroutine uniform FresnelSubroutine uFresnel;
subroutine uniform GeometrySubroutine uGeometry;
subroutine uniform DistributionSubroutine uDistribution;
#define F_FACTOR uFresnel
#define G_FACTOR uGeometry
#define D_FACTOR uDistribution

// Fresnel Definitions
subroutine(FresnelSubroutine)
vec3 sFNone(vec3 lightDir, vec3 halfDir)
{
  return FNone(lightDir, halfDir);
}
subroutine(FresnelSubroutine)
vec3 sFSchlick2(vec3 lightDir, vec3 halfDir)
{
  return FSchlick2(lightDir, halfDir);
}
subroutine(FresnelSubroutine)
vec3 sFSchlick(vec3 lightDir, vec3 halfDir)
{
  return FSchlick(lightDir, halfDir);
}
subroutine(FresnelSubroutine)
vec3 sFCookTorrance(vec3 lightDir, vec3 halfDir)
{
  return FCookTorrance(lightDir, halfDir);
}

// Geometry Definitions
subroutine(GeometrySubroutine)
float sGImplicit(vec3 lightDir, vec3 viewDir, vec3 halfDir)
{
  return GImplicit(lightDir, viewDir, halfDir);
}
subroutine(GeometrySubroutine)
float sGNeumann(vec3 lightDir, vec3 viewDir, vec3 halfDir)
{
  return GNeumann(lightDir, viewDir, halfDir);
}
subroutine(GeometrySubroutine)
float sGCookTorrance(vec3 lightDir, vec3 viewDir, vec3 halfDir)
{
  return GCookTorrance(lightDir, viewDir, halfDir);
}
subroutine(GeometrySubroutine)
float sGKelemen(vec3 lightDir, vec3 viewDir, vec3 halfDir)
{
  return GKelemen(lightDir, viewDir, halfDir);
}
subroutine(GeometrySubroutine)
float sGSmithBeckmann(vec3 lightDir, vec3 viewDir, vec3 halfDir)
{
  return GSmithBeckmann(lightDir, viewDir, halfDir);
}
subroutine(GeometrySubroutine)
float sGSmithGgx(vec3 lightDir, vec3 viewDir, vec3 halfDir)
{
  return GSmithGgx(lightDir, viewDir, halfDir);
}
subroutine(GeometrySubroutine)
float sGSmithSchlickBeckmann(vec3 lightDir, vec3 viewDir, vec3 halfDir)
{
  return GSmithSchlickBeckmann(lightDir, viewDir, halfDir);
}

// Distribution Definitions
subroutine(DistributionSubroutine)
float sDPhong(vec3 halfDir)
{
  return DPhong(halfDir);
}
subroutine(DistributionSubroutine)
float sDBlinnPhong(vec3 halfDir)
{
  return DBlinnPhong(halfDir);
}
subroutine(DistributionSubroutine)
float sDBeckmann(vec3 halfDir)
{
  return DBeckmann(halfDir);
}
subroutine(DistributionSubroutine)
float sDGgx(vec3 halfDir)
{
  return DGgx(halfDir);
}
subroutine(DistributionSubroutine)
float sDGgxAnisotropic(vec3 halfDir)
{
  return DGgxAnisotropic(halfDir);
}

#endif // GL_ES

////////////////////////////////////////////////////////////////////////////////
// Selected BRDF:
// Notes: The following defines will be set dynamically, to allow for selecting
//        different combinations of different factors through C++ code.
////////////////////////////////////////////////////////////////////////////////
vec3 F(vec3 lightDir, vec3 halfDir)
{
  return F_FACTOR(lightDir, halfDir);
}

float G(vec3 lightDir, vec3 viewDir, vec3 halfDir)
{
  return G_FACTOR(lightDir, viewDir, halfDir);
}

float D(vec3 halfDir)
{
  return D_FACTOR(halfDir);
}

vec3 Brdf(vec3 lightDir, vec3 viewDir, vec3 halfDir)
{
  vec3 color = F(lightDir, halfDir) * G(lightDir, viewDir, halfDir) * D(halfDir);
  return color / (4 * max(dot(normal(), lightDir), 0.0) * max(dot(normal(), viewDir), 0.0));
}

vec3 L(vec3 lightIntensity, vec3 lightDir, vec3 viewDir)
{
  vec3 halfDir = normalize(lightDir + viewDir);
  vec3 color = diffuse() / M_PI + Brdf(lightDir, viewDir, halfDir);
  return color * lightIntensity * max(dot(normal(), lightDir), 0.0);
}

#endif // PHYSICAL_GLSL
