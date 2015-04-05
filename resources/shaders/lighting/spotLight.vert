/*******************************************************************************
 * lighting/spotLight.vert
 *------------------------------------------------------------------------------
 * Pass-through shader that simply deferrs information to fragment shader.
 ******************************************************************************/

// Per-Vertex Attribs
layout(location = 0) in highp vec3 position;

// Per-Instance Attribs
layout(location = 1) in highp vec4 lightViewPosition;   // { vec3::Position, float::Inner }
layout(location = 2) in highp vec4 lightViewDirection;  // { vec3::Direction, float::Outer }
layout(location = 4) in highp vec4 lightAttenuation;    // { vec3::attenuation, float::radius }
layout(location = 5) in highp vec4 lightDiffuse;        // { vec3::Diffuse, float::Diff }
layout(location = 6) in highp vec3 lightSpecular;       // { vec3::Specular }
layout(location = 7) in highp mat4 modelToPersp;        // { mat4::modelToPersp }

// Output variables
out LightData
{
  flat vec3 ViewPosition;
  flat vec3 ViewDirection;
  flat vec4 Attenuation;
  flat vec3 Diffuse;
  flat vec3 Specular;
  flat float InnerAngle;
  flat float OuterAngle;
  flat float DiffAngle;
} Light;

void main()
{
  // Pass Outputs
  Light.ViewPosition = lightViewPosition.xyz;
  Light.ViewDirection = lightViewDirection.xyz;
  Light.Attenuation = lightAttenuation;
  Light.Diffuse = lightDiffuse.xyz;
  Light.Specular = lightSpecular;
  Light.InnerAngle = lightViewPosition.w;
  Light.OuterAngle = lightViewDirection.w;
  Light.DiffAngle = lightDiffuse.w;

  // Send to Fragment Shader
  gl_Position = modelToPersp * vec4(position, 1.0);
}
