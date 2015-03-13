#include <GBuffer.ubo>
out highp vec4 fColor;

void main()
{
  highp vec2 fragCoord = uvCoord();
  highp vec2 texelSize = 1.0 / vec2(textureSize(lightbufferTexture, 0));
  highp vec2 v = velocity();
  highp float speed = length(v / texelSize);
  highp int nSamples = clamp(int(speed), 1, 20);
  highp vec3 color = lightbuffer().xyz;
  for (highp int i = 1; i < nSamples; ++i) {
     highp vec2 offset = -v * (float(i) / float(nSamples - 1) - 0.5);
     color += lightbuffer(fragCoord + offset).xyz;
  }
  color /= float(nSamples);
  fColor = vec4(color, 1.0);
}
