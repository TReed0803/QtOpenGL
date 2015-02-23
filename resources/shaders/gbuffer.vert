#include <GlobalBuffer.ubo>

// Per-Vertex Attributes
layout(location = 0)  in vec3 position;
layout(location = 1)  in vec3 normal;

// Per-Instance Attributes
layout(location = 2)  in vec3 diffuse;
layout(location = 3)  in vec4 specular;
layout(location = 4)  in mat4 currModelToCamera;
layout(location = 8)  in mat4 prevModelToCamera;
layout(location = 12) in mat4 normalTransform;

// Framebuffer Outputs
out vec3 vCameraNormal;
out vec4 vCurrCameraPosition;
out vec4 vCurrViewPosition;
out vec4 vPrevViewPosition;
flat out vec3 vDiffuse;
flat out vec4 vSpecular;

void main()
{
  // Calculations
  vec4 currCameraPos  = currModelToCamera * vec4(position, 1.0);
  vec4 prevCameraPos  = prevModelToCamera * vec4(position, 1.0);
  vec4 cameraNormal   =   normalTransform * vec4(normal  , 1.0);

  // Outputs
  vCameraNormal = cameraNormal.xyz;
  vCurrCameraPosition = currCameraPos;
  vCurrViewPosition = cameraToView * currCameraPos;
  vPrevViewPosition = cameraToView * prevCameraPos;
  vDiffuse = diffuse;
  vSpecular = specular;

  // Final position
  gl_Position = vCurrViewPosition;
}
