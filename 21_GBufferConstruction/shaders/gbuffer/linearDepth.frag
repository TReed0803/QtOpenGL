#include <GBuffer.ubo>
out highp vec4 fColor;

void main()
{
  highp float d = depth();
  float ndcDepth =
    (2.0 * d - gl_DepthRange.near - gl_DepthRange.far) / gl_DepthRange.diff;
  float ndcDepthDiv = ndcDepth * 0.5 + 0.5;
  fColor = vec4(ndcDepth, ndcDepth, ndcDepth, 1.0);
}
