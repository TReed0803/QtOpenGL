#include <GBuffer.ubo>
#include <Math.glsl>
out highp vec4 fColor;

void main()
{
  fColor = vec4(vec3(linearizeDepth(depth())), 1.0);
}
