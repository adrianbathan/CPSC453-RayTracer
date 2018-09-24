// ==========================================================================
// Barebones OpenGL Core Profile Boilerplate
//    using the GLFW windowing system (http://www.glfw.org)
//
// Loosely based on
//  - Chris Wellons' example (https://github.com/skeeto/opengl-demo) and
//  - Camilla Berglund's example (http://www.glfw.org/docs/latest/quick.html)
//
// Author:  Sonny Chan, University of Calgary
// Co-Authors:
//			Jeremy Hart, University of Calgary
//			John Hall, University of Calgary
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
//		Screenshot in openGL (since i'm not using sonny's image buffer
//			http://www.david-amador.com/2012/09/how-to-take-screenshot-in-opengl/
// ==========================================================================

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <iterator>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "texture.h"
#include "geometry.h"
#include "myobjects.h"
//#include "imagebuffer.h"

#define GL_SHADER_STORAGE_BUFFER 0x90D2
#define GL_SHADER_STORAGE_BUFFER_BINDING 0x90D3
#define GL_SHADER_STORAGE_BUFFER_START 0x90D4
#define GL_SHADER_STORAGE_BUFFER_SIZE 0x90D5
#define GL_MAX_VERTEX_SHADER_STORAGE_BLOCKS 0x90D6
#define GL_MAX_GEOMETRY_SHADER_STORAGE_BLOCKS 0x90D7
#define GL_MAX_TESS_CONTROL_SHADER_STORAGE_BLOCKS 0x90D8
#define GL_MAX_TESS_EVALUATION_SHADER_STORAGE_BLOCKS 0x90D9
#define GL_MAX_FRAGMENT_SHADER_STORAGE_BLOCKS 0x90DA
#define GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS 0x90DB
#define GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS 0x90DC
#define GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS 0x90DD
#define GL_MAX_SHADER_STORAGE_BLOCK_SIZE 0x90DE
#define GL_SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT 0x90DF

using namespace std;
using namespace glm;
// --------------------------------------------------------------------------
// OpenGL utility and support function prototypes

void QueryGLVersion();
bool CheckGLErrors();

string LoadSource(const string &filename);
GLuint CompileShader(GLenum shaderType, const string &source);
GLuint LinkProgram(GLuint vertexShader, GLuint fragmentShader);

// --------------------------------------------------------------------------
// Functions to set up OpenGL shader programs for rendering

// load, compile, and link shaders, returning true if successful
GLuint InitializeShaders()
{
	// load shader source from files
	string vertexSource = LoadSource("shaders/vertex.glsl");
	string fragmentSource = LoadSource("shaders/fragment.glsl");
	if (vertexSource.empty() || fragmentSource.empty()) return false;

	// compile shader source into shader objects
	GLuint vertex = CompileShader(GL_VERTEX_SHADER, vertexSource);
	GLuint fragment = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

	// link shader program
	GLuint program = LinkProgram(vertex, fragment);

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	// check for OpenGL errors and return false if error occurred
	return program;
}


GLuint program;
MyTexture fbo;
char* scenefiles[4] = {"scene1.txt" , "scene2.txt", "scene4.txt", "scene3.txt"};
int mode = 1;
double dx=0, dy=0, dz=1, thx=0, thy=0;
float fov = -1.4f;
double xclick = 0, yclick = 0, oldx=0, oldy=0;
bool lclickdown = false;
vec3 position = vec3(0,0,1);
int current = 0;
int pupilradius;
float pupil = 0.05f;

vector<vec2> points;
vector<vec2> uvs;
bool goforward = false, backward = false, strafeLeft = false, strafeRight = false;
bool flyUp = false, flyDown = false;

