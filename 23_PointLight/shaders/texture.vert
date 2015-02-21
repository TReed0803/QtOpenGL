const vec2 middle = vec2(0.5, 0.5);

layout(location = 0) in highp vec3 position;
out highp vec2 fTextureCoord;

void main()
{
  fTextureCoord = position.xy * middle + middle;
  gl_Position = vec4(position.xy, 0.0, 1.0);
}
