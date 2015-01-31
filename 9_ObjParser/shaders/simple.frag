in highp vec4 vNormal;
out highp vec4 fColor;

void main()
{
  highp vec3 N = normalize(vNormal.xyz);
  fColor = vec4(N,1.0);
}
