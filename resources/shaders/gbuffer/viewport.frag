#include <GBuffer.ubo>
#include <Math.glsl>
#include <GlobalBuffer.ubo>
#include <ToneMapping.glsl>

in vec2 vUvCoord;
out highp vec4 fColor;



void main()
{
  highp vec3 color = lightbuffer(vUvCoord).xyz;
  color *= Current.Exposure;

  //*
  color = Uncharted(color);
  //*/
  /*
  color = Reinhard(color);
  //*/

  color = l2rgb(color);
  fColor = vec4(color, 1.0);
}
