#include <GlobalMatrices.ubo>

// Per-Vertex Attribs
layout(location = 0) in highp vec3 position;

// Per-Instance Attribs
layout(location = 1) in highp vec3 lightCameraPosition;
layout(location = 2) in highp vec4 lightAttenuation;
layout(location = 3) in highp vec3 lightAmbient;
layout(location = 4) in highp vec3 lightDiffuse;
layout(location = 5) in highp vec3 lightSpecular;
layout(location = 6) in highp mat4 modelToView;

// Output variables
flat out highp vec3 vLightCameraPosition;
flat out highp vec4 vLightAttenuation;
flat out highp vec3 vLightAmbient;
flat out highp vec3 vLightDiffuse;
flat out highp vec3 vLightSpecular;

void main()
{
  // Pass Outputs
  vLightCameraPosition = lightCameraPosition;
  vLightAttenuation = lightAttenuation;
  vLightAmbient = lightAmbient;
  vLightDiffuse = lightDiffuse;
  vLightSpecular = lightSpecular;

  // Send to Fragment Shader
  gl_Position = modelToView * vec4(position, 1.0);
}
