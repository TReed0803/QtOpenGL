/*******************************************************************************
 * debug/screen.frag
 *------------------------------------------------------------------------------
 * Passhtrough shader, outputs color to fColor.
 ******************************************************************************/

in highp vec4 vColor;
layout(location = 0) out highp vec4 fColor;

void main()
{
  fColor = vColor;
}
