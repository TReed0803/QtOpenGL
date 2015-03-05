/*******************************************************************************
 * lighting/pointLight.vert
 *------------------------------------------------------------------------------
 * Pass-through shader that simply deferrs information to fragment shader.
 ******************************************************************************/

#include <GlobalBuffer.ubo>

// Per-Vertex Attribs
layout(location = 0) in highp vec3 position;

// Per-Instance Attribs
layout(location = 1) in highp vec3 lightDirection;
layout(location = 2) in highp vec3 lightDiffuse;
layout(location = 3) in highp vec3 lightSpecular;

// Output variables
flat out highp vec3 vLightDirection;
flat out highp vec3 vLightDiffuse;
flat out highp vec3 vLightSpecular;

void main()
{
  // Pass Outputs
  vLightDirection = -lightDirection;
  vLightDiffuse   = lightDiffuse;
  vLightSpecular  = lightSpecular;

  // Send to Fragment Shader (FSQ)
  gl_Position = vec4(position, 1.0);
}
