/*******************************************************************************
 * lighting/pointLight.frag
 *------------------------------------------------------------------------------
 * Apply the lighting calculation to a given fragment of incident light.
 * Uses GBuffer information to access statistics about the scene itself.
 ******************************************************************************/

// Light Output
layout(location = 0) out highp float fMapDepth;

void main()
{
  fMapDepth = gl_FragCoord.z;
}
