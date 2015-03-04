/*******************************************************************************
 * lighting/pointLight.vert
 *------------------------------------------------------------------------------
 * Pass-through shader that simply deferrs information to fragment shader.
 ******************************************************************************/

#include <GlobalBuffer.ubo>

// Per-Vertex Attribs
layout(location = 0) in highp vec3 position;

// Per-Instance Attribs
layout(location = 1) in highp vec3 lightViewPosition;
layout(location = 2) in highp vec4 lightAttenuation;
layout(location = 3) in highp vec3 lightDiffuse;
layout(location = 4) in highp vec3 lightSpecular;
layout(location = 5) in highp mat4 modelToClip;

// Output variables
flat out highp vec3 vLightViewPosition;
flat out highp vec4 vLightAttenuation;
flat out highp vec3 vLightDiffuse;
flat out highp vec3 vLightSpecular;

void main()
{
  // Pass Outputs
  vLightViewPosition = lightViewPosition;
  vLightAttenuation = lightAttenuation;
  vLightDiffuse = lightDiffuse;
  vLightSpecular = lightSpecular;

  // Send to Fragment Shader
  gl_Position = modelToClip * vec4(position, 1.0);
}
