#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "object.h"

/**
 * Triangle class :
 * Representation of a triangle in our raytracer. This class is one of the most important as triangles are the base mesh for a lot of more complex models
 * It derives from the Object class
 */
class Triangle : public Object{
public:

	/**
	 * Standard constructor for a triangle
	 * @param[in] p1, first point of the triangle
	 * @param[in] p2, second point of the triangle
	 * @param[in] p3, third point of the triangle
	 * @return none
	 */
	Triangle(Point p1, Point p2, Point p3) : points{p1,p2,p3} { }

	//intersection
    virtual Hit intersect(const Ray &ray);

	//We need 3 points to define a triangle
    const Point points[3];
	
};

#endif /* end of include guard: TRIANGLE_H */
