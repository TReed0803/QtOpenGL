#include <GlobalMatrices.ubo>

layout(location = 0) in highp vec3 position;
layout(location = 1) in highp vec3 color;
layout(location = 2) in highp mat4 modelToWorld;
uniform mat4 uModelToWorld;

out highp vec3 vColor;

void main()
{
  gl_Position = worldToView * modelToWorld * vec4(position, 1.0);
  vColor = abs(color);
}
