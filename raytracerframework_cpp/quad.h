#pragma once
#include "object.h"
#include "triple.h"

class Quad : public Object {
public:
	Quad(Point p1, Point p2, Point p3, Point p4) : points{p1,p2,p3,p4} { }

	Point points[4];
	Hit intersect(const Ray& ray) override;

};

