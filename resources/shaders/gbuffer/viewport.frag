#include <GBuffer.ubo>
in vec2 vUvCoord;
out highp vec4 fColor;

void main()
{
  fColor = vec4(pow(lightbuffer(vUvCoord).xyz, vec3(1.0/2.2)), 1.0);
}
