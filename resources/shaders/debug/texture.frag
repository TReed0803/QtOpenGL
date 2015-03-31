/*******************************************************************************
 * debug/screen.frag
 *------------------------------------------------------------------------------
 * Passhtrough shader, outputs color to fColor.
 ******************************************************************************/

#include <Bindings.glsl>

in highp vec2 vUv;
layout(location = 0) out highp vec4 fColor;
layout(binding = K_TEXTURE_0)
uniform sampler2D textureData;

void main()
{
  fColor = texture(textureData, vUv);
}
