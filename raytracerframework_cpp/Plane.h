#ifndef PLANE_H
#define PLANE_H

#include "object.h"

/**
 * Plane class :
 * Representation of a 2D plane in our raytracer
 * It derives from the Object class
 */
class Plane : public Object {
public:

	/**
	 * Standard constructor for a plane
	 * @param[in] p1, first point of the plane
	 * @param[in] p2, second point of the plane
	 * @param[in] p3, third point of the plane
	 * @param[in] vUp, up vector of the model (not used currently)
	 * @param[in] rot, rotation angle (not used currently)
	 * @return none
	 */
	Plane(Point p1, Point p2, Point p3, Vector vUp, double rot) : points{ p1,p2,p3 } {
		rotationAxis = vUp;
		rotationAngleDeg = rot;
	}

	//intersection
	virtual Hit intersect(const Ray& ray);

	//texture mapping
	std::pair<double, double> getTextureCoords(Point p, Vector rotationAxis, double rotationAngleDeg) override;
	
	//we need 3 points to mathematically define a plane in 3D space
	const Point points[3];

};

#endif /* end of include guard: PLANE_H */