char* pics[36] = {	"shimakaze/on_SHIMAKAZE_v090/sima_face_all.tga",
					"shimakaze/on_SHIMAKAZE_v090/sima_face_all.tga",
					"shimakaze/on_SHIMAKAZE_v090/new_body.png",
					"shimakaze/on_SHIMAKAZE_v090/sima_face_all.tga",
					"shimakaze/on_SHIMAKAZE_v090/mayu01.tga",
					"shimakaze/on_SHIMAKAZE_v090/simakaze_ribbon.png",
					
					"shimakaze/on_SHIMAKAZE_v090/eye_LA.png",
					"shimakaze/on_SHIMAKAZE_v090/eye_LB.png",
					"shimakaze/on_SHIMAKAZE_v090/eye_LC.png",
					"shimakaze/on_SHIMAKAZE_v090/eye_RA.png",
					"shimakaze/on_SHIMAKAZE_v090/eye_RB.png",
					"shimakaze/on_SHIMAKAZE_v090/eye_RC.png",
					
					"shimakaze/on_SHIMAKAZE_v090/hair_B.tga",
					"shimakaze/on_SHIMAKAZE_v090/hair_F.tga",
					"shimakaze/on_SHIMAKAZE_v090/hair_F.tga",
					"shimakaze/on_SHIMAKAZE_v090/hair_F.tga",
					"shimakaze/on_SHIMAKAZE_v090/simakaze_top_skt.png",
					"shimakaze/on_SHIMAKAZE_v090/simakaze_eri.png",
					
					"shimakaze/on_SHIMAKAZE_v090/simakaze_top_skt.png",
					"shimakaze/on_SHIMAKAZE_v090/simakaze_top_skt.png",
					"shimakaze/on_SHIMAKAZE_v090/simakaze_glove.png",
					"shimakaze/on_SHIMAKAZE_v090/simakaze_sox.png",
					"shimakaze/on_SHIMAKAZE_v090/simakaze_ribbon.png",
					"shimakaze/on_SHIMAKAZE_v090/sima_face_all.tga",
					
					"shimakaze/on_SHIMAKAZE_v090/sima_face_all.tga",
					"shimakaze/on_SHIMAKAZE_v090/sima_face_all.tga",
					"shimakaze/on_SHIMAKAZE_v090/sima_face_all.tga",
					"shimakaze/on_SHIMAKAZE_v090/sima_face_all.tga",
					"shimakaze/on_SHIMAKAZE_v090/eyeparts.png",
					"shimakaze/on_SHIMAKAZE_v090/0siki_5ren.png",
					
					"shimakaze/on_SHIMAKAZE_v090/0siki_5ren_cover.png",
					"shimakaze/on_SHIMAKAZE_v090/simakaze_parts.png",
					"shimakaze/on_SHIMAKAZE_v090/hair_S.tga",
					"shimakaze/on_SHIMAKAZE_v090/simakaze_boots.png",
					"shimakaze/on_SHIMAKAZE_v090/hair_B.tga",
					"shimakaze/on_SHIMAKAZE_v090/hair_S.tga" };
					
char* wallName0 = "hoppo.jpg";
char* wallName1 = "zzz.jpg";
float texka = 1.0f, texkd = 0.0f, texks = 0.0f;					
MyTexture shimaTex[36];
MyTexture wallTex0;
MyTexture wallTex1;

// --------------------------------------------------------------------------
// Rendering function that draws our scene to the frame buffer
vector<vec3> allSpheres;
vector<vec3> allTriangles;
vector<vec3> allPlanes;
vector<vec3> allLights;

struct Sphere{
	vec3 center;
	float radius;
	vec3 colour;
	float p;
	float ks;
	float kd;
	float kr;
	float pad0;
};
struct Triangle{
	vec3 p0;
	float pad0;
	vec3 p1;
	float pad1;
	vec3 p2;
	float pad2;
	vec3 colour;
	float pad3;
	float p;	//
	float ks;
	float kd;
	float kr;
};
struct Plane{
	vec3 normal;
	float pad0;
	vec3 point;
	float pad1;
	vec3 colour;
	float pad2;
	float p;
	float ks;
	float kd;
	float kr;
};

struct TextTri{
	vec3 p0;
	float pad0;
	vec3 p1;
	float pad1;
	vec3 p2;
	float pad2;
	vec2 uv0;
	vec2 pad3;
	vec2 uv1;
	vec2 pad4;
	vec2 uv2;
	vec2 pad5;
//	float pad4;
	vec3 vn;
	int textureIndex;
	float p;	//
	float ks;
	float kd;
	float kr;
};

struct Light{
	vec3 pos;
	float pad0;
	vec3 colour;
	float pad1;
};


vector<Sphere> mySph;
vector<Triangle> myTri;
vector<Plane> myPla;
vector<Light> myLig;
vector<TextTri> myShimakaze;
	
