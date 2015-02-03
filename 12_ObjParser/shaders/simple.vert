layout(location = 0) in highp vec3 position;
layout(location = 1) in highp vec3 color;
out highp vec4 vColor;

uniform mat4 modelToWorld;
uniform mat4 worldToCamera;
uniform mat4 cameraToView;

void main()
{
  gl_Position = cameraToView * worldToCamera * modelToWorld * vec4(position, 1.0);
  vColor = vec4(color,1.0);
}
