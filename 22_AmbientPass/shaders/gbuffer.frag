#include <EncodeDecode.glsl>
#include <GlobalMatrices.ubo>

const vec4 atmosphericColor = vec4(0.0, 0.0, 0.0, 1.0);

in highp vec3 vNormal;
in highp vec4 vPrevPosition;
in highp vec4 vCurrPosition;
in highp vec3 vDiffuse;
in highp vec4 vSpecular;

in highp vec4 cameraPosition;

layout(location = 0) out highp vec4 fAmbient;
layout(location = 1) out highp vec4 fGeometry;
layout(location = 2) out highp vec4 fMaterial;
layout(location = 3) out highp vec4 fDynamics;

void main()
{
  //////////////////////////////////////////////////////////////////////////////
  // Deferred Buffer 1: Geometry Buffer
  // Encode Geometry information (Normal, Normal, Specular Exp., Depth)
  fGeometry.xy = encodeNormal(normalize(vNormal));
  fGeometry.z = vSpecular.x;
  fGeometry.w = encodeDepth(gl_FragCoord.z);

  //////////////////////////////////////////////////////////////////////////////
  // Deferred Buffer 2: Material Buffer
  // Encode Material information (Diffuse, Diffuse, Diffuse, Specular)
  fMaterial.xyz = vDiffuse;
  fMaterial.w = vSpecular.w;

  //////////////////////////////////////////////////////////////////////////////
  // Deferred Buffer 3: Dynamics Buffer
  // Encode Dynamics information (velocity, velocity, velocity, 0)
  vec2 a = (vCurrPosition.xy / vCurrPosition.w) * 0.5 + 0.5;
  vec2 b = (vPrevPosition.xy / vPrevPosition.w) * 0.5 + 0.5;
  fDynamics.xy = a - b;

  // Get material and write ambient / atmospheric pass
  float linearDepth = (depthFar - length(cameraPosition)) / depthDiff;
  fAmbient = vec4(vDiffuse, 1.0) * ambientColor;
  fAmbient = fAmbient * linearDepth + (1.0 - linearDepth) * atmosphericColor;
}