void MakeWorld(){
	bool res0 = loadTXT(scenefiles[mode-1], allSpheres, allTriangles, allPlanes, allLights);
	vec3 temp;
	string uniformName;
	for (unsigned i = 0; i < allSpheres.size(); i=i+4){
		Sphere sph;
		if (mode == 4) {
			sph.center = allSpheres[i]/2.0f;
			sph.radius = allSpheres[i+1].x/2.0f;
		}
		else {
			sph.center = allSpheres[i];
			sph.radius = allSpheres[i+1].x;
		}
		sph.kr = allSpheres[i+1].y;
		sph.colour = allSpheres[i+2];
		sph.p = allSpheres[i+3].x;
		sph.ks = allSpheres[i+3].y;
		sph.kd = allSpheres[i+3].z;
		mySph.push_back(sph);
	}
	for (unsigned i = 0; i < allTriangles.size(); i=i+5){
		Triangle tri;
		if (mode == 4) {
			tri.p0 = allTriangles[i]/2.0f;
			tri.p1 = allTriangles[i+1]/2.0f;
			tri.p2 = allTriangles[i+2]/2.0f;
		}
		else {
			tri.p0 = allTriangles[i];
			tri.p1 = allTriangles[i+1];
			tri.p2 = allTriangles[i+2];
		}
		tri.colour = allTriangles[i+3];
		tri.p = allTriangles[i+4].x;
		tri.ks = allTriangles[i+4].y;
		tri.kd = allTriangles[i+4].z;
		myTri.push_back(tri);		
	}
		
	for (unsigned i = 0; i < allPlanes.size(); i=i+4){
		Plane pla;
		if (mode == 4) {
			pla.normal = allPlanes[i]/2.0f;
			pla.point = allPlanes[i+1]/2.0f;
		}
		else {
			pla.normal = allPlanes[i];
			pla.point = allPlanes[i+1];
		}
		pla.colour = allPlanes[i+2];
		pla.p = allPlanes[i+3].x;
		pla.ks = allPlanes[i+3].y;
		pla.kd = allPlanes[i+3].z;
		myPla.push_back(pla);		
	}
	for (unsigned i = 0; i < allLights.size(); i=i+2){
		Light lig;
		if (mode == 4)
			lig.pos = allLights[i]/2.0f;
		else
			lig.pos = allLights[i];
		lig.colour = allLights[i+1];
		myLig.push_back(lig);
	}
	
	if (mode == 4) {
		vector< vector< vec3 > > vertices0;
		vector< vector< vec2 > > uvs;
		vector< vector< vec3 > > normals; 
		vector< unsigned int > tID;
			
		bool res = loadOBJ("shimakaze/shimakaze.obj", vertices0, uvs, normals, tID);
		
		for(int i=0; i<vertices0.size(); i++) {
//		int i = 13;
			for(int j=0; j<vertices0[i].size(); j=j+3) {
				TextTri shimaTri;
				shimaTri.p0 = (vertices0[i][j]+vec3(-1.5f,-2.75f,-8.25f));
				shimaTri.p1 = (vertices0[i][j+1]+vec3(-1.5f,-2.75f,-8.25f));
				shimaTri.p2 = (vertices0[i][j+2]+vec3(-1.5f,-2.75f,-8.25f));
				shimaTri.uv0 = uvs[i][j];
				shimaTri.uv1 = uvs[i][j+1];
				shimaTri.uv2 = uvs[i][j+2];
				shimaTri.textureIndex = tID[i];
				shimaTri.vn = cross(shimaTri.p1-shimaTri.p0,shimaTri.p2-shimaTri.p0);
//				shimaTri.vn = normals[i][j];
				shimaTri.kd = texkd;
				shimaTri.p = 1.0f;
				shimaTri.ks = texks;
				myShimakaze.push_back(shimaTri);
			}
		}
		
		TextTri rightWall0;
		rightWall0.p0 = vec3(2.75f, 2.75f, -5.0f);
		rightWall0.p1 = vec3(2.75f, 2.75f, -10.5f);
		rightWall0.p2 = vec3(2.75f, -2.75f, -10.5f);
		rightWall0.uv0 = vec2(1.0f, 1.0f);
		rightWall0.uv1 = vec2(0.0f, 1.0f);
		rightWall0.uv2 = vec2(0.0f, 0.0f);
		rightWall0.textureIndex = 36;
		rightWall0.vn = cross(rightWall0.p1 - rightWall0.p0, rightWall0.p2 - rightWall0.p0);
		rightWall0.kd = texkd;
		rightWall0.p = 1.0f;
		rightWall0.ks = texks;
		TextTri rightWall1;
		rightWall1.p0 = vec3(2.75f, -2.75f, -5.0f);
		rightWall1.p1 = vec3(2.75f, 2.75f, -5.0f);
		rightWall1.p2 = vec3(2.75f, -2.75f, -10.5f);
		rightWall1.uv0 = vec2(1.0f, 0.0f);
		rightWall1.uv1 = vec2(1.0f, 1.0f);
		rightWall1.uv2 = vec2(0.0f, 0.0f);
		rightWall1.textureIndex = 36;
		rightWall1.vn = cross(rightWall1.p1 - rightWall1.p0, rightWall1.p2 - rightWall1.p0);
		rightWall1.kd = texkd;
		rightWall1.p = 1.0f;
		rightWall1.ks = texks;
		TextTri leftWall0;
		leftWall0.p0 = vec3(-2.75f, -2.75f, -5.0f);
		leftWall0.p1 = vec3(-2.75f, -2.75f, -10.5f);
		leftWall0.p2 = vec3(-2.75f, 2.75f, -10.5f);
		leftWall0.uv0 = vec2(0.0f, 0.0f);
		leftWall0.uv1 = vec2(1.0f, 0.0f);
		leftWall0.uv2 = vec2(1.0f, 1.0f);
		leftWall0.textureIndex = 37;
		leftWall0.vn = cross(leftWall0.p1 - leftWall0.p0, leftWall0.p2 - leftWall0.p0);
		leftWall0.kd = texkd;
		leftWall0.p = 1.0f;
		leftWall0.ks = texks;
		TextTri leftWall1;
		leftWall1.p0 = vec3(-2.75f, 2.75f, -5.0f);
		leftWall1.p1 = vec3(-2.75f, -2.75f, -5.0f);
		leftWall1.p2 = vec3(-2.75f, 2.75f, -10.5f);
		leftWall1.uv0 = vec2(0.0f, 1.0f);
		leftWall1.uv1 = vec2(0.0f, 0.0f);
		leftWall1.uv2 = vec2(1.0f, 1.0f);
		leftWall1.textureIndex = 37;
		leftWall1.vn = cross(leftWall1.p1 - leftWall1.p0, leftWall1.p2 - leftWall1.p0);
		leftWall1.kd = texkd;
		leftWall1.p = 1.0f;
		leftWall1.ks = texks;
		myShimakaze.push_back(rightWall0);
		myShimakaze.push_back(rightWall1);
		myShimakaze.push_back(leftWall0);
		myShimakaze.push_back(leftWall1);
		
		TextTri shimakazeFML[myShimakaze.size()];
		for (int i=0; i<myShimakaze.size(); i++)
			shimakazeFML[i] = myShimakaze[i];
		
		glUseProgram(program);
		
		for (int i=0; i<36; i++) {
			
			if (i<10)
				uniformName = "tex0" + to_string(i);
			else
				uniformName = "tex" + to_string(i);
			glUniform1i(glGetUniformLocation(program, uniformName.c_str()), i);
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, shimaTex[i].textureID);
		}		
		uniformName = "tex36";
		glUniform1i(glGetUniformLocation(program, uniformName.c_str()), 36);
		glActiveTexture(GL_TEXTURE0 + 36);
		glBindTexture(GL_TEXTURE_2D, wallTex0.textureID);
		uniformName = "tex37";
		glUniform1i(glGetUniformLocation(program, uniformName.c_str()), 37);
		glActiveTexture(GL_TEXTURE0 + 37);
		glBindTexture(GL_TEXTURE_2D, wallTex1.textureID);
		
		GLuint ssbo4;
		glGenBuffers(1, &ssbo4);	
		glBindBuffer(GL_SHADER_STORAGE_BUFFER,ssbo4);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(TextTri)*myShimakaze.size(), shimakazeFML, GL_DYNAMIC_COPY);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER,6,ssbo4);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
		myShimakaze.clear();
		for(int i=0; i<vertices0.size(); i++) {
			vertices0[i].clear();
			uvs[i].clear();
			normals[i].clear();
		}
		vertices0.clear();
		uvs.clear();
		normals.clear();
		tID.clear();
		glUseProgram(0);

	
	}
	
	Sphere sphFML[mySph.size()];
	for (int i=0; i<mySph.size(); i++)
		sphFML[i] = mySph[i];

	Triangle triFML[myTri.size()];
	for (int i=0; i<myTri.size(); i++)
		triFML[i] = myTri[i];
	
	Plane plaFML[myPla.size()];
	for (int i=0; i<myPla.size(); i++)
		plaFML[i] = myPla[i];
	
	Light ligFML[myLig.size()];
	for (int i=0; i<myLig.size(); i++)
		ligFML[i] = myLig[i];
	
	glUseProgram(program);
	GLuint ssbo0, ssbo1, ssbo2, ssbo3;
	glGenBuffers(1, &ssbo0);	
	glBindBuffer(GL_SHADER_STORAGE_BUFFER,ssbo0);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Sphere)*mySph.size(), sphFML, GL_DYNAMIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER,2,ssbo0);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	
	glGenBuffers(1, &ssbo1);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER,ssbo1);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Triangle)*myTri.size(), triFML, GL_DYNAMIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER,3,ssbo1);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	
	glGenBuffers(1, &ssbo2);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER,ssbo2);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Plane)*myPla.size(), plaFML, GL_DYNAMIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER,4,ssbo2);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	glGenBuffers(1, &ssbo3);	
	glBindBuffer(GL_SHADER_STORAGE_BUFFER,ssbo3);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Light)*myLig.size(), ligFML, GL_DYNAMIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER,5,ssbo3);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	myTri.clear();
	mySph.clear();
	myPla.clear();
	myLig.clear();
	
	glUseProgram(0);
	allSpheres.clear();
    allTriangles.clear();
    allPlanes.clear();
    allLights.clear();
