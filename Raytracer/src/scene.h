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

#ifndef SCENE_H_KNBLQLP6
#define SCENE_H_KNBLQLP6

#include <vector>
#include "triple.h"
#include "light.h"
#include "primitives/object.h"
#include "image.h"

/**
 * Enum that defines the rendering mode for the raytracer
 */
typedef enum {
	PHONG			= 0,
	NORMALS			= 1,
	ZBUFFER			= 2,
	TEXTURECOORDS	= 3,
	GOOCH			= 4,
	BLINNPHONG		= 5,
	CEL				= 6
} raytracingType;

/**
 * Struct that defines the parameters required for Gooch shading
 */
struct GoochParameters{
	double alpha;
	double beta;
	double b;
	double y;
	GoochParameters() {};
	GoochParameters(double gAlpha, double gBeta, double gB, double gY) : alpha(gAlpha), beta(gBeta), b(gB), y(gY) {}

};

/**
 * Struct that defines what a camera is for our static image renderer
 */
struct Camera {
	Point eye;
	Point c;	//center
	Vector side;
	Vector up;
	double aspectRatio; //width/height
	int baseline;		//define the baseline as the height
	int superSampling;	//supersampling factor (SSAA technique)
};

/**
 * Scene class :
 * Representation of a scene as it is usually done in graphics.
 * It is the main node containing all the objects, ligth sources and the camera (+ Some parameters)
 */
class Scene{
private:
    std::vector<Object*> objects;	//storing objects as pointers for memory sake (lighter)
    std::vector<Light*> lights;		//storing light sources as pointers for memory sake (lighter)
	raytracingType type;
	bool shadowComputation = false; //shadowComputation answers "Do we have to compute shadows ?" : => Yes (Compute shadows) = true; => No (don't compute) = false
	
	int maxRecurDepth;
	GoochParameters* gooch = new GoochParameters(0,0,0,0);

public:
	//Camera is accessible from the outside
	Camera camera;

	
	Color trace(const Ray &ray, int recurDepth);	//Method that computes the color of a given pixel (depending on the ray)
    void render(Image &img);						//Method that renders the scene to an image 

    void addObject(Object *o);						//Method to add objects to the scene
    void addLight(Light *l);						//Method to add light sources to the scene

	void setRaytracingType(raytracingType r);		//Method that defines the shading type that we use (e.g. Phong Shading, Gooch Shading, Cel Shading, etc.)
	//void setRaytracingType(string r);				//Overload from the previous method

	void setShadowBool(bool shadow);				//Setter that defines if the scene "contains" shadows
	void setMaxRecursion(int recursionLimit);		//Setter that defines how many times "bounces" the ray that we shoot
	void setGoochParams(const GoochParameters& g);	//Setter for Gooch parameters in Gooch Shading

    unsigned int getNumObjects() { return objects.size(); }	//Getter for the amount of objects
    unsigned int getNumLights() { return lights.size(); }	//Getter for the amount of light sources
	bool hiddenSurface(const Ray &ray, Light& l);			//Method that defines if the hit point is on the hidden surface of an object (regarding a given light source)

	void setCamera(Camera* c);						//Setter for the scene's camera

};

#endif /* end of include guard: SCENE_H_KNBLQLP6 */
