#include <EncodeDecode.glsl>
#include <GlobalBuffer.ubo>

// Vertex Inputs
in vec3 vCameraNormal;
in vec4 vCurrCameraPosition;
in vec4 vCurrViewPosition;
in vec4 vPrevViewPosition;
flat in vec3 vDiffuse;
flat in vec4 vSpecular;

// Framebuffer Outputs
layout(location = 0) out vec4 fGeometry;
layout(location = 1) out vec4 fMaterial;
layout(location = 2) out vec4 fSurface;

void main()
{
  //////////////////////////////////////////////////////////////////////////////
  // Deferred Buffer 1: Geometry Buffer
  // Encode Geometry information (Normal, Normal, , Depth)
  vec2 currClipPos = (vCurrViewPosition.xy / vCurrViewPosition.w) * 0.5 + 0.5;
  vec2 prevClipPos = (vPrevViewPosition.xy / vPrevViewPosition.w) * 0.5 + 0.5;
  fGeometry.xy  = encodeNormal(normalize(vCameraNormal));
  fGeometry.zw  = currClipPos - prevClipPos;

  //////////////////////////////////////////////////////////////////////////////
  // Deferred Buffer 2: Material Buffer
  // Encode Material information (Diffuse, Diffuse, Diffuse, Specular Color)
  fMaterial.xyz = vDiffuse;
  fMaterial.w   = vSpecular.z;

  //////////////////////////////////////////////////////////////////////////////
  // Deferred Buffer 3: Surface Buffer
  // Encode Dynamics information (Specular Exp.)
  fSurface.x = vSpecular.w;
}
