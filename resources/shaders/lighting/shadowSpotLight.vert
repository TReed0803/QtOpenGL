/*******************************************************************************
 * lighting/pointLight.vert
 *------------------------------------------------------------------------------
 * Pass-through shader that simply deferrs information to fragment shader.
 ******************************************************************************/

#include <SpotLight.ubo>

// Per-Vertex Attribs
layout(location = 0) in highp vec3 position;

void main()
{
  // Send to Fragment Shader
  gl_Position = Light.ModelToPersp * vec4(position, 1.0);
}
