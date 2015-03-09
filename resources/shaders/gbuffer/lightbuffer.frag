#include <GBuffer.ubo>
out highp vec4 fColor;

void main()
{
  fColor = vec4(lightbuffer().xyz, 1.0);
}
