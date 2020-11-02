#include "plane.h"
#include <iostream>
#include <math.h>

/************************** Plane **********************************/

/**
 * Method that defines the hit point between the plane and a given ray
 * @param[in] ray, the ray we want to compute the intersection
 * @return the hit point and all its data (or Hit::NO_HIT() if no hit point)
 * @see https://www.cs.princeton.edu/courses/archive/fall00/cs426/lectures/raycast/sld017.htm
 */
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

/**
 * Method that gives the UV coordinates of a given point on the plane
 * @param[in] p, the point where the UV coordinates are needed
 * @param[in] rotationAxis, the the rotation axis of the plane (not used currently)
 * @param[in] rotationAngleDeg, the rotation angle (not used currently)
 * @return the UV coordinates as a pair of doubles
 */
std::pair<double, double> Plane::getTextureCoords(Point p, Vector rotationAxis, double rotationAngleDeg) {

	//Horizontal vector
	Vector P1P2 = (this->points[1] - this->points[0]);
	//Vertical Vector 
	Vector P1P3 = (this->points[2] - this->points[0]);
	//Vector to the point
	Vector P1P = (p - this->points[0]);

	double integerPart = 0.0;

	double u = P1P.dot(P1P2.normalized()) / P1P2.length();
	u = u - floor(u);
	//u = modf(u, &integerPart);

	double v = -P1P.dot(P1P3.normalized()) / P1P3.length();
	v = v - floor(v);

	//u = (u + 1.0) / 2.0;
	//v = (v + 1.0) / 2.0;

	return { u,v };

}