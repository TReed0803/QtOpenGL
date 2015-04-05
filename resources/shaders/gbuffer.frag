/*******************************************************************************
 * GBuffer.frag
 *------------------------------------------------------------------------------
 * Writes final information to the GBuffers.
 ******************************************************************************/

#include <EncodeDecode.glsl>
#include <Material.ubo>

// Vertex Inputs
in highp vec3 vViewNormal;
in highp vec4 vCurrClipPosition;
in highp vec4 vPrevClipPosition;

// Framebuffer Outputs
layout(location = 0) out highp vec4 fGeometry;
layout(location = 1) out highp vec4 fMaterial;
layout(location = 2) out highp float fSurface;
layout(location = 3) out highp vec4 fPhysical;

void main()
{
  // Translate clip [-1,1] -> homogenous [0,1]
  highp vec2 currHomogeneousPos = vCurrClipPosition.xy / vCurrClipPosition.w;
  highp vec2 prevHomogeneousPos = vPrevClipPosition.xy / vPrevClipPosition.w;
  highp vec2 homogeneousDiff = currHomogeneousPos - prevHomogeneousPos;

  //////////////////////////////////////////////////////////////////////////////
  // Deferred Buffer 1: Geometry Buffer
  // Encode Geometry information (Normal, Normal, Velocity, Velocity)
  fGeometry.xy  = encodeNormal(normalize(vViewNormal));
  fGeometry.zw  = homogeneousDiff;

  //////////////////////////////////////////////////////////////////////////////
  // Deferred Buffer 2: Material Buffer
  // Encode Material information (BaseColor, BaseColor, BaseColor, Metallic)
  fMaterial.xyz = Material.BaseColor;
  fMaterial.w   = Material.Metallic;

  //////////////////////////////////////////////////////////////////////////////
  // Deferred Buffer 3: Surface Buffer
  // Encode Dynamics information (Roughness)
  fSurface = Material.Roughness;
}
