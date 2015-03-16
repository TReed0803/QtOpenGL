/*******************************************************************************
 * lighting/pointLight.vert
 *------------------------------------------------------------------------------
 * Pass-through shader that simply deferrs information to fragment shader.
 ******************************************************************************/

#include <GlobalBuffer.ubo>
#include <LightBuffer.ubo>

// Per-Vertex Attribs
layout(location = 0) in highp vec3 position;

// Per-Instance Attribs
layout(location = 4) in highp mat4 currModelToView;

void main()
{
  // Send to Fragment Shader
  gl_Position = Light.ViewToLightPersp * currModelToView * vec4(position, 1.0);
}
