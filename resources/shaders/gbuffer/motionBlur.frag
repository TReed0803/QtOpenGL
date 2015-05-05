#include <GBuffer.ubo>
#include <GlobalBuffer.ubo>
out highp vec4 fColor;
uniform int MaxSamples = 20;
uniform float Power = 1.0;

void main()
{
  // Calculate Initial Color
  highp vec3 color = lightbuffer().xyz;

  // Calculate the sample size and velocity
  highp vec2 texelSize = 1.0 / vec2(Current.Dimensions);
  highp vec2 vel = Power * velocity();
  // Note: vel *= currFps / targetFps; For more accuracy
  highp float speed = length(vel / texelSize);
  highp int nSamples = clamp(int(speed), 1, MaxSamples);

  // Calculate the Iterative motion blur
  highp vec2 adder = vel / float(nSamples);
  highp vec2 fragCoord = uvCoord() - vel * vec2(0.5);

  // Apply the motion blur
  for (highp int i = 1; i < nSamples; ++i) {
    color += lightbuffer(fragCoord).xyz;
    fragCoord += adder;
  }
  color /= float(nSamples);

  // Set the final color
  fColor = vec4(color, 1.0);
}