//}
}


void RenderScene(Geometry *geometry, Geometry *myfbo, GLuint program, MyTexture *tex, bool clr)
//void RenderScene(Geometry *geometry, GLuint program, MyTexture *tex, bool clr)
{
	// clear screen to a dark grey colour
	if (clr) {
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT);// | GL_DEPTH_BUFFER_BIT);
	}
	if (mode == 3) {
		glUseProgram(program);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo.fboID);
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glBindTexture(GL_TEXTURE_2D, tex->textureID);

//		glBindVertexArray(myfbo->vertexArray);
		glUniform1f(glGetUniformLocation(program, "fov0"), fov);
		glUniform1i(glGetUniformLocation(program, "mode"), mode);
		glUniform1f(glGetUniformLocation(program, "camx"), position.x);
		glUniform1f(glGetUniformLocation(program, "camy"), position.y);
		glUniform1f(glGetUniformLocation(program, "camz"), position.z);
		glUniform1f(glGetUniformLocation(program, "camthx"), thy);
		glUniform1f(glGetUniformLocation(program, "camthy"), thx);
		glUniform1f(glGetUniformLocation(program, "aperture"), pupil);
		glUniform1i(glGetUniformLocation(program, "applyblur"), 1);
		glBindVertexArray(myfbo->vertexArray);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// reset state to default (no shader or geometry bound)
		glBindVertexArray(0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glUseProgram(0);
		glBindTexture(GL_TEXTURE_2D, fbo.textureID);
		
		glUseProgram(program);
	// bind our shader program and the vertex array object containing our
	// scene geometry, then tell OpenGL to draw our geometry
		glUniform1f(glGetUniformLocation(program, "fov0"), fov);
		glUniform1i(glGetUniformLocation(program, "mode"), mode);
		glUniform1f(glGetUniformLocation(program, "camx"), position.x);
		glUniform1f(glGetUniformLocation(program, "camy"), position.y);
		glUniform1f(glGetUniformLocation(program, "camz"), position.z);
		glUniform1f(glGetUniformLocation(program, "camthx"), thy);
		glUniform1f(glGetUniformLocation(program, "camthy"), thx);
		glUniform1f(glGetUniformLocation(program, "aperture"), pupil);
		glUniform1i(glGetUniformLocation(program, "applyblur"), 1);
		glBindVertexArray(geometry->vertexArray);
		glDrawArrays(GL_TRIANGLES, 0, geometry->elementCount);	

	}
	else {
	
//		glBindTexture(GL_TEXTURE_2D, tex->textureID);
	
	
		glUseProgram(program);
		// bind our shader program and the vertex array object containing our
		// scene geometry, then tell OpenGL to draw our geometry
		glBindVertexArray(geometry->vertexArray);
		glUniform1f(glGetUniformLocation(program, "fov0"), fov);
		glUniform1i(glGetUniformLocation(program, "mode"), mode);
		glUniform1f(glGetUniformLocation(program, "camx"), position.x);
		glUniform1f(glGetUniformLocation(program, "camy"), position.y);
		glUniform1f(glGetUniformLocation(program, "camz"), position.z);
		glUniform1f(glGetUniformLocation(program, "camthx"), thy);
		glUniform1f(glGetUniformLocation(program, "camthy"), thx);
		glUniform1f(glGetUniformLocation(program, "aperture"), pupil);
		glUniform1i(glGetUniformLocation(program, "applyblur"), 0);
		glBindVertexArray(geometry->vertexArray);
		glDrawArrays(GL_TRIANGLES, 0, geometry->elementCount);

	}
	
	// reset state to default (no shader or geometry bound)
	glBindVertexArray(0);
	glUseProgram(0);

	// check for an report any OpenGL errors
	CheckGLErrors();
}

