#include <GBuffer.ubo>
out highp vec4 fColor;

void main()
{
  // Lets try to motion blur it
  vec2 texelSize = 1.0 / vec2(textureSize(backbufferTexture, 0));
  vec2 v = 2.0 * velocity();
  float speed = length(v / texelSize);
  float nSamples = clamp(int(speed), 1, 20);
  fColor = backbuffer();
  for (int i = 1; i < nSamples; ++i) {
     vec2 offset = -v * (float(i) / float(nSamples - 1) - 0.5);
     fColor += backbufferCoord(fFragCoord + offset);
  }
  fColor /= float(nSamples);
}
