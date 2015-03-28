/*******************************************************************************
 * GBuffer.vert
 *------------------------------------------------------------------------------
 * Calculates previous and current positions, as well as material attributes.
 ******************************************************************************/

#include <GlobalBuffer.ubo>
#include <Object.ubo>

// Per-Vertex Attributes
layout(location = 0)  in highp vec3 position;
layout(location = 1)  in highp vec3 normal;

// Framebuffer Outputs
out highp vec3 vViewNormal;
out highp vec4 vCurrClipPosition;
out highp vec4 vPrevClipPosition;

void main()
{
  // Calculations
  highp vec4 currViewPos = Object.CurrentModelToView  * vec4(position, 1.0);
  highp vec4 prevViewPos = Object.PreviousModelToView * vec4(position, 1.0);
  highp vec4 viewNormal  = Object.NormalTransform     * vec4(normal  , 1.0);

  // Outputs
  vViewNormal       = viewNormal.xyz;
  vCurrClipPosition = Current.ViewToPersp  * currViewPos;
  vPrevClipPosition = Previous.ViewToPersp * prevViewPos;

  // Final position
  gl_Position = vCurrClipPosition;
}
