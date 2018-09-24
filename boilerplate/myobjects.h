// Created by:
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

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <iterator>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

//using namespace std;
//using namespace glm;


bool loadTXT( 
	const char * path,
	std::vector<glm::vec3> & out_Spheres, 
	std::vector<glm::vec3> & out_Triangles, 
	std::vector<glm::vec3> & out_Planes, 
	std::vector<glm::vec3> & out_Lights 
);
bool loadOBJ(
    const char * path,
    std::vector < std::vector < glm::vec3 > >& out_vertices,
    std::vector < std::vector < glm::vec2 > >& out_uvs,
    std::vector < std::vector < glm::vec3 > >& out_normals,
    std::vector < unsigned int > & texture_names
);
