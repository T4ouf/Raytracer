#include "model.h"

Model::Model(Point p, double r, double rot, Vector up) : position(p), boundingSphere(p, r, rot, up) {}

Hit Model::intersect(const Ray& ray){

	Hit h = boundingSphere.intersect(ray);
	Hit closestHit = Hit::NO_HIT();

	//bounding sphere to detect if the ray is not hiting the sphere
	if (h.no_hit) {
		return Hit::NO_HIT();
	}
	
	for (Triangle t : meshes) {
		h = t.intersect(ray);
		
		//if we hit one of the triangle => no need to look for the other triangles
		if (closestHit.no_hit && !h.no_hit) {
			closestHit = h;
		}
		else if(!closestHit.no_hit && !h.no_hit){

			if (closestHit.t >= h.t) {
				closestHit = h;
			}
		}

	}

	return closestHit;
}

//std::pair<double, double> Model::getTextureCoords(Point p, Vector rotationAxis, double rotationAngleDeg)
//{
//	return std::pair<double, double>();
//}
