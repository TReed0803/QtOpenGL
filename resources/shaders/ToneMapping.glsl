vec3 Reinhard(vec3 color)
{
  return color / (color + vec3(1.0));
}

vec3 HejlDawson(vec3 color)
{
  vec3 x = max(vec3(0.0),color-vec3(0.004));
  return (x*(6.2*x+.5))/(x*(6.2*x+1.7)+0.06);
}

vec3 _Uncharted(vec3 x)
{
  const float A = 0.15;
  const float B = 0.50;
  const float C = 0.10;
  const float D = 0.20;
  const float E = 0.02;
  const float F = 0.30;
  const float W = 11.2;
  return ((x*(A*x+C*B)+D*E)/(x*(A*x+B)+D*F))-E/F;
}

vec3 Uncharted(vec3 color)
{
  const float W = 11.2;
  const float ExposureBias = 2.0f;
  return _Uncharted(ExposureBias*color) / _Uncharted(vec3(W));
}