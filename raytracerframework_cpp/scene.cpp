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

Color Scene::trace(const Ray &ray)
{
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
		Vector Ambient = material->ka * material->color;

		//Compute diffuse and Specular part for each light source
		for (auto l : lights) {

			//We compute Light vector
			Vector L = (l->position - hit);
			double diffuseLightAttenuation = L.dot(N) / L.length();

			//We compute the incomming ray vector
			Vector I = (-L).normalized();

			//We compute the reflected ray vector
			Vector R = I - 2 * (N.dot(I)) * N;
			R = R.normalized();

			// if the hitpoint is behind the sphere (aka the light cannot reach it => no diffuse component)
			if (L.dot(N) < 0) {
				Diffuse = Diffuse * 0;
				Specular = Specular * 0;
				continue;
			}

			//Otherwhise we compute the diffuse part
			Diffuse += material->kd * material->color * L.normalized().dot(N.normalized()) * l->color;

			if (R.dot(V) <= 0) {
				Specular = Specular * 0;
				continue;
			}

			//we compute the specular component
			Specular += material->ks * pow(R.dot(V), material->n) * l->color;
			Ambient = Ambient * l->color;
		}

		//Phong Shading
		color += Ambient + Diffuse + Specular;

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

		double I = (hit - eye).length();
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
            Ray ray(eye, (pixel-eye).normalized());
            Color col = trace(ray);
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

void Scene::setEye(Triple e)
{
    eye = e;
}

void Scene::setRaytracingType(raytracingType r) {
	this->type = r;
}
