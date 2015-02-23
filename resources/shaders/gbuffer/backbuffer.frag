#include <GBuffer.ubo>
out highp vec4 fColor;

void main()
{
  vec2 fragCoord = uvCoord();
  vec4 gamma = vec4(1.0/2.2);

  // Lets try to motion blur it
  vec2 texelSize = 1.0 / vec2(textureSize(backbufferTexture, 0));
  vec2 v = velocity();
  float speed = length(v / texelSize);
  float nSamples = clamp(int(speed), 1, 20);
  fColor = pow(backbuffer() + lightbuffer(), gamma);
  for (int i = 1; i < nSamples; ++i) {
     vec2 offset = -v * (float(i) / float(nSamples - 1) - 0.5);
     fColor += pow(backbufferCoord(fragCoord + offset) + lightbufferCoord(fragCoord + offset), gamma);
  }
  fColor /= float(nSamples);
}
