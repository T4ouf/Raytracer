//
//  Framework for a raytracer
//  File: scene.h
//
//  Created for the Computer Science course "Introduction Computer Graphics"
//  taught at the University of Groningen by Tobias Isenberg.
//
//  Authors:
//    Maarten Everts
//    Jasper van de Gronde
//
//  This framework is inspired by and uses code of the raytracer framework of 
//  Bert Freudenberg that can be found at
//  http://isgwww.cs.uni-magdeburg.de/graphik/lehre/cg2/projekt/rtprojekt.html 
//

//---------------------------------------------------//
//		ADVANCED GRAPHICS ASSIGNMENT (ET5 info)		 //
//              THOMAS VON ASCHEBERG                 //
//					 MY-LINH HO		                 //
//---------------------------------------------------//

#ifndef SCENE_H_KNBLQLP6
#define SCENE_H_KNBLQLP6

#include <vector>
#include "triple.h"
#include "light.h"
#include "object.h"
#include "image.h"

typedef enum {
	PHONG			= 0,
	NORMALS			= 1,
	ZBUFFER			= 2,
	TEXTURECOORDS	= 3
} raytracingType;

struct Camera {
	Point eye;
	Point c;	//center
	Vector side;
	Vector up;
	double aspectRatio;
	int baseline;
	int superSampling;
};

class Scene{
private:
    std::vector<Object*> objects;
    std::vector<Light*> lights;
	raytracingType type;
	bool shadowComputation = false; //shadowComputation means : Compute shadows = true; don't compute = false
	
	int maxRecurDepth;

public:
	//Camera is accessible from the outside
	Camera camera;
	
	Color trace(const Ray &ray, int recurDepth);
    void render(Image &img);
    void addObject(Object *o);
    void addLight(Light *l);
	void setRaytracingType(raytracingType r);
	void setRaytracingType(string r);
	void setShadowBool(bool shadow);
	void setMaxRecursion(int recursionLimit);
    unsigned int getNumObjects() { return objects.size(); }
    unsigned int getNumLights() { return lights.size(); }
	bool hiddenSurface(const Ray &ray, Light& l);

	void setCamera(Camera* c);

};

#endif /* end of include guard: SCENE_H_KNBLQLP6 */