// --------------------------------------------------------------------------
// GLFW callback functions

// reports GLFW errors
void ErrorCallback(int error, const char* description)
{
	cout << "GLFW ERROR " << error << ":" << endl;
	cout << description << endl;
}

// handles keyboard input events
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_1 && (action == GLFW_PRESS) && mode != 1) {
		mode = 1;
		fov = -1.4f, current = 0;
		dx=0, dy=0, dz=1, thx=0, thy=0;
		xclick = 0, yclick = 0, oldx=0, oldy=0;
		position = vec3(0,0,1);
		glDeleteProgram(program);
		program = InitializeShaders();
		MakeWorld();
	}
	if (key == GLFW_KEY_2 && (action == GLFW_PRESS) && mode != 2) {
		mode = 2;
		fov = -1.0f, current = 0;
		dx=0, dy=0, dz=1, thx=0, thy=0;
		xclick = 0, yclick = 0, oldx=0, oldy=0;
		position = vec3(0,0,1);
		glDeleteProgram(program);
		program = InitializeShaders();
		MakeWorld();
	}
	if (key == GLFW_KEY_3 && (action == GLFW_PRESS) && mode != 3){
		mode = 3;	
		fov = -1.0, current = 0;
		dx=0, dy=0, dz=1, thx=0, thy=0;
		xclick = 0, yclick = 0, oldx=0, oldy=0;
		position = vec3(0,0,1);
		glDeleteProgram(program);
		program = InitializeShaders();
		MakeWorld();
	}
	if (key == GLFW_KEY_4 && (action == GLFW_PRESS) && mode != 4){
		cout << "WARNING, DUE TO THE 36000+ TRIANGLES ON THIS SCENE, \n" <<
				"RAY TRACING MAY TAKE SEVERAL MINUTES... \n" << endl;
		mode = 4;	
		fov = -1.4, current = 0;
		dx=0, dy=0, dz=1, thx=0, thy=0;
		xclick = 0, yclick = 0, oldx=0, oldy=0;
		position = vec3(0,0,1);
		glDeleteProgram(program);
		program = InitializeShaders();
		MakeWorld();
	}
	if (key == GLFW_KEY_A && action == GLFW_PRESS) 
		strafeLeft = true;
	if (key == GLFW_KEY_S && action == GLFW_PRESS) 
		backward = true;
	if (key == GLFW_KEY_D && action == GLFW_PRESS) 
		strafeRight = true;
	if (key == GLFW_KEY_W && action == GLFW_PRESS) 
		goforward = true;
	if (key == GLFW_KEY_R && action == GLFW_PRESS) 
		flyUp = true;
	if (key == GLFW_KEY_F && action == GLFW_PRESS) 
		flyDown = true;
	if (key == GLFW_KEY_G && (action == GLFW_PRESS)) {//||action == GLFW_REPEAT)) {
		fov = -1.25f;
	}
	if (key == GLFW_KEY_A && action == GLFW_RELEASE) 
		strafeLeft = false;
	if (key == GLFW_KEY_S && action == GLFW_RELEASE) 
		backward = false;
	if (key == GLFW_KEY_D && action == GLFW_RELEASE) 
		strafeRight = false;
	if (key == GLFW_KEY_W && action == GLFW_RELEASE) 
		goforward = false;
	if (key == GLFW_KEY_R && action == GLFW_RELEASE) 
		flyUp = false;
	if (key == GLFW_KEY_F && action == GLFW_RELEASE) 
		flyDown = false;
	if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
		if (mode == 3) {
			if (pupilradius < 10) {
				pupilradius++;
				pupil = pupil + 0.05f;
			}
		}
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
		if (mode == 3) {
			if (pupilradius > 0) {
				pupilradius--;
				pupil = pupil - 0.05f;
			}
		}
	}
	if (key == GLFW_KEY_P && action == GLFW_RELEASE) {
		cout << "SCREENSHOT CAPTURED" << endl;
		int w = 512, h = 512;
		string filename = "scene.tga";
		glPixelStorei(GL_PACK_ALIGNMENT, 1);

		int nSize = w*h*3;
		// First let's create our buffer, 3 channels per Pixel
		char* dataBuffer = (char*)malloc(nSize*sizeof(char));

		//  if (!dataBuffer) return false;

		// Let's fetch them from the backbuffer	
		// We request the pixels in GL_BGR format, thanks to Berzeger for the tip
		glReadPixels((GLint)0, (GLint)0,
			(GLint)w, (GLint)h,
			 GL_BGR, GL_UNSIGNED_BYTE, dataBuffer);

		//Now the file creation
		FILE *filePtr = fopen(filename.c_str(), "wb");
		//   if (!filePtr) return false;


		unsigned char TGAheader[12]={0,0,2,0,0,0,0,0,0,0,0,0};
		unsigned char header[6] = { w%256,w/256,
					   h%256,h/256,
					   24,0};
		// We write the headers
		fwrite(TGAheader,	sizeof(unsigned char),	12,	filePtr);
		fwrite(header,	sizeof(unsigned char),	6,	filePtr);
		// And finally our image data
		fwrite(dataBuffer,	sizeof(GLubyte),	nSize,	filePtr);
		fclose(filePtr);

		free(dataBuffer);	
	}
	
}
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
	cout.precision(8);
	
	double x_raw, y_raw, x_ogl, y_ogl;//, dx0,dy0;
	if (lclickdown) {	
		glfwGetCursorPos(window, &x_raw, &y_raw);
		x_ogl = (-512+x_raw)/512;
		y_ogl = (512-y_raw)/512;
		thx = oldx-x_ogl+xclick;
		thy = oldy+y_ogl-yclick;
	}	
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	xclick = (-512+x)/512;
	yclick = (512-y)/512;
	
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{	
		lclickdown = true;
//		dx = 0;
//		dy = 0;
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		lclickdown = false;
		oldx=thx;
		oldy=thy;

	}
	
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	int min = -5, max = 20;
	if (yoffset < 0) 
		if (current > min)
			current--;
	if (yoffset > 0) 
		if (current < max)
			current++;
	if (current >min && current<max) {
		
		fov = fov-0.25f*yoffset;
	}
}


