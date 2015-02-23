/*******************************************************************************
 * GBuffer.vert
 *------------------------------------------------------------------------------
 * Calculates previous and current positions, as well as material attributes.
 ******************************************************************************/

#include <GlobalBuffer.ubo>

// Per-Vertex Attributes
layout(location = 0)  in vec3 position;
layout(location = 1)  in vec3 normal;

// Per-Instance Attributes
layout(location = 2)  in vec3 diffuse;
layout(location = 3)  in vec4 specular;
layout(location = 4)  in mat4 currModelToView;
layout(location = 8)  in mat4 prevModelToView;
layout(location = 12) in mat4 normalTransform;

// Framebuffer Outputs
out vec3 vViewNormal;
out vec4 vCurrViewPosition;
out vec4 vCurrClipPosition;
out vec4 vPrevClipPosition;
flat out vec3 vDiffuse;
flat out vec4 vSpecular;

void main()
{
  // Calculations
  vec4 currViewPos = currModelToView * vec4(position, 1.0);
  vec4 prevViewPos = prevModelToView * vec4(position, 1.0);
  vec4 viewNormal  = normalTransform * vec4(normal  , 1.0);

  // Outputs
  vViewNormal = viewNormal.xyz;
  vCurrViewPosition = currViewPos;
  vCurrClipPosition = viewToClip * currViewPos;
  vPrevClipPosition = viewToClip * prevViewPos;
  vDiffuse = diffuse;
  vSpecular = specular;

  // Final position
  gl_Position = vCurrClipPosition;
}
