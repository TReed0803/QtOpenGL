#include <GBuffer.ubo>
out highp vec4 fColor;

void main()
{
  fColor = vec4(velocity(), 0.0, 1.0);
}