void controller() {
	if (mode == 3) {
/*		if (strafeLeft) {
			position += vec3(-1,0,0)*0.1f;
		}
		if (backward) {
			position += vec3(0,0,1)*0.1f;
		}
		if (strafeRight) {
			position -= vec3(-1,0,0)*0.1f;
		}
		if (goforward) {
			position -= vec3(0,0,1)*0.1f;
		}
		if (flyUp) {
			position += vec3(0.0f,0.1f,0.0f);
			
		}
		if (flyDown) {
			position -= vec3(0.0f,0.1f,0.0f);
		}*/
	}
	else {
		if (strafeLeft) {
			position += vec3(sin(thx-M_PI/2.0f),0,cos(thx-M_PI/2.0f))*0.1f;
		}
		if (backward) {
			position += vec3(	cos(-thy) * sin(thx), sin(-thy),cos(-thy) * cos(thx))*0.1f;
		}
		if (strafeRight) {
			position -= vec3(sin(thx-M_PI/2.0f),0,cos(thx-M_PI/2.0f))*0.1f;
		}
		if (goforward) {
			position -= vec3(	cos(-thy) * sin(thx), sin(-thy),cos(-thy) * cos(thx))*0.1f;
		}
		if (flyUp) {
			position += vec3(0.0f,0.1f,0.0f);
			
		}
		if (flyDown) {
			position -= vec3(0.0f,0.1f,0.0f);
		}
	}
}
// ==========================================================================
// PROGRAM ENTRY POINT

