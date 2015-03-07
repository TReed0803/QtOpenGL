/*******************************************************************************
 * GBuffer.vert
 *------------------------------------------------------------------------------
 * Calculates previous and current positions, as well as material attributes.
 ******************************************************************************/

#include <GlobalBuffer.ubo>

// Per-Vertex Attributes
layout(location = 0)  in highp vec3 position;
layout(location = 1)  in highp vec3 normal;

// Per-Instance Attributes
layout(location = 2)  in highp vec3 diffuse;
layout(location = 3)  in highp vec4 specular;
layout(location = 4)  in highp mat4 currModelToView;
layout(location = 8)  in highp mat4 prevModelToView;
layout(location = 12) in highp mat4 normalTransform;

// Framebuffer Outputs
out highp vec3 vViewNormal;
out highp vec4 vCurrViewPosition;
out highp vec4 vCurrClipPosition;
out highp vec4 vPrevClipPosition;
flat out highp vec3 vDiffuse;
flat out highp vec4 vSpecular;

void main()
{
  // Calculations
  highp vec4 currViewPos = currModelToView * vec4(position, 1.0);
  highp vec4 prevViewPos = prevModelToView * vec4(position, 1.0);
  highp vec4 viewNormal  = normalTransform * vec4(normal  , 1.0);

  // Outputs
  vViewNormal = viewNormal.xyz;
  vCurrViewPosition = currViewPos;
  vCurrClipPosition = Current.ViewToPersp * currViewPos;
  vPrevClipPosition = Previous.ViewToPersp * prevViewPos;
  //vCurrClipPosition = currViewPos;
  //vPrevClipPosition = prevViewPos;
  vDiffuse = diffuse;
  vSpecular = specular;

  // Final position
  gl_Position = vCurrClipPosition;
}
