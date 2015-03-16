/*******************************************************************************
 * debug/screen.vert
 *------------------------------------------------------------------------------
 * Passhtrough shader, draws directly to screen.
 ******************************************************************************/

layout(location = 0) in highp vec3 position;
layout(location = 1) in highp vec3 uv;
out highp vec2 vUv;

void main()
{
  gl_Position = vec4(position, 1.0);
  vUv = uv.xy;
}
