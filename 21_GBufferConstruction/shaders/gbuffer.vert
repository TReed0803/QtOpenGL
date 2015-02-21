#include <GlobalMatrices.ubo>

layout(location = 0) in highp vec3 position;
layout(location = 1) in highp vec3 normal;
layout(location = 2) in highp mat4 modelToWorld;
layout(location = 6) in highp mat4 prevModelToWorld;
layout(location = 10) in highp vec3 diffuse;
layout(location = 11) in highp vec4 specular;

out highp vec3 vNormal;
out highp vec4 vPrevPosition;
out highp vec4 vCurrPosition;
out highp vec3 vDiffuse;
out highp vec4 vSpecular;

void main()
{
  // Calculations
  highp mat4 InverseTranspose = transpose(inverse(worldToCamera * modelToWorld));
  highp vec4 currViewPos = worldToView * modelToWorld * vec4(position, 1.0);
  highp vec4 prevViewPos = prevWorldToView * prevModelToWorld * vec4(position, 1.0);
  highp vec4 cameraNormal = InverseTranspose * vec4(normal, 1.0);

  // Outputs
  vNormal = cameraNormal.xyz;
  vPrevPosition = prevViewPos;
  vCurrPosition = currViewPos;
  vDiffuse = diffuse;
  vSpecular = specular;

  // Final position
  gl_Position = currViewPos;
}
