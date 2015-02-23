#include <GBuffer.ubo>
out highp vec4 fColor;

void main()
{
  fColor = vec4(worldPosition(), 1.0);
}
