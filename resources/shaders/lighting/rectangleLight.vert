/*******************************************************************************
 * lighting/pointLight.vert
 *------------------------------------------------------------------------------
 * Pass-through shader that simply deferrs information to fragment shader.
 ******************************************************************************/

#include <GlobalBuffer.ubo>
#include <AreaLightBuffer.ubo>

// Per-Vertex Attribs
layout(location = 0) in vec3 position;

void main()
{
  // Send to Fragment Shader
  gl_Position = vec4(position, 1.0);
}
