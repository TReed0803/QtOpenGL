#include <GBuffer.ubo>
in vec2 vUvCoord;
out highp vec4 fColor;

void main()
{
  highp vec2 color = velocity(vUvCoord);
  fColor = vec4(color, 0.0, 1.0);
}
