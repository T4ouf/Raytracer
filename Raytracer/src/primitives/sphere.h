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


#ifndef SPHERE_H_115209AE
#define SPHERE_H_115209AE

#include "object.h"

/**
 * Sphere class :
 * Representation of a Sphere in our raytracer
 * It derives from the Object class
 */
class Sphere : public Object {
public:

	/**
	 * Standard constructor for a sphere
	 * @param[in] position, Position of the center of the sphere
	 * @param[in] r, radius of the sphere
	 * @param[in] rot, rotation angle (for textures)
	 * @param[in] vUp, up vector of the model (for textures)
	 * @return none
	 */
    Sphere(Point position,double r, double rot, Vector vUp) : position(position), radius(r) {
		rotationAxis = vUp;
		rotationAngleDeg = rot;
	}

    virtual Hit intersect(const Ray &ray);
	virtual std::pair<double, double> getTextureCoords(Point p, Vector rotationAxis, double rotationAngleDeg) override;

private:
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
