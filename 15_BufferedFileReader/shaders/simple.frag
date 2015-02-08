in highp vec3 vColor;
out highp vec4 fColor;

void main()
{
  highp vec3 N = normalize(vColor);
  fColor = vec4(N,1.0);
}
