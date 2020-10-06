#include "quad.h"
#include "triangle.h"

/**
 * Method that defines the hit point between Quad and a given ray
 * @param[in] ray, the ray we want to compute the intersection
 * @return the hit point and all its data (or Hit::NO_HIT() if no hit point)
 */
Hit Quad::intersect(const Ray& ray){

	//we can consider a quad as 2 two triangles => rely on triangle intersection
	Triangle t1 = Triangle(points[0], points[1], points[2]);
	Triangle t2 = Triangle(points[2], points[3], points[0]);

	//does the ray hit any of the triangle ?
	Hit h1 = t1.intersect(ray);
	Hit h2 = t2.intersect(ray);

	//looking for the right hit point to return
	if (h1.no_hit && !h2.no_hit) {
		return h2;
	}
	else if (!h1.no_hit && h2.no_hit) {
		return h1;
	}
	else if (!h1.no_hit && !h2.no_hit) { //very unlikely to happen (parallel ray)

		Vector d1 = ray.O + h1.t * ray.D;
		Vector d2 = ray.O + h2.t * ray.D;
		
		if (d1.length() < d2.length()) {
			return h1;
		}
		else {
			return h2;
		}
	}
	else {
		return Hit::NO_HIT();
	}
	
	return Hit::NO_HIT();
	
}
