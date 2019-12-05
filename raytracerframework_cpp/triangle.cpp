//---------------------------------------------------//
//		ADVANCED GRAPHICS ASSIGNMENT (ET5 info)		 //
//              THOMAS VON ASCHEBERG                 //
//					 MY-LINH HO		                 //
//---------------------------------------------------//

#include "triangle.h"
#include <iostream>
#include <math.h>

/************************** Triangle **********************************/


//Method inspired by https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/ray-triangle-intersection-geometric-solution
Hit Triangle::intersect(const Ray& ray) {

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
	// compute the intersection point using equation definition : 
	// P = ray.O + t*ray.D
	Vector P = ray.O + t * ray.D;

	//Then we need to check inside-outside issue (with 2D object)
	Vector C; // vector that is perpendicular to triangle's plane (will change for each vertex)

	// edge 0
	Vector edge0 = points[1] - points[0];
	Vector vp0 = P - points[0];
	if (edge0.dot(vp0) <= 0) {
		return Hit::NO_HIT(); // P is on the right side 
	}

	// edge 1
	Vector edge1 = points[2] - points[1];
	Vector vp1 = P - points[1];
	//C = edge1.cross(vp1);
	if (edge1.dot(vp1) <= 0) {
		return Hit::NO_HIT(); // P is on the right side 
	}

	// edge 2
	Vector edge2 = points[0] - points[2];
	Vector vp2 = P - points[2];
	//C = edge2.cross(vp2);
	if (edge2.dot(vp2) <= 0) {
		return Hit::NO_HIT(); // P is on the right side 
	}

	return Hit(t, N);
}