int main(int argc, char *argv[])
{
	// initialize the GLFW windowing system
	if (!glfwInit()) {
		cout << "ERROR: GLFW failed to initialize, TERMINATING" << endl;
		return -1;
	}
	glfwSetErrorCallback(ErrorCallback);

	// attempt to create a window with an OpenGL 4.3 core profile context
	GLFWwindow *window = 0;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	int width = 512, height = 512;
	window = glfwCreateWindow(width, height, "CPSC 453 OpenGL Boilerplate", 0, 0);
	if (!window) {
		cout << "Program failed to create GLFW window, TERMINATING" << endl;
		glfwTerminate();
		return -1;
	}

	// set keyboard callback function and make our context current (active)
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwMakeContextCurrent(window);

	//Intialize GLAD
	if (!gladLoadGL())
	{
		cout << "GLAD init failed" << endl;
		return -1;
	}

	// query and print out information about our OpenGL environment
	QueryGLVersion();

	// call function to load and compile shader programs
	program = InitializeShaders();
	if (program == 0) {
		cout << "Program could not initialize shaders, TERMINATING" << endl;
		return -1;
	}
	
	cout << glGetString(GL_VERSION) << endl;
	
	vec2 vertices[] = {
		vec2( -1.0f, -1.0f),
		vec2( 1.0f,  1.0f),
		vec2( 1.0f, -1.0f),
		vec2( -1.0f, -1.0f),
		vec2( 1.0f,  1.0f),
		vec2( -1.0f, 1.0f)
	};

	vec3 colours[] = {
		vec3( 1.0f, 0.0f, 0.0f ),
		vec3( 0.0f, 1.0f, 0.0f ),
		vec3( 0.0f, 0.0f, 1.0f ),
		vec3( 1.0f, 0.0f, 0.0f ),
		vec3( 0.0f, 1.0f, 0.0f ),
		vec3( 0.0f, 0.0f, 1.0f )
	};
	vec2 textures[] = {
		vec2( .0f, .0f ),
		vec2( 1.0f,  1.0f ),
		vec2( 1.0f, .0f ),
		vec2( .0f, .0f ),
		vec2( 1.0f,  1.0f ),
		vec2( 0.0f, 1.0f )
	};
	vec2 fbos[] = {
		vec2( -1.0f, -1.0f ),
		vec2( 1.0f,  1.0f ),
		vec2( -1.0f, 1.0f ),
		vec2( -1.0f, -1.0f ),
		vec2( 1.0f,  1.0f ),
		vec2( 1.0f, -1.0f )
	};
	
	
	if (!InitializeFBO(&fbo, GL_TEXTURE_2D))
		cout << "Program failed to initialize rendered texture" << endl;
	Geometry fbogeo;
	if (!InitializeVAO(&fbogeo))
		cout << "Program failed to intialize geometry!" << endl;
	if(!LoadGeometry(&fbogeo, fbos, colours, textures, 6))
		cout << "Failed to load geometry" << endl;

	// three vertex positions and assocated colours of a triangle

	Geometry geometry;
	MyTexture temp1;
//	for (int i=0; i<3; i++) {
	if (!InitializeVAO(&geometry))
		cout << "Program failed to intialize geometry!" << endl;
	if(!LoadGeometry(&geometry, vertices, colours, textures, 6))
		cout << "Failed to load geometry" << endl;
		
	for (int i=0; i<36; i++) {
		if (!InitializeTexture(&shimaTex[i], pics[i], GL_TEXTURE_2D))
			cout << "Program failed to initialize texture" << endl;
	}
	if (!InitializeTexture(&wallTex0, wallName0, GL_TEXTURE_2D))
			cout << "Program failed to initialize texture" << endl;
	if (!InitializeTexture(&wallTex1, wallName1, GL_TEXTURE_2D))
			cout << "Program failed to initialize texture" << endl;
	MakeWorld();
	
		// run an event-triggered main loop
	bool renderOnce = true; // FOR SPEEDING UP SCENE 4, CANNOT MOVE CAMERA
	while (!glfwWindowShouldClose(window))
	{
		controller();
		if (mode != 4) {
			RenderScene(&geometry, &fbogeo, program, &temp1,true);
		}
		else {
			if (renderOnce) {
				RenderScene(&geometry, &fbogeo, program, &temp1,true);
				renderOnce = false;
			}
		}

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	// clean up allocated resources before exit
	DestroyGeometry(&geometry);
	for(int i=0; i<36; i++)
		DestroyTexture(&shimaTex[i]);
	DestroyTexture(&wallTex0);
	DestroyTexture(&wallTex1);
	glUseProgram(0);
	glDeleteProgram(program);
	glfwDestroyWindow(window);
	glfwTerminate();

	cout << "Goodbye!" << endl;
	return 0;
}

// ==========================================================================
// SUPPORT FUNCTION DEFINITIONS

// --------------------------------------------------------------------------
// OpenGL utility functions

void QueryGLVersion()
{
	// query opengl version and renderer information
	string version = reinterpret_cast<const char *>(glGetString(GL_VERSION));
	string glslver = reinterpret_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION));
	string renderer = reinterpret_cast<const char *>(glGetString(GL_RENDERER));

	cout << "OpenGL [ " << version << " ] "
		<< "with GLSL [ " << glslver << " ] "
		<< "on renderer [ " << renderer << " ]" << endl;
}

bool CheckGLErrors()
{
	bool error = false;
	for (GLenum flag = glGetError(); flag != GL_NO_ERROR; flag = glGetError())
	{
		cout << "OpenGL ERROR:  ";
		switch (flag) {
		case GL_INVALID_ENUM:
			cout << "GL_INVALID_ENUM" << endl; break;
		case GL_INVALID_VALUE:
			cout << "GL_INVALID_VALUE" << endl; break;
		case GL_INVALID_OPERATION:
			cout << "GL_INVALID_OPERATION" << endl; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			cout << "GL_INVALID_FRAMEBUFFER_OPERATION" << endl; break;
		case GL_OUT_OF_MEMORY:
			cout << "GL_OUT_OF_MEMORY" << endl; break;
		default:
			cout << "[unknown error code]" << endl;
		}
		error = true;
	}
	return error;
}

// --------------------------------------------------------------------------
// OpenGL shader support functions

// reads a text file with the given name into a string
string LoadSource(const string &filename)
{
	string source;

	ifstream input(filename.c_str());
	if (input) {
		copy(istreambuf_iterator<char>(input),
			istreambuf_iterator<char>(),
			back_inserter(source));
		input.close();
	}
	else {
		cout << "ERROR: Could not load shader source from file "
			<< filename << endl;
	}

	return source;
}

// creates and returns a shader object compiled from the given source
GLuint CompileShader(GLenum shaderType, const string &source)
{
	// allocate shader object name
	GLuint shaderObject = glCreateShader(shaderType);

	// try compiling the source as a shader of the given type
	const GLchar *source_ptr = source.c_str();
	glShaderSource(shaderObject, 1, &source_ptr, 0);
	glCompileShader(shaderObject);

	// retrieve compile status
	GLint status;
	glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint length;
		glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &length);
		string info(length, ' ');
		glGetShaderInfoLog(shaderObject, info.length(), &length, &info[0]);
		cout << "ERROR compiling shader:" << endl << endl;
		cout << source << endl;
		cout << info << endl;
	}

	return shaderObject;
}

// creates and returns a program object linked from vertex and fragment shaders
GLuint LinkProgram(GLuint vertexShader, GLuint fragmentShader)
{
	// allocate program object name
	GLuint programObject = glCreateProgram();

	// attach provided shader objects to this program
	if (vertexShader)   glAttachShader(programObject, vertexShader);
	if (fragmentShader) glAttachShader(programObject, fragmentShader);

	// try linking the program with given attachments
	glLinkProgram(programObject);

	// retrieve link status
	GLint status;
	glGetProgramiv(programObject, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint length;
		glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &length);
		string info(length, ' ');
		glGetProgramInfoLog(programObject, info.length(), &length, &info[0]);
		cout << "ERROR linking shader program:" << endl;
		cout << info << endl;
	}

	return programObject;
}
