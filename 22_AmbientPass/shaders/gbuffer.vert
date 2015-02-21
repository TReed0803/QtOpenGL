#include <GlobalMatrices.ubo>

// Per-Vertex Attributes
layout(location = 0)  in highp vec3 position;
layout(location = 1)  in highp vec3 normal;

// Per-Instance Attributes
layout(location = 2)  in highp vec3 diffuse;
layout(location = 3)  in highp vec4 specular;
layout(location = 4)  in highp mat4 currModelTransform;
layout(location = 8)  in highp mat4 prevModelTransform;
layout(location = 12) in highp mat4 normalTransform;

out highp vec3 vNormal;
out highp vec4 vPrevPosition;
out highp vec4 vCurrPosition;
out highp vec3 vDiffuse;
out highp vec4 vSpecular;
out highp vec4 cameraPosition;

void main()
{
  // Calculations
  highp vec4 currViewPos  = currModelTransform * vec4(position, 1.0);
  highp vec4 prevViewPos  = prevModelTransform * vec4(position, 1.0);
  highp vec4 cameraNormal =    normalTransform * vec4(normal  , 1.0);

  // Outputs
  vNormal = cameraNormal.xyz;
  vPrevPosition = prevViewPos;
  vCurrPosition = currViewPos;
  vDiffuse = diffuse;
  vSpecular = specular;
  cameraPosition = viewToCamera * currViewPos;

  // Final position
  gl_Position = currViewPos;
}
