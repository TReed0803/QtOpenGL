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

void main()
{
  gl_Position = Current.ViewToPersp * currModelToView * vec4(position, 1.0);
}
