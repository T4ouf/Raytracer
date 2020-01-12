#pragma once
#include "object.h"
#include "triangle.h"
#include "sphere.h"
#include <vector>


class Model : public Object {
public:
	std::vector<Triangle> meshes;
	//std::vector<std::pair<double, double>> UVcoords;
	Point position;

	Sphere boundingSphere;

	Model(Point p, double r, double rot, Vector up);

	Hit intersect(const Ray& ray) override;
	//std::pair<double, double> getTextureCoords(Point p, Vector rotationAxis, double rotationAngleDeg) override;

};

