//
//  Framework for a raytracer
//  File: object.h
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

#ifndef OBJECT_H_AXKLE0OF
#define OBJECT_H_AXKLE0OF

#include "triple.h"
#include "light.h"
#include "transformations.h"

class Material; //precreate class material for circular inclusion issue

/**
 * Object abstract class :
 * Representation of an abstract object (mesh) for our raytracer
 * It is the base class for all object types
 */
class Object {
public:
	
    Material *material;

	Vector rotationAxis;        //small trick for rotation
	double rotationAngleDeg;

    virtual ~Object() { }

    //Method to calculate the intersection between the object and a ray
    virtual Hit intersect(const Ray &ray) = 0;  

    //Method to get the UV coordinates of a point of the object
	virtual std::pair<double, double> getTextureCoords(Point p, Vector rotationAxis, double rotationAngleDeg) { return {}; };
};

#endif /* end of include guard: OBJECT_H_AXKLE0OF */
