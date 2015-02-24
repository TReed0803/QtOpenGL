/*******************************************************************************
 * EncodeDecode.glsl
 *------------------------------------------------------------------------------
 * Provides all of the capabilities to encode and decode data from the GBuffer.
 ******************************************************************************/

#ifndef   ENCODEDECODE_GLSL
#define   ENCODEDECODE_GLSL

highp vec2 encodeNormal(highp vec3 n)
{
  highp vec2 encN = normalize(n.xy) * sqrt(-n.z * 0.5 + 0.5);
  return (encN * 0.5 + 0.5);
}

highp vec3 decodeNormal(highp vec2 n)
{
  highp vec4 decN = vec4(n, 0.0, 0.0) * 2.0 + vec4(-1.0, -1.0, 1.0, -1.0);
  highp float len = dot(decN.xyz, -decN.xyw);
  decN.z = len;
  decN.xy *= sqrt(len);
  return decN.xyz * 2.0 + vec3(0.0, 0.0, -1.0);
}

highp float encodeSpecularColor(highp vec3 s)
{
  return (s.x + s.y + s.z) / 3.0;
}

highp vec3 decodeSpecularColor(highp float s)
{
  return vec3(s, s, s);
}

highp float encodeSpecularExponent(highp float s)
{
  return s / 255.0;
}

highp float decodeSpecularExponent(highp float s)
{
  return s * 255.0;
}

#endif // ENCODEDECODE_GLSL
