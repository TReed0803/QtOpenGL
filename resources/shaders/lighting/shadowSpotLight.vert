/*******************************************************************************
 * lighting/shadowSpotLight.vert
 *------------------------------------------------------------------------------
 * Pass-through shader that simply deferrs information to fragment shader.
 ******************************************************************************/

#include <LightBuffer.ubo>

// Per-Vertex Attribs
layout(location = 0) in highp vec3 position;

// Output variables
out highp mat4 vViewToLightBias;

void main()
{
  const mat4 ShadowBiasMatrix = mat4(
    0.5, 0.0, 0.0, 0.0,
    0.0, 0.5, 0.0, 0.0,
    0.0, 0.0, 0.5, 0.0,
    0.5, 0.5, 0.5, 1.0
  );

  // Send to Fragment Shader
  vViewToLightBias = ShadowBiasMatrix * Light.ViewToLightPersp;
  gl_Position = Light.ModelToPersp * vec4(position, 1.0);
}
