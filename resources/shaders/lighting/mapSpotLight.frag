/*******************************************************************************
 * lighting/pointLight.frag
 *------------------------------------------------------------------------------
 * Apply the lighting calculation to a given fragment of incident light.
 * Uses GBuffer information to access statistics about the scene itself.
 ******************************************************************************/

void main()
{
  gl_FragDepth = gl_FragCoord.z;
}
