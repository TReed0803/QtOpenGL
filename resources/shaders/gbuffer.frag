#include <EncodeDecode.glsl>
#include <GlobalBuffer.ubo>

// Vertex Inputs
in vec3 vViewNormal;
in vec4 vCurrViewPosition;
in vec4 vCurrClipPosition;
in vec4 vPrevClipPosition;
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
  vec2 currHomogeneousPos = (vCurrClipPosition.xy / vCurrClipPosition.w) * 0.5 + 0.5;
  vec2 prevHomogeneousPos = (vPrevClipPosition.xy / vPrevClipPosition.w) * 0.5 + 0.5;
  fGeometry.xy  = encodeNormal(normalize(vViewNormal));
  fGeometry.zw  = currHomogeneousPos - prevHomogeneousPos;

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
