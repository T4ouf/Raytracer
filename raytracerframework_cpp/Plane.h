
//---------------------------------------------------//
//		ADVANCED GRAPHICS ASSIGNMENT (ET5 info)		 //
//              THOMAS VON ASCHEBERG                 //
//					 MY-LINH HO		                 //
//---------------------------------------------------//

#ifndef PLANE_H
#define PLANE_H

#include "object.h"

class Plane : public Object {
public:
	Plane(Point p1, Point p2, Point p3) : points{ p1,p2,p3} { }

	virtual Hit intersect(const Ray& ray);
	
	//we need 3 points to mathematically define a plane
	const Point points[3];

};

#endif /* end of include guard: PLANE_H */
