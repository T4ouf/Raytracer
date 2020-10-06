#include "model.h"

/**
 * Standard constructor for a 3D-model
 * @param[in] p, the position in world coordinates of our model
 * @param[in] r, the bounding sphere radius (for basic intersection)
 * @param[in] rot, rotation angle (not used currently)
 * @param[in] up, up vector of the model (not used currently)
 * @return none
 */
Model::Model(Point p, double r, double rot, Vector up) : position(p), boundingSphere(p, r, rot, up) {}


/**
 * Method that defines the hit point between the 3D-model and a given ray
 * @param[in] ray, the ray we want to compute the intersection
 * @return the hit point and all its data (or Hit::NO_HIT() if no hit point)
 */
Hit Model::intersect(const Ray& ray){

	// First to save a lot of useless computation, we check if the ray is close from our model
	// to do so, we check if the ray intersect the bounding sphere (bigger than the model itself)
	// ray-sphere intersection is quite cheap (2 dot product + 2 sqrt)
	Hit h = boundingSphere.intersect(ray);
	Hit closestHit = Hit::NO_HIT();

	//if the ray is not hitting the sphere...
	if (h.no_hit) {
		//... then it cannot hit the model => we don't compute intersection per triangle => save computation
		return Hit::NO_HIT();
	}
	
	// if it did hit the bounding sphere => we need to check each triangle to see if there is an intersection
	for (Triangle t : meshes) {
		h = t.intersect(ray);
		
		//if we hit one of the triangle => we need to be sure that it is the closest triangle (not behind) => then no need to look for the other triangles => TODO
		//currently we use a method to find the closest hit point (by checking all the triangles)
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

//TODO
//std::pair<double, double> Model::getTextureCoords(Point p, Vector rotationAxis, double rotationAngleDeg)
//{
//	return std::pair<double, double>();
//}
