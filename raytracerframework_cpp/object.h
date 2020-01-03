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

//---------------------------------------------------//
//		ADVANCED GRAPHICS ASSIGNMENT (ET5 info)		 //
//              THOMAS VON ASCHEBERG                 //
//					 MY-LINH HO		                 //
//---------------------------------------------------//

#ifndef OBJECT_H_AXKLE0OF
#define OBJECT_H_AXKLE0OF

#include "triple.h"
#include "light.h"
#include "transformations.h"

class Material;

class Object {
public:
	
    Material *material;
	Vector rotationAxis;
	double rotationAngleDeg;

    virtual ~Object() { }

    virtual Hit intersect(const Ray &ray) = 0;
	virtual std::pair<double, double> getTextureCoords(Point p, Vector rotationAxis, double rotationAngleDeg) { return {}; };
};

#endif /* end of include guard: OBJECT_H_AXKLE0OF */
