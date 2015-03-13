layout(location = 0) in highp vec3 position;

uniform mat4 ViewportReposition;
out vec2 vUvCoord;

void main()
{
  vUvCoord = position.xy / 2.0 + 0.5;
  gl_Position = vec4(position.xyz, 1.0);
}
