#include <GBuffer.ubo>
out highp vec4 fColor;

void main()
{
  highp float d = depth();
  fColor = vec4(d, d, d, 1.0);
}
