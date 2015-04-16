/*******************************************************************************
 * Bindings.glsl
 *------------------------------------------------------------------------------
 * Defines all of the binding locations for GLSL uniforms.
 ******************************************************************************/

#ifndef BINDINGS_GLSL
#define BINDINGS_GLSL

// Samplers
#define K_TEXTURE_0                     0
#define K_TEXTURE_1                     1
#define K_TEXTURE_2                     2
#define K_DEPTH_TEXTURE_BINDING         10
#define K_GEOMETRY_TEXTURE_BINDING      11
#define K_MATERIAL_TEXTURE_BINDING      12
#define K_PHYSICAL_TEXTURE_BINDING      13
#define K_SURFACE_TEXTURE_BINDING       14
#define K_LIGHT_BUFFER_TEXTURE_BINDING  15
#define K_AMBIENT_OCCLUSION_BINDING     16

// Uniform Blocks
#define K_CURRENT_VIEW_BINDING  1
#define K_PREVIOUS_VIEW_BINDING 2
#define K_LIGHT_BINDING         3
#define K_MATERIAL_BINDING      4
#define K_OBJECT_BINDING        5
#define K_HAMMERSLEY_BINDING    6
#define K_BLUR_BINDING          7

#endif // BINDINGS_GLSL
