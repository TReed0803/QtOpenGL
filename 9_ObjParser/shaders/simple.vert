layout(location = 0) in highp vec3 position;
layout(location = 1) in highp vec3 color;
layout(location = 2) in highp vec3 normal;
out highp vec4 vNormal;

uniform mat4 modelToWorld;
uniform mat4 worldToCamera;
uniform mat4 cameraToView;

void main()
{
  gl_Position = cameraToView * worldToCamera * modelToWorld * vec4(position, 1.0);
  vNormal = vec4(normal,0.0);
}
