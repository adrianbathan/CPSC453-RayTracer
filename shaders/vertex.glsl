// ==========================================================================
// Vertex program for barebones GLFW boilerplate
//
// Author:  Sonny Chan, University of Calgary
// Modified by:
//			Adrian Bathan, University of Calgary (30011953)
// Date:    December 2015
// Modified on: March 28, 2018
// Citation (for all files):
//		SSBO (shader storage buffer object)
//			https://www.khronos.org/opengl/wiki/Shader_Storage_Buffer_Object
//			http://www.geeks3d.com/20140704/tutorial-introduction-to-opengl-4-3-shader-storage-buffers-objects-ssbo-demo/
//		THIS PROGRAM USES OPENGL 4.3 #defines taken from
//			https://github.com/cloudwu/ejoy3d/blob/master/3rd/glad/include/glad/glad.h#L1826
//		Object Loader
//			http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/
// ==========================================================================

#version 430
// location indices for these attributes correspond to those specified in the
// InitializeGeometry() function of the main program
layout(location = 0) in vec2 VertexPosition;
layout(location = 1) in vec3 VertexColour;
layout(location = 2) in vec2 UVPos;
// output to be interpolated between vertices and passed to the fragment stage
out vec3 Colour;
out vec2 UV;

out vec2 pixelPos;

uniform mat4 MVP;

void main()
{
    // assign vertex position without modification
//    gl_Position = vec4(VertexPosition, 0.0, 1.0);
    gl_Position = /*MVP **/ vec4(VertexPosition, 0.0, 1.0);

    // assign output colour to be interpolated
    Colour = VertexColour;
    UV = UVPos;
    pixelPos = VertexPosition;
}
