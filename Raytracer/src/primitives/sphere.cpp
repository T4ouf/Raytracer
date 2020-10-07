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

#include "sphere.h"
#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

/************************** Sphere **********************************/

/**
 * Method that defines the hit point between the sphere and a given ray
 * @param[in] ray, the ray we want to compute the intersection
 * @return the hit point and all its data (or Hit::NO_HIT() if no hit point)
 */
Hit Sphere::intersect(const Ray &ray)
{
	//////////////////////////////////////////
	//First we look for the intersection point

	//t is the distance from ray's origin to intersection point
    double t = 0;

	//Vector from ray's origin to center of the sphere
	Vector L = (position - ray.O);

	//We project L on the ray's direction (and get the total length)
	double tca = L.dot(ray.D);

	//if the sphere is behind the ray => no hit point
	if (tca < 0) {
		return Hit::NO_HIT();
	}

	//d is the distance between the sphere's origin and the "inbound part" of the ray (perpendicularly)
	double d = sqrt( L.length_2() - (tca * tca));

	if (d < 0) {
		return Hit::NO_HIT();
	}

	//Normally impossible (but remove some NaN values of thc so we keep it)
	if (d > radius) {
		return Hit::NO_HIT();
	}
	
	//length of the ray that is "inside" the sphere (trigo computation)
	double thc = sqrt(radius * radius - d * d);

	//from the total length we remove the part inside the sphere => we get the length until intersection
	t = tca - thc;
	//We also compute our vector from ray's origin to the impact point
	Vector P = (ray.O + (t * ray.D));

	//////////////////////////////////////////
    //Then we compute the normal at this point

	//Vector from sphere center to impact point (normal to the surface)
	Vector CP = P - position;
	Vector N = (CP).normalized();

    return Hit(t,N);
}

/**
 * Method that gives the UV coordinates of a given point on the sphere
 * @param[in] p, the point where the UV coordinates are needed
 * @param[in] rotationAxis, the the rotation axis of the sphere (for textures)
 * @param[in] rotationAngleDeg, the rotation angle in degree (for textures)
 * @return the UV coordinates as a pair of doubles
 */
std::pair<double, double> Sphere::getTextureCoords(Point p, Vector rotationAxis, double rotationAngleDeg) {

	if (rotationAngleDeg == 0.0f) {
		p = Transformations::rotationDeg(p - position, Vector(0, 0, 1), 90) + position;
	}
	else {
		p = Transformations::rotationDeg(p - position, rotationAxis, rotationAngleDeg) + position;
	}
	
	//trigonometric computations
	double theta = acos((p.z - position.z) / radius);
	double phi = atan2(p.y - position.y, p.x - position.x) - (M_PI);

	//make the phi angle positive
	while (phi < 0.0)
		phi += 2 * M_PI;
	
	//Use spherical coordinate system
	double u = phi / (2 * M_PI);
	double v = theta / M_PI;

	return { u,v };
	
}