#include <GBuffer.ubo>
in vec2 vUvCoord;
out highp vec4 fColor;

void main()
{
  fColor = lightbuffer(vUvCoord);
}
