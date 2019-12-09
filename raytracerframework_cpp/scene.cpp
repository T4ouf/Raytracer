//
//  Framework for a raytracer
//  File: scene.cpp
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

//---------------------------------------------------//
//		ADVANCED GRAPHICS ASSIGNMENT (ET5 info)		 //
//              THOMAS VON ASCHEBERG                 //
//					 MY-LINH HO		                 //
//---------------------------------------------------//

#include "scene.h"
#include "material.h"

bool Scene::hiddenSurface(const Ray &ray, Light& l) {
	// Find hit object and distance
	Hit min_hit(std::numeric_limits<double>::infinity(), Vector());
	Object *obj = NULL;
	for (unsigned int i = 0; i < objects.size(); ++i) {
		Hit hit(objects[i]->intersect(ray));
		if (hit.t < min_hit.t && hit.t>0) {
			min_hit = hit;
			obj = objects[i];
		}
	}
	   
	Point hit = ray.at(min_hit.t);                 //the hit point

	double distObj = (hit-ray.O).length();
	double distLight = (l.position - ray.O).length();

	if (distObj < distLight) {
		return true;
	}

	return false;

}

Color Scene::trace(const Ray &ray, int recurDepth){
    // Find hit object and distance
    Hit min_hit(std::numeric_limits<double>::infinity(),Vector());
    Object *obj = NULL;
    for (unsigned int i = 0; i < objects.size(); ++i) {
        Hit hit(objects[i]->intersect(ray));
        if (hit.t<min_hit.t) {
            min_hit = hit;
            obj = objects[i];
        }
    }

    // No hit? Return background color.
    if (!obj) return Color(0.0, 0.0, 0.0);

    Material *material = obj->material;            //the hit objects material
    Point hit = ray.at(min_hit.t);                 //the hit point
    Vector N = min_hit.N;                          //the normal at hit point
    Vector V = -ray.D;                             //the view vector


    /****************************************************
    * This is where you should insert the color
    * calculation (Phong model).
    *
    * Given: material, hit, N, V, lights[]
    * Sought: color
    *
    * Hints: (see triple.h)
    *        Triple.dot(Vector) dot product
    *        Vector+Vector      vector sum
    *        Vector-Vector      vector difference
    *        Point-Point        yields vector
    *        Vector.normalize() normalizes vector, returns length
    *        double*Color        scales each color component (r,g,b)
    *        Color*Color        dito
    *        pow(a,b)           a to the power of b
    ****************************************************/

	Color color = Color(0,0,0); //pixel color

	if (this->type == COLORED) {

		//Ensure normalization
		N = N.normalized();
		V = V.normalized();
	
		Vector Diffuse = Color(0, 0, 0);
		Vector Specular= Color(0, 0, 0); //The specular value doesn't include the object color
		color += material->ka * material->color;

		//Compute diffuse and Specular part for each light source
		/**/
		for (auto l : lights) {

			//We compute Light vector
			Vector L = (l->position - hit).normalized();

			//We compute the incomming ray vector
			//Vector I = (-L).normalized();

			//We compute the reflected ray vector
			Vector R = 2 * (N.dot(L)) * N - L;
			R = R.normalized();

			bool diffSpecOK = true;

			if (shadowComputation) {
				diffSpecOK = !hiddenSurface(Ray(hit, L), *l);
			}

			//adding the diffuse component
			auto input = L.dot(N) / (L.length() * N.length());
			if (L.dot(N) > 0.0 && diffSpecOK) {
				color += material->kd * l->color * input * material->color;
			}

			// adding the specular componenent
			if (R.dot(V) > 0.0 && diffSpecOK) {
				color += material->ks * pow((R.dot(V)), material->n) * l->color;
			}
		}


		//We compute the reflected ray vector
		/*Vector reflectedRay = (2 * (min_hit.N.dot(-ray.D)) * min_hit.N + ray.D).normalized();
		color += reflection(Ray(hit, reflectedRay.normalized()), reflectionNumber) * obj->material->ks;
		*/

		if (recurDepth < maxRecurDepth - 1) {
			Vector R2 = -2 * (N.dot(V)) * N + V;
			R2 = R2.normalized();
			Color addedColor = trace(Ray(hit, -R2), recurDepth + 1);
			color += material->ks * addedColor;
		}
		if (recurDepth >= maxRecurDepth - 1)
			color += Color(0.0, 0.0, 0.0);



	}
	else if (this->type == NORMALS) {
		Color normals = (N + 1.0f) / 2.0f;
		color = normals;
	}
	else if (this->type == ZBUFFER) {

		double farClipping	= -100;
		double closeClipping= 500;

		//distance between the clipping planes
		double distance = sqrt((farClipping - closeClipping) * (farClipping - closeClipping));

		double I = (hit - camera.eye).length();
		double colorValue = (1 - (I - closeClipping) / (distance));

		color = Color(colorValue, colorValue, colorValue);
		
	}
	else {
		cerr << "UNSUPPORTED SCENE TYPE : " << this->type << " !\n";
		exit(EXIT_FAILURE);
	}
		
	return color;
}

void Scene::render(Image &img)
{
    long long int w = img.width();
	long long int h = img.height();
    for (long long int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            Point pixel(x+0.5, h-1-y+0.5, 0);
            Ray ray(camera.eye, (pixel-camera.eye).normalized());
            Color col = trace(ray,0);
            col.clamp();
            img(x,y) = col;
        }
    }
}

void Scene::addObject(Object *o)
{
    objects.push_back(o);
}

void Scene::addLight(Light *l)
{
    lights.push_back(l);
}

void Scene::setCamera(Camera* c)
{
	this->camera = *c;
}

void Scene::setRaytracingType(raytracingType r) {
	this->type = r;
}

void Scene::setShadowBool(bool shadow) {
	this->shadowComputation = shadow; 
}

void Scene::setMaxRecursion(int recursionLimit) {
	this->maxRecurDepth = recursionLimit;
}