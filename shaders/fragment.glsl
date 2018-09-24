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

// interpolated colour received from vertex stage
in vec3 Colour;
in vec2 UV;

in vec2 pixelPos;
// first output is mapped to the framebuffer's colour index by default
out vec4 FragmentColour;

uniform sampler2D ourTexture;

uniform sampler2D tex00;
uniform sampler2D tex01;
uniform sampler2D tex02;
uniform sampler2D tex03;
uniform sampler2D tex04;
uniform sampler2D tex05;
uniform sampler2D tex06;
uniform sampler2D tex07;
uniform sampler2D tex08;
uniform sampler2D tex09;
uniform sampler2D tex10;
uniform sampler2D tex11;
uniform sampler2D tex12;
uniform sampler2D tex13;
uniform sampler2D tex14;
uniform sampler2D tex15;
uniform sampler2D tex16;
uniform sampler2D tex17;
uniform sampler2D tex18;
uniform sampler2D tex19;
uniform sampler2D tex20;
uniform sampler2D tex21;
uniform sampler2D tex22;
uniform sampler2D tex23;
uniform sampler2D tex24;
uniform sampler2D tex25;
uniform sampler2D tex26;
uniform sampler2D tex27;
uniform sampler2D tex28;
uniform sampler2D tex29;
uniform sampler2D tex30;
uniform sampler2D tex31;
uniform sampler2D tex32;
uniform sampler2D tex33;
uniform sampler2D tex34;
uniform sampler2D tex35;
uniform sampler2D tex36;
uniform sampler2D tex37;
uniform sampler2D tex38;
uniform sampler2D tex39;

uniform int mode;

uniform float camx;
uniform float camy;
uniform float camz;
uniform float camthx;
uniform float camthy;

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
//uniform Triangle triangles[100];
/*layout(std430, binding = 3) buffer myTri {
	triangles[];
};*/
layout(std430, binding = 2) buffer SphereName {Sphere spheres[];};
layout(std430, binding = 3) buffer TriangleName {Triangle triangles[];};
layout(std430, binding = 4) buffer PlaneName {Plane planes[];};
layout(std430, binding = 5) buffer LightName {Light lights[];};
layout(std430, binding = 6) buffer TextureName {TextTri textures[];};

float PI = 3.1415926535897932384626433832795;
float fov = PI/3.0f;
uniform float fov0;

mat3 rotAboutY(float theta){
	return mat3(cos(theta), 0.0f, sin(theta), 0.0f,	1.0f, 0.0f,	-sin(theta), 0.0f, cos(theta));
}
mat3 rotAboutX(float theta){
	return mat3(1.0f, 0.0f, 0.0f, 0.0f, cos(theta), -sin(theta), 0.0f, sin(theta), cos(theta));
}
float sphereGetT(vec3 o, vec3 d, Sphere s) {	//d should be normalized
	float a = dot(d, d);
	float b = 2.0 * (dot(o, d) - dot(s.center, d));
	float c = (-2.0*dot(o, s.center)) + dot(o, o) + dot(s.center, s.center) - (s.radius * s.radius);
	float underRoot = b*b - 4.0*a*c;
	if (underRoot < 0){
		return -1.0;
	}
	float t1 = (-b + sqrt(underRoot)) / 2.0*a;
	float t2 = (-b - sqrt(underRoot)) / 2.0*a;
	float val = min(t1,t2);
	if (s.kd > 0) {
		if (abs(t1) < abs(t2))
			return t1;
		else
			return t2;
		}
	if (val < 0.0){
		val = max(t1,t2);
	}
	return val;
}
float triangleGetT(vec3 o, vec3 d, Triangle s) {
	vec3 d0 = o - s.p0;
	vec3 e1 = s.p1 - s.p0;
	vec3 e2 = s.p2 - s.p0;

	float tNumer = determinant(mat3(d0,e1,e2));
	float uNumer = determinant(mat3(-d,d0,e2));
	float vNumer = determinant(mat3(-d,e1,d0));
	float denom = 1.0/determinant(mat3(-d,e1,e2));

	float t = tNumer * denom;
	float u = uNumer * denom;
	float v = vNumer * denom;

	if ((u+v) < 1.0 && (u+v) > 0.0 && u > 0.0 && u < 1.0 && v > 0.0 && v < 1.0){
		return t;
	}
	else {
		return -1.0;
	}
}
vec3 triangleGetN(Triangle s){
	vec3 v1 = s.p1 - s.p0;
	vec3 v2 = s.p2 - s.p0;
	vec3 n = cross(v1, v2);
	return n;
}
float texturesGetT(vec3 o, vec3 d, TextTri s) {
	vec3 d0 = o - s.p0;
	vec3 e1 = s.p1 - s.p0;
	vec3 e2 = s.p2 - s.p0;

	float tNumer = determinant(mat3(d0,e1,e2));
	float uNumer = determinant(mat3(-d,d0,e2));
	float vNumer = determinant(mat3(-d,e1,d0));
	float denom = 1.0/determinant(mat3(-d,e1,e2));

	float t = tNumer * denom;
	float u = uNumer * denom;
	float v = vNumer * denom;

	if ((u+v) < 1.0 && (u+v) > 0.0 && u > 0.0 && u < 1.0 && v > 0.0 && v < 1.0){
		return t;
	}
	else {
		return -1.0;
	}
}
vec2 triangleGetB(vec3 o, vec3 d, TextTri s) {
	vec3 d0 = o - s.p0;
	vec3 e1 = s.p1 - s.p0;
	vec3 e2 = s.p2 - s.p0;

	float tNumer = determinant(mat3(d0,e1,e2));
	float uNumer = determinant(mat3(-d,d0,e2));
	float vNumer = determinant(mat3(-d,e1,d0));
	float denom = 1.0f/determinant(mat3(-d,e1,e2));

	float t = tNumer * denom;
	float u = uNumer * denom;
	float v = vNumer * denom;
	
	

	if ((u+v) < 1.0 && (u+v) > 0.0 && u > 0.0 && u < 1.0 && v > 0.0 && v < 1.0){
		return s.uv0*(1.0f-u-v)+s.uv1*u+s.uv2*v;
	}
	else {
		return vec2(0.0f, 0.0f);
	}
}


float planeGetT(vec3 o, vec3 d, Plane s) {
	vec3 d0 = s.point - o;
	if (dot(d, s.normal) == 0)
		return -1.0f;
	else 
		return dot(s.normal,d0)/dot(d,s.normal);
}

vec3 getTextureColour(vec2 tUV, int tIndex) {
	switch (tIndex) {
		case 0: return texture(tex00, tUV).xyz;
		case 1: return texture(tex01, tUV).xyz;
		case 2: return texture(tex02, tUV).xyz;
		case 3: return texture(tex03, tUV).xyz;
		case 4: return texture(tex04, tUV).xyz;
		case 5: return texture(tex05, tUV).xyz;
		case 6: return texture(tex06, tUV).xyz;
		case 7: return texture(tex07, tUV).xyz;
		case 8: return texture(tex08, tUV).xyz;
		case 9: return texture(tex09, tUV).xyz;
		case 10: return texture(tex10, tUV).xyz;
		case 11: return texture(tex11, tUV).xyz;
		case 12: return texture(tex12, tUV).xyz;
		case 13: return texture(tex13, tUV).xyz;
		case 14: return texture(tex14, tUV).xyz;
		case 15: return texture(tex15, tUV).xyz;
		case 16: return texture(tex16, tUV).xyz;
		case 17: return texture(tex17, tUV).xyz;
		case 18: return texture(tex18, tUV).xyz;
		case 19: return texture(tex19, tUV).xyz;
		case 20: return texture(tex20, tUV).xyz;
		case 21: return texture(tex21, tUV).xyz;
		case 22: return texture(tex22, tUV).xyz;
		case 23: return texture(tex23, tUV).xyz;
		case 24: return texture(tex24, tUV).xyz;
		case 25: return texture(tex25, tUV).xyz;
		case 26: return texture(tex26, tUV).xyz;
		case 27: return texture(tex27, tUV).xyz;
		case 28: return texture(tex28, tUV).xyz;
		case 29: return texture(tex29, tUV).xyz;
		case 30: return texture(tex30, tUV).xyz;
		case 31: return texture(tex31, tUV).xyz;
		case 32: return texture(tex32, tUV).xyz;
		case 33: return texture(tex33, tUV).xyz;
		case 34: return texture(tex34, tUV).xyz;
		case 35: return texture(tex35, tUV).xyz;		
		case 36: return texture(tex36, tUV).xyz;		
		case 37: return texture(tex37, tUV).xyz;		
    }
}

vec3 getObjColour(vec3 p, vec3 d, int shape, int obj) {
	vec3 col = vec3(1.0f);
	float objKd;
	float t;
	bool inShadow = false;
	for (int i=0; i<lights.length(); i++) {
		vec3 pToLight = normalize(lights[i].pos - p);
		float maxt = sqrt(dot((lights[i].pos - p), (lights[i].pos - p)));
		float t0 = maxt;
		float border = 0.001f;
		if (mode ==3) border = 0.0f;
		for (int j=0; j<spheres.length(); j++) {
			if (shape != 0 || obj != j)			//
				t = sphereGetT(p, pToLight, spheres[j]);
			if (t > border && t <= maxt && t < t0) {
				t0 = t;
				inShadow = true;
			}
		}
		for (int j=0; j<triangles.length(); j++) {
			if (shape != 1 || obj != j)			//
				t = triangleGetT(p, pToLight, triangles[j]);
			if (t > border && t <= maxt && t < t0) {
				t0 = t;
				inShadow = true;
			}
		}
		for (int j=0; j<textures.length(); j++) {
			if (shape != 3 || obj != j)			//
				t = texturesGetT(p, pToLight, textures[j]);
			if (t > border && t <= maxt && t < t0) {
				t0 = t;
				inShadow = true;
			}
		}
		for (int j=0; j<planes.length(); j++) {
			if (shape != 2 || obj != j)			//
				t = planeGetT(p, pToLight, planes[j]);
			if (t > border && t <= maxt && t < t0) {
				t0 = t;
				inShadow = true;
			}
		}
		if (t0 < 100000.0f) {
			vec3 objColour, pNormal;
			float ks, phong = 0.0f, sCol = 0.0f;
			if (shape == 0) {
				objColour = spheres[obj].colour;
				pNormal = normalize(p-spheres[obj].center);
				phong = spheres[obj].p;
				ks = spheres[obj].ks;
				
			}
			else if (shape == 1) {
				objColour = triangles[obj].colour;
				pNormal = normalize(triangleGetN(triangles[obj]));
				phong = triangles[obj].p;
				ks = triangles[obj].ks;
			}
			else if (shape == 3) {
				vec2 bc = triangleGetB(p, pToLight, textures[obj]);
				objColour = getTextureColour(bc, textures[obj].textureIndex);//texture(ourTexture, bc).xyz;
				pNormal = normalize(textures[obj].vn);
				phong = textures[obj].p;
				ks = textures[obj].ks;
			}
			else if (shape == 2) {
				objColour = planes[obj].colour;
				pNormal = normalize(planes[obj].normal);
				phong = planes[obj].p;
				ks = planes[obj].ks;
			}
			
			vec3 iL = lights[i].colour;
			vec3 iD = iL;
			vec3 iS = iL;
			if (inShadow) {
				iD = iD*(atan(t0*0.5f)/(PI*0.5f));
				iS = 0.4f*iD;
			}
			vec3 A = iL*0.2f;
			vec3 S = vec3(1.0f); //
			vec3 h = normalize(-d + pToLight);
			vec3 D = objColour*(A+(iD*max(0.0f,dot(pNormal,pToLight))));
			col = col * (D + (iS * S *pow(max(0.0f, dot(h,pNormal)), phong)));
		}
		else return vec3(1.0f);
	}
	return col;
}
vec3 getReflection(vec3 p, vec3 d, vec3 n, vec3 startColour, float kd) {
	vec3 col = startColour * (1.0f - kd);
	vec3 p_new = p, d_new = d;
	float kdCoeff = kd; //
	vec3 reflectedRay;
	vec3 objCol0;
	float kd1;
	vec3 vn0;
	int shape0;
	int obji;
	float border = 0.001f;
//	if (mode == 3) border = 0.0f;
	int reflectCount = 0;
	while (reflectCount < 15 && kd > 0) {
		reflectedRay = reflect(d,n);//-(2.0f*n*dot(d,n)));
		float t0 = 100000.0f;
		float t;
		for (int j=0; j<spheres.length(); j++) {
				t = sphereGetT(p, reflectedRay, spheres[j]);
			if (t > border && t < t0) {
				t0 = t;
				kd1 = spheres[j].kd;
				vn0 = normalize(p-spheres[j].center);
				shape0 = 0;
				obji = j;
			}
		}
		for (int j=0; j<triangles.length(); j++) {
			t = triangleGetT(p, reflectedRay, triangles[j]);
			if (t > border && t < t0) {
				t0 = t;
				kd1 = triangles[j].kd;
				vn0 = normalize(triangleGetN(triangles[j]));
				shape0 = 1;
				obji = j;
			}
		}
		for (int j=0; j<textures.length(); j++) {
			t = texturesGetT(p, reflectedRay, textures[j]);
			if (t > border && t < t0) {
				t0 = t;
				kd1 = textures[j].kd;
				vn0 = normalize(textures[j].vn);
				shape0 = 3;
				obji = j;
			}
		}

		for (int j=0; j<planes.length(); j++) {
				t = planeGetT(p, reflectedRay, planes[j]);
			if (t > border && t < t0) {
				t0 = t;
				kd1 = planes[j].kd;
				vn0 = normalize(planes[j].normal);
				shape0 = 2;
				obji = j;
			}
		}
		if (t0 < 100000.0f) {
			d_new = reflectedRay;
			p_new = p + (t0 * d_new);
			n = vn0;
			objCol0 = getObjColour(p_new, d_new, shape0, obji);
			kd = kd1;
			col = col +(objCol0*(1.0f-kd)*kdCoeff);
			kdCoeff = kdCoeff * kd;
			reflectCount++;
		}
		else {
			objCol0 = vec3(0.0f);
			kd = kd1;
			col = col + (objCol0 *(1.0 - kd) * kdCoeff);
			return col;
		}
	}
	return col;
	
}
/*
vec3 getRefraction(vec3 p, vec3 d, vec3 n, vec3 startColour, float kr) {
	vec3 col = startColour * (1.0f - kr);
	vec3 p_new = p, d_new = d;
	float kdCoeff = kr; //
	vec3 refractedRay;
	vec3 objCol0;
	float kd1;
	vec3 vn0;
	int shape0;
	int obji;
	float border = -0.001f;
	if (mode == 3) border = 0.0f;
	int reflectCount = 0;
	while (reflectCount < 5 && kr > 0) {
		refractedRay = refract(d,n,1.52f);//-(2.0f*n*dot(d,n)));
		float t0 = 100000.0f;
		float t;
		for (int j=0; j<spheres.length(); j++) {
				t = sphereGetT(p, refractedRay, spheres[j]);
			if (t > border && t < t0) {
				t0 = t;
				kd1 = spheres[j].kr;
				vn0 = normalize(p-spheres[j].center);
				shape0 = 0;
				obji = j;
			}
		}
		for (int j=0; j<triangles.length(); j++) {
			t = triangleGetT(p, refractedRay, triangles[j]);
			if (t > border && t < t0) {
				t0 = t;
				kd1 = triangles[j].kr;
				vn0 = normalize(triangleGetN(triangles[j]));
				shape0 = 1;
				obji = j;
			}
		}
		for (int j=0; j<planes.length(); j++) {
				t = planeGetT(p, refractedRay, planes[j]);
			if (t > border && t < t0) {
				t0 = t;
				kd1 = planes[j].kr;
				vn0 = normalize(planes[j].normal);
				shape0 = 2;
				obji = j;
			}
		}
		if (t0 < 100000.0f) {
			d_new = refractedRay;
			p_new = p + (t0 * d_new);
			n = vn0;
			objCol0 = getObjColour(p_new, d_new, shape0, obji);
			kr = kd1;
			col = col +(objCol0*(1.0f-kr)*kdCoeff);
			kdCoeff = kdCoeff * kr;
			reflectCount++;
		}
		else {
			objCol0 = vec3(0.0f);
			kr = kd1;
			col = col + (objCol0 *(1.0 - kr) * kdCoeff);
			return col;
		}
	}
	return col;
	
}
*/

vec3 getPixelColour(vec3 o, vec3 d) {
	vec3 col;
	float t;
	int obj, objIndex;
	float objKd, objKr;
	vec3 vn;
	float far = 100000.0f;
	for (int i=0; i< spheres.length(); i++) {
		t = sphereGetT(o, d, spheres[i]);
		if (t < far && t > 0.0f) {
			far = t;
			obj = 0;
			objIndex = i;
			objKd = spheres[i].kd;
			objKr = spheres[i].kr;
			vn = normalize((o + far*d) - spheres[i].center);
		}
	}
	for (int i=0; i< triangles.length(); i++) {
		t = triangleGetT(o, d, triangles[i]);
		if (t < far && t > 0.0f) {
			far = t;
			obj = 1;
			objIndex = i;
			objKd = triangles[i].kd;
			objKr = triangles[i].kr;
			vn = normalize(triangleGetN(triangles[i]));
		}
	}
	for (int i=0; i< textures.length(); i++) {
		t = texturesGetT(o, d, textures[i]);
		if (t < far && t > 0.0f) {
			far = t;
			obj = 3;
			objIndex = i;
			objKd = textures[i].kd;
			objKr = textures[i].kr;
			vn = normalize(textures[i].vn);
		}
	}
	for (int i=0; i< planes.length(); i++) {
		t = planeGetT(o, d, planes[i]);
		if (t < far && t > 0.0f) {
			far = t;
			obj = 2;
			objIndex = i;
			objKd = planes[i].kd;
			objKr = planes[i].kr;
			vn = normalize(planes[i].normal);
		}
	}
	
	
	if (far < 100000.0f) {
		vec3 intersectPoint = o + far*d;
		col = getObjColour(intersectPoint, d, obj, objIndex);
//		if (mode == 4) return col;
		col = getReflection(intersectPoint, d, vn, col, objKd);
		if (objKr > 0)
			return col;//getRefraction(intersectPoint, d, vn, col, objKr) ;
		else
			return col;
	}
	return vec3(0.0f);
}
//int n = 10;
//float aperture = 0.05;aw

vec3 getObjectT(vec3 o, vec3 d) {
//	vec3 col;
	float t;
	int obj, objIndex;
	float objKd;
	vec3 vn;
	float far = 100000.0f;
	for (int i=0; i< spheres.length(); i++) {
		t = sphereGetT(o, d, spheres[i]);
		if (t < far && t > 0.0f) {
			far = t;
		}
	}
	for (int i=0; i< triangles.length(); i++) {
		t = triangleGetT(o, d, triangles[i]);
		if (t < far && t > 0.0f) {
			far = t;
		}
	}
	for (int i=0; i< planes.length(); i++) {
		t = planeGetT(o, d, planes[i]);
		if (t < far && t > 0.0f) {
			far = t;
		}
	}
	
	if (far < 100000.0f) {
		return o + far*d;
	}
	else 
		return (vec3(0.0f,0.0f,-10.5f));
}

uniform float aperture;
uniform int applyblur;

vec3 camAngleColour(vec3 o, vec3 d, vec3 p, vec3 apOffset) {
	vec3 camera = o + apOffset;
	vec3 toDepth = p - camera;
	float thx3 = -atan(toDepth.x, -toDepth.z);
	float thy3 = atan(toDepth.y, -toDepth.z);
	vec3 objDir = d * rotAboutX(thy3) * rotAboutY(thx3);
	return getPixelColour(camera, objDir);
//	col = (tempCol0 +tempCol1)/2.0f;
}

void main(void) {
	vec3 col = vec3(0.0f);
	vec3 origin = vec3(0.0f, 0.0f, 0.0f);
	origin += vec3(camx, camy, camz);
	if (mode == 3 && applyblur == 1) {
		float focalLen = fov0/tan(fov * 0.5f);
		vec3 objPos = getObjectT(vec3(0.0001f, 0.0001f, 0.0f), vec3(0.0f, 0.0f, -1.0f));
//		vec3 objPos = vec3(0.0f, 0.0f, -5.0f);
		vec3 direction = normalize(vec3(pixelPos,focalLen));
		direction *= rotAboutX(camthx) * rotAboutY(camthy);
		vec3 tempCol0 = getPixelColour(origin, direction);
		vec3 tempCol1 = camAngleColour(origin, direction, objPos, vec3(-aperture, 0.0f, 0.0f));
		vec3 tempCol2 = camAngleColour(origin, direction, objPos, vec3(aperture, 0.0f, 0.0f));
		vec3 tempCol3 = camAngleColour(origin, direction, objPos, vec3(0.0f, -aperture, 0.0f));
		vec3 tempCol4 = camAngleColour(origin, direction, objPos, vec3(0.0f, aperture, 0.0f));
		
		vec3 tempCol5 = camAngleColour(origin, direction, objPos, vec3(-aperture/2.0f, -aperture/2.0f, 0.0f));
		vec3 tempCol6 = camAngleColour(origin, direction, objPos, vec3(-aperture/2.0f, aperture/2.0f, 0.0f));
		vec3 tempCol7 = camAngleColour(origin, direction, objPos, vec3(aperture/2.0f, aperture/2.0f, 0.0f));
		vec3 tempCol8 = camAngleColour(origin, direction, objPos, vec3(aperture/2.0f, -aperture/2.0f, 0.0f));
		col = (tempCol0 +tempCol1+tempCol2 +tempCol3+tempCol4 +tempCol5+tempCol6 +tempCol7+tempCol8)/9.0f;
	}
	else {
		float focalLen = fov0/tan(fov * 0.5f);
		vec3 direction = normalize(vec3(pixelPos,focalLen));
		direction *= rotAboutX(camthx) * rotAboutY(camthy);
		col = getPixelColour(origin, direction);	
	}
	FragmentColour = vec4(col, 1.0f);

}
