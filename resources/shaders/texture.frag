uniform sampler2D texture;

in highp vec2 fTextureCoord;
layout(location = 0) out highp vec4 fColor;

void main()
{
  fColor = texture2D(texture, fTextureCoord);
}
