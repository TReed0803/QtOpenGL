#include <GlobalMatrices.ubo>

layout(location = 0) in highp vec3 position;
layout(location = 1) in highp vec3 color;
uniform mat4 modelToWorld;

out highp vec3 vColor;

void main()
{
  gl_Position = cameraToView * worldToCamera * modelToWorld * vec4(position, 1.0);
  vColor = abs(color);
}
