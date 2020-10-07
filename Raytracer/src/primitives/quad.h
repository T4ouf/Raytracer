#pragma once
#include "object.h"
#include "triple.h"

/**
 * Quad class :
 * Representation of a Quad in our raytracer
 * It derives from the Object class
 */
class Quad : public Object {
public:

	/**
	 * Standard constructor for a quad
	 * @param[in] p1, first point of the quad
	 * @param[in] p2, second point of the quad
	 * @param[in] p3, third point of the quad
	 * @param[in] p4, fourth point of the quad
	 * @return none
	 */
	Quad(Point p1, Point p2, Point p3, Point p4) : points{p1,p2,p3,p4} { }

	//A quad is just 4 points
	Point points[4];

	//intersection
	Hit intersect(const Ray& ray) override;

};

