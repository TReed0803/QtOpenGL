/*******************************************************************************
 * lighting/ambientOcclusion.vert
 *------------------------------------------------------------------------------
 * Pass-through shader that simply deferrs information to fragment shader.
 ******************************************************************************/

#include <GlobalBuffer.ubo>

// Per-Vertex Attribs
layout(location = 0) in highp vec3 position;

void main()
{
  // Send to Fragment Shader (FSQ)
  gl_Position = vec4(position, 1.0);
}
