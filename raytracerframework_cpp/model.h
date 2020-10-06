#pragma once
#include "object.h"
#include "triangle.h"
#include "sphere.h"
#include <vector>

/**
 * Model class :
 * Representation of a 3D model in our raytracer
 * It derives from the Object class 
 */
class Model : public Object {
public:

	// A 3D model is just a list of triangles
	std::vector<Triangle> meshes;	
	//std::vector<std::pair<double, double>> UVcoords; //TODO implement texture for 3D model (start with triangle)
	
	Point position;
	
	// The bounding sphere is a basic big hitbox to simplify ray-model intersection (in order not to compute intersection for every ray/triangle combination)
	Sphere boundingSphere;	

	Model(Point p, double r, double rot, Vector up);

	Hit intersect(const Ray& ray) override;
	//std::pair<double, double> getTextureCoords(Point p, Vector rotationAxis, double rotationAngleDeg) override; //TODO implement texture for 3D model (start with triangle)

};

