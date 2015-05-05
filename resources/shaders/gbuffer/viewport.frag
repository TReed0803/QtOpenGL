#include <GBuffer.ubo>
#include <Math.glsl>
#include <GlobalBuffer.ubo>

in vec2 vUvCoord;
out highp vec4 fColor;

// Tone Mapping Settings
uniform float A = 0.15;
uniform float B = 0.50;
uniform float C = 0.10;
uniform float D = 0.20;
uniform float E = 0.02;
uniform float F = 0.30;
uniform float W = 11.2;
uniform float Exposure = 1.0;
uniform float ExposureBias = 2.0;

vec3 _ToneMap(vec3 x)
{
  return ((x*(A*x+C*B)+D*E)/(x*(A*x+B)+D*F))-E/F;
}

vec3 ToneMap(vec3 color)
{
  return _ToneMap(ExposureBias*color) / _ToneMap(vec3(W));
}

void main()
{
  highp vec3 color = lightbuffer(vUvCoord).xyz;
  color *= Exposure;
  color = ToneMap(color);
  color = l2rgb(color);
  fColor = vec4(color, 1.0);
}
