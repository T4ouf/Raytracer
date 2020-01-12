//
//  Framework for a raytracer
//  File: sphere.h
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

#ifndef SPHERE_H_115209AE
#define SPHERE_H_115209AE

#include "object.h"

class Sphere : public Object
{
public:
    Sphere(Point position,double r, double rot, Vector vUp) : position(position), radius(r) {
		rotationAxis = vUp;
		rotationAngleDeg = rot;
	}

    virtual Hit intersect(const Ray &ray);
	virtual std::pair<double, double> getTextureCoords(Point p, Vector rotationAxis, double rotationAngleDeg) override;

	Point position;
    double radius;

	/*
	const Vector up;	//aka r
	const Vector side;	//aka s1
	const Vector s2;	// computed vector to ensure a 3D-Space basis
	const double RotationAngle;
	*/

};

#endif /* end of include guard: SPHERE_H_115209AE */
