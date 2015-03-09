#include <GBuffer.ubo>
out highp vec4 fColor;

void main()
{
  fColor = vec4(vec3(specular().w), 1.0);
}
