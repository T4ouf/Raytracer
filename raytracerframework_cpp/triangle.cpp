//---------------------------------------------------//
//		ADVANCED GRAPHICS ASSIGNMENT (ET5 info)		 //
//              THOMAS VON ASCHEBERG                 //
//					 MY-LINH HO		                 //
//---------------------------------------------------//

#include "triangle.h"
#include <iostream>
#include <math.h>

/************************** Triangle **********************************/


Hit Triangle::intersect(const Ray& ray) {

	//Möller-Trumbore Algorithm for fast Triangle-Ray intersection
	//inspired from the tutorial : https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/moller-trumbore-ray-triangle-intersection

	const double EPSILON = 0.0000001;
	Vector vertex0 = points[0];
	Vector vertex1 = points[1];
	Vector vertex2 = points[2];
	Vector edge1, edge2, h, s, q;
	double a, f, u, v;
	edge1 = vertex1 - vertex0;
	edge2 = vertex2 - vertex0;
	h = ray.D.cross(edge2);
	a = edge1.dot(h);
	if (a > -EPSILON && a < EPSILON) {
		return Hit::NO_HIT();    // parallel ray => no hit
	}

	f = 1.0 / a;
	s = ray.O - vertex0;
	u = f * (s.dot(h));

	if (u < 0.0 || u > 1.0)
		return Hit::NO_HIT();

	q = s.cross(edge1);
	v = f * ray.D.dot(q);
	if (v < 0.0 || u + v > 1.0)
		return Hit::NO_HIT();

	// We compute the intersection point
	double t = f * edge2.dot(q);
	if (t > EPSILON) // If we do have an intersection
	{
		Vector N = ((points[1] - points[0]).cross(points[2] - points[0])).normalized();
		return Hit(t, N);
	}
	else {
		return Hit::NO_HIT();
	}

}
