/*******************************************************************************
 * lighting/ambient.vert
 *------------------------------------------------------------------------------
 * Meant to render with a full screen quad.
 ******************************************************************************/

// Per-Vertex Attribs
layout(location = 0) in highp vec3 position;

void main()
{
  gl_Position = vec4(position, 1.0);
}
