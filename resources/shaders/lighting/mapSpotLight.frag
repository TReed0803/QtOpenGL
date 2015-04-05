/*******************************************************************************
 * lighting/mapSpotLight.frag
 *------------------------------------------------------------------------------
 * Apply the lighting calculation to a given fragment of incident light.
 * Uses GBuffer information to access statistics about the scene itself.
 ******************************************************************************/

layout(location = 0) out float depth;

void main()
{
  depth = gl_FragCoord.z;
}
