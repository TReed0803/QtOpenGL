/*******************************************************************************
 * lighting/mapSpotLight.vert
 *------------------------------------------------------------------------------
 * Pass-through shader that simply deferrs information to fragment shader.
 ******************************************************************************/

#include <GlobalBuffer.ubo>
#include <LightBuffer.ubo>
#include <Object.ubo>

// Per-Vertex Attribs
layout(location = 0) in highp vec3 position;

void main()
{
  // Send to Fragment Shader
  gl_Position = Light.ViewToLightPersp * Object.CurrentModelToView * vec4(position, 1.0);
}
