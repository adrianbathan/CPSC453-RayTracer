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

#include "myobjects.h"

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
)
{
	FILE * file = fopen(path, "r");
	if( file == NULL ){
		printf("Impossible to open the file !\n");
		return false;
	}
	int count = 0;
	while( 1 ){

	char lineHeader[128];
	
	char useless[128];	
	std::string useless0;

	int res = fscanf(file, "%s", lineHeader);
	if (res == EOF)
		break; // EOF = End Of File. Quit the loop.
	if ( strcmp( lineHeader, "#" ) == 0 ) {
		std::fgets(useless, 128, file);
		continue;
	}
	if ( strcmp( lineHeader, "sphere" ) == 0 || strcmp( lineHeader, "sphere{" ) == 0 ){
		if (strcmp( lineHeader, "sphere" ) == 0 )
			fscanf(file, "%s", useless);	
		for (int i=0; i<4; i++) {
			glm::vec3 spherevertex;
			fscanf(file, "%f %f %f\n", &spherevertex.x, &spherevertex.y, &spherevertex.z );
			out_Spheres.push_back(spherevertex);
		}
	}
	else if ( strcmp( lineHeader, "triangle" ) == 0 || strcmp( lineHeader, "triangle{" ) == 0 ) {
		if (strcmp( lineHeader, "triangle" ) == 0 )
			fscanf(file, "%s", useless);
		for (int i=0; i<5; i++) {
			glm::vec3 trivertex;
			fscanf(file, "%f %f %f\n", &trivertex.x, &trivertex.y, &trivertex.z );
			out_Triangles.push_back(trivertex);
		}
	}
	else if ( strcmp( lineHeader, "plane" ) == 0 || strcmp( lineHeader, "plane{" ) == 0 ) {
		if (strcmp( lineHeader, "plane" ) == 0 )
			fscanf(file, "%s", useless);
		for (int i=0; i<4; i++) {
			glm::vec3 planevertex;
			fscanf(file, "%f %f %f\n", &planevertex.x, &planevertex.y, &planevertex.z );
			out_Planes.push_back(planevertex);
		}
	}
	else if ( strcmp( lineHeader, "light" ) == 0 || strcmp( lineHeader, "light{" ) == 0 ) {
		if (strcmp( lineHeader, "light" ) == 0 )
			fscanf(file, "%s", useless);
		for (int i=0; i<2; i++) {
			glm::vec3 lightvertex;
			fscanf(file, "%f %f %f\n", &lightvertex.x, &lightvertex.y, &lightvertex.z );
			out_Lights.push_back(lightvertex);
		}
	}
	else {}

}
	
	return true;
}
bool loadOBJ(
    const char * path,
    std::vector < std::vector < glm::vec3 > > & out_vertices,
    std::vector <std::vector < glm::vec2 > > & out_uvs,
    std::vector <std::vector < glm::vec3 > > & out_normals,
    std::vector < unsigned int > & texture_names
)
{
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;
	
	std::vector< std::vector < unsigned int > > objectvIndices,objectuvIndices,objectnIndices;

	FILE * file = fopen(path, "r");
	if( file == NULL ){
		printf("Impossible to open the file !\n");
		return false;
	}
	while( 1 ){

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.
//		else
//			count++;
		// else : parse lineHeader
		
		if ( strcmp( lineHeader, "v" ) == 0 ){
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
			temp_vertices.push_back(vertex);
		
		}
		else if ( strcmp( lineHeader, "vt" ) == 0 ){
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y );
			temp_uvs.push_back(uv);
		
		}
		else if ( strcmp( lineHeader, "vn" ) == 0 ){
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			temp_normals.push_back(normal);
		
		}
		else if ( strcmp( lineHeader, "f" ) == 0 ){
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
//			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
//			int matches = fscanf(file, "%d/%d %d/%d %d/%d\n", &vertexIndex[0], &uvIndex[0], &vertexIndex[1], &uvIndex[1], &vertexIndex[2], &uvIndex[2]);
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );			
			if (matches != 9){
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
//				printf("%d/%d/%d %d/%d/%d %d/%d/%d\n", vertexIndex[0], uvIndex[0], normalIndex[0], vertexIndex[1], uvIndex[1], normalIndex[1], vertexIndex[2], uvIndex[2], normalIndex[2] );
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices    .push_back(uvIndex[0]);
			uvIndices    .push_back(uvIndex[1]);
			uvIndices    .push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
		else if ( strcmp( lineHeader, "usemtl" ) == 0) {// && vertexIndices.size() > 0){
			if (vertexIndices.size()>0) {
				objectvIndices.push_back(vertexIndices);
				objectuvIndices.push_back(uvIndices);
				objectnIndices.push_back(normalIndices);
				vertexIndices.clear();
				uvIndices.clear();
				normalIndices.clear();
			}
			unsigned int texID;
			int matches = fscanf(file, "%d\n", &texID);
			texture_names.push_back(texID);
		}
	}
	if (vertexIndices.size()>0) {
		objectvIndices.push_back(vertexIndices);
		objectuvIndices.push_back(uvIndices);
		objectnIndices.push_back(normalIndices);
		vertexIndices.clear();
		uvIndices.clear();
		normalIndices.clear();
	}
	
	std::vector< glm::vec3 > tempv;
	std::vector< glm::vec2 > tempu;
	std::vector< glm::vec3 > tempn;
	for(unsigned int i=0; i<objectvIndices.size(); i++) {
		for(unsigned int j=0; j<objectvIndices[i].size(); j++) {
			unsigned int objvIndex = objectvIndices[i][j];
			unsigned int objuvIndex = objectuvIndices[i][j];
			unsigned int objnIndex = objectnIndices[i][j];
			glm::vec3 v = temp_vertices[ objvIndex-1 ];
			glm::vec2 u = temp_uvs[ objuvIndex-1 ];
			glm::vec3 n = temp_normals[ objnIndex-1 ];
			tempv.push_back(v);
			tempu.push_back(u);
			tempn.push_back(n);
//			std::cout << objvIndex << "/" << objuvIndex << "/" << objnIndex << std::endl;
		}
//		std::cout<<objectvIndices.size()<< ":" << objectuvIndices.size()<<std::endl;
		out_vertices.push_back(tempv);
		out_uvs.push_back(tempu);
		out_normals.push_back(tempn);
		tempv.clear();
		tempu.clear();
		tempn.clear();
	}
	fclose(file);
	return true;
}
