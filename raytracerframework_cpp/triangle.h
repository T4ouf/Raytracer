
//---------------------------------------------------//
//		ADVANCED GRAPHICS ASSIGNMENT (ET5 info)		 //
//              THOMAS VON ASCHEBERG                 //
//					 MY-LINH HO		                 //
//---------------------------------------------------//

#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "object.h"

class Triangle : public Object{
public:
	Triangle(Point p1, Point p2, Point p3) : points{p1,p2,p3} { }

    virtual Hit intersect(const Ray &ray);

	//We need 3 points to define a triangle
    const Point points[3];
	
};

#endif /* end of include guard: TRIANGLE_H */
