#include <GBuffer.ubo>
in vec2 vUvCoord;
out highp vec4 fColor;

void main()
{
  highp vec3 color = lightbuffer(vUvCoord).xyz;
  color = pow(color, vec3(1.0 / 2.2));
  fColor = vec4(color, 1.0);
}
