//---------------------------------------------------//
//		ADVANCED GRAPHICS ASSIGNMENT (ET5 info)		 //
//              THOMAS VON ASCHEBERG                 //
//					 MY-LINH HO		                 //
//---------------------------------------------------//

#include "plane.h"
#include <iostream>
#include <math.h>

/************************** Plane **********************************/

Hit Plane::intersect(const Ray& ray) {

	double t = 0.0f;
	Vector N;

	//Constant for dot product min (for parallel rays)
	const double EPSILON = 0.0000001;

	//Computation of the normal of the triangle (we need at least one edge)
	Vector edgeP0P1 = points[1] - points[0];
	Vector edgeP1P2 = points[2] - points[1];

	//Cross product of 2 edges is the normal vector of the plane
	N = edgeP0P1.cross(edgeP1P2).normalized();
	 

	//First we search for P the vector from the ray's origin to the inpact point

	//We don't want to compute almost parallel rays
	double v = N.dot(ray.D);
	if (1 - abs(v) < EPSILON) {
		return Hit::NO_HIT();    // NO HIT => the ray is parallel to the triangle
	}

	//d value in plane's equation
	double d = N.dot(-points[0]);

	//then compute t according to the equation
	t = -(N.dot(ray.O) + d) / v;

	return Hit(t, N);
}
