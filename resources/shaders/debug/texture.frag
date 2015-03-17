/*******************************************************************************
 * debug/screen.frag
 *------------------------------------------------------------------------------
 * Passhtrough shader, outputs color to fColor.
 ******************************************************************************/

uniform sampler2D textureData;
in highp vec2 vUv;
layout(location = 0) out highp vec4 fColor;

void main()
{
  fColor = vec4(vec3(log(texture(textureData, vUv).r)/60), 1.0);
}
