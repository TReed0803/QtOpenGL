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
  fColor = texture(textureData, vUv);
}
