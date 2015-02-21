#include <EncodeDecode.glsl>
#include <GlobalMatrices.ubo>

vec4 atmosphericColor = vec4(0.0, 0.0, 0.0, 1.0);

// Vertex Inputs
in vec3 vCameraNormal;
in vec4 vCurrCameraPosition;
in vec4 vCurrViewPosition;
in vec4 vPrevViewPosition;
flat in vec3 vDiffuse;
flat in vec4 vSpecular;

// Framebuffer Outputs
layout(location = 0) out vec4 fAmbient;
layout(location = 1) out vec4 fGeometry;
layout(location = 2) out vec4 fMaterial;
layout(location = 3) out vec4 fDynamics;

void main()
{
  //////////////////////////////////////////////////////////////////////////////
  // Deferred Buffer 1: Geometry Buffer
  // Encode Geometry information (Normal, Normal, Specular Exp., Depth)
  fGeometry.xy  = encodeNormal(normalize(vCameraNormal));
  fGeometry.z   = vSpecular.x;
  fGeometry.w   = encodeDepth(gl_FragCoord.z);

  //////////////////////////////////////////////////////////////////////////////
  // Deferred Buffer 2: Material Buffer
  // Encode Material information (Diffuse, Diffuse, Diffuse, Specular )
  fMaterial.xyz = vDiffuse;
  fMaterial.w   = vSpecular.w;

  //////////////////////////////////////////////////////////////////////////////
  // Deferred Buffer 3: Dynamics Buffer
  // Encode Dynamics information (velocity, velocity, velocity, 0)
  vec2 currClipPos = (vCurrViewPosition.xy / vCurrViewPosition.w) * 0.5 + 0.5;
  vec2 prevClipPos = (vPrevViewPosition.xy / vPrevViewPosition.w) * 0.5 + 0.5;
  fDynamics.xy  = currClipPos - prevClipPos;

  //////////////////////////////////////////////////////////////////////////////
  // Perform the Ambient Pass
  // Get material and write ambient / atmospheric pass
  float linearDepth = (depthFar - length(vCurrCameraPosition)) / depthDiff;
  fAmbient = vec4(vDiffuse, 1.0) * ambientColor;
  fAmbient = fAmbient * linearDepth + (1.0 - linearDepth) * atmosphericColor;
}
