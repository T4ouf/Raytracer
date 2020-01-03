//
//  Framework for a raytracer
//  File: sphere.cpp
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

#include "sphere.h"
#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

/************************** Sphere **********************************/

Hit Sphere::intersect(const Ray &ray)
{
    /****************************************************
    * RT1.1: INTERSECTION CALCULATION
    *
    * Given: ray, position, r
    * Sought: intersects? if true: *t
    * 
    * Insert calculation of ray/sphere intersection here. 
    *
    * You have the sphere's center (C) and radius (r) as well as
    * the ray's origin (ray.O) and direction (ray.D).
    *
    * If the ray does not intersect the sphere, return Hit::NO_HIT().
    * Otherwise, return an instance of Hit() with the distance of the
    * intersection point from the ray origin as t and the normal as N (see example).
    ****************************************************/

    // place holder for actual intersection calculation

	//t is the distance from ray's origin to intersection point
    double t = 0;

	//Vector from ray's origin to center of the sphere
	Vector L = (position - ray.O);

	//We project L on the ray's direction (and get the total length)
	double tca = L.dot(ray.D);

	if (tca < 0) {
		return Hit::NO_HIT();
	}

	//d is the distance between the sphere's origin and the "inbound part" of the ray (perpendicularly)
	double d = sqrt( L.length_2() - (tca * tca));

	if (d < 0) {
		return Hit::NO_HIT();
	}

	//Normally impossible (but remove some NaN values of thc so we keep it
	if (d > radius) {
		return Hit::NO_HIT();
	}
	
	//length of the ray that is "inside" the sphere (trigo computation)
	double thc = sqrt(radius * radius - d * d);

	//from the total length we remove the part inside the sphere => we get the length until intersection
	t = tca - thc;
	//We also compute our vector from ray's origin to the impact point
	Vector P = (ray.O + (t * ray.D));

    /****************************************************
    * RT1.2: NORMAL CALCULATION
    *
    * Given: t, C, r
    * Sought: N
    * 
    * Insert calculation of the sphere's normal at the intersection point.
    ****************************************************/

	//Vector from sphere center to impact point (normal to the surface)
	Vector CP = P - position;
	Vector N = (CP).normalized();
	//Vector N = (Cimpact.normalized() + (ray.O + ray.D * t) + ray.O).normalized();
    return Hit(t,N);
}


std::pair<double, double> Sphere::getTextureCoords(Point p, Vector rotationAxis, double rotationAngleDeg) {
	/*
	Vector OP = p - this->position;
	double v = (1+this->up.normalized().dot(OP.normalized()))/2; // cos(theta)

	Vector OPPlaneProj = OP - (up.dot(OP)*up);

	double u = (1 + side.normalized().dot(OPPlaneProj.normalized())) / 2;

	return { u,v };
	*/
	
	//p = Transformations::rotationDeg(p - position, Vector(0, 0, 1), 180) + position;

	p = Transformations::rotationDeg(p - position, rotationAxis, rotationAngleDeg) + position;

	double theta = acos((p.z - position.z) / radius);
	double phi = atan2(p.y - position.y, p.x - position.x) - (M_PI);

	while (phi < 0.0)
		phi += 2 * M_PI;
	
	double u = phi / (2 * M_PI);
	double v = theta / M_PI;

	return { u,v };
	
}