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
#include <omp.h>


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

	if (this->type == PHONG) {

		//We use either the texture pixel color or the material color
		Color mat;

		std::pair<double, double> UV = obj->getTextureCoords(hit, obj->rotationAxis, obj->rotationAngleDeg);

		//No Phong Shading if the object has a texture
		if (obj->material->texture != NULL) {
			
			mat = Color(obj->material->texture->colorAt(UV.first, UV.second));
			//material->ks = 0;
		}
		else {
			mat = material->color;
		}

		if (obj->material->bumpMap != NULL) {
			N = (N + (2 * obj->material->bumpMap->colorAt(UV.first, UV.second) - 1)).normalized();
		}

		//Ensure normalization
		N = N.normalized();
		V = V.normalized();
	
		Vector Diffuse = Color(0, 0, 0);
		Vector Specular= Color(0, 0, 0); //The specular value doesn't include the object color
		color += material->ka * mat;

		//Compute diffuse and Specular part for each light source
		for (auto l : lights) {

			//We compute Light vector
			Vector L = (l->position - hit).normalized();

			//We compute the incomming ray vector
			//Vector I = (-L).normalized();

			//We compute the reflected ray vector
			Vector R = 2 * (N.dot(L)) * N - L;
			R = R.normalized();

			//do we compute diffuse and specular ? (false for hidden surface in the shadow)
			bool diffSpecOK = true;

			if (shadowComputation) {
				diffSpecOK = !hiddenSurface(Ray(hit, L), *l);
			}

			//adding the diffuse component
			auto input = L.dot(N) / (L.length() * N.length());
			if (L.dot(N) > 0.0 && diffSpecOK) {
				color += material->kd * l->color * input * mat;
			}

			// adding the specular componenent
			if (R.dot(V) > 0.0 && diffSpecOK) {
				color += material->ks * pow((R.dot(V)), material->n) * l->color;
			}
		}


		//We compute the reflected ray vector
		if (recurDepth < maxRecurDepth - 1) {
			Vector R2 = -2 * (N.dot(V)) * N + V;
			R2 = R2.normalized();
			Color addedColor = trace(Ray(hit, -R2), recurDepth + 1);
			color += material->ks * addedColor;
		}
		if (recurDepth >= maxRecurDepth - 1) {
			color += Color(0.0, 0.0, 0.0);
		}

	}
	else if (this->type == NORMALS) {

		std::pair<double, double> UV = obj->getTextureCoords(hit, obj->rotationAxis, obj->rotationAngleDeg);

		if (obj->material->bumpMap != NULL) {
			N = (N + (2*obj->material->bumpMap->colorAt(UV.first, UV.second)-1)).normalized();
		}
		
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
	else if (this->type == TEXTURECOORDS) {

		std::pair<double,double> UV = obj->getTextureCoords(hit, obj->rotationAxis, obj->rotationAngleDeg);
		color = Color(UV.first, 0.0f, UV.second);

	}
	else if (this->type == GOOCH) {

		//We use either the texture pixel color or the material color
		Color mat;

		//No Phong Shading if the object has a texture
		if (obj->material->texture != NULL) {
			std::pair<double, double> UV = obj->getTextureCoords(hit, obj->rotationAxis, obj->rotationAngleDeg);
			mat = Color(obj->material->texture->colorAt(UV.first, UV.second));
			material->ks = 0;
		}
		else {
			mat = material->color;
		}

		//Ensure normalization
		N = N.normalized();
		V = V.normalized();

		Vector Diffuse = Color(0, 0, 0);
		Vector Specular = Color(0, 0, 0); //The specular value doesn't include the object color
		//color += material->ka * mat;

		//Compute diffuse and Specular part for each light source
		for (auto l : lights) {

			//We compute Light vector
			Vector L = (l->position - hit).normalized();

			//We compute the incomming ray vector
			//Vector I = (-L).normalized();

			//We compute the reflected ray vector
			Vector R = 2 * (N.dot(L)) * N - L;
			R = R.normalized();

			Color diffGooch = l->color * mat * material->kd;
			Color kCool = Color(0.0f, 0.0f, gooch->b) + gooch->alpha * diffGooch;
			Color kWarm = Color(gooch->y, gooch->y, 0.0f) + gooch->beta * diffGooch;

			//do we compute diffuse and specular ? (false for hidden surface in the shadow)
			bool diffSpecOK = true;

			if (shadowComputation) {
				diffSpecOK = !hiddenSurface(Ray(hit, L), *l);
			}

			color += kCool*(1.0 - N.dot(L))/2.0 + kWarm*(1.0 + N.dot(L))/2.0;
			
			// adding the specular componenent
			if (R.dot(V) > 0.0 && diffSpecOK) {
				color += material->ks * pow((R.dot(V)), material->n) * l->color;
			}
		}


		//We compute the reflected ray vector
		if (recurDepth < maxRecurDepth - 1) {
			Vector R2 = -2 * (N.dot(V)) * N + V;
			R2 = R2.normalized();
			Color addedColor = trace(Ray(hit, -R2), recurDepth + 1);
			color += material->ks * addedColor;
		}
		if (recurDepth >= maxRecurDepth - 1) {
			color += Color(0.0, 0.0, 0.0);
		}

	}
	else {
		cerr << "UNSUPPORTED SCENE TYPE : " << this->type << " !\n";
		exit(EXIT_FAILURE);
	}
		
	return color;
}

void Scene::render(Image& img){

	long long int w = img.width();
	long long int h = img.height();

	Vector pixelWidth = camera.side;
	Vector pixelHeight = -camera.up;

	//MidPoint of the image
	Vector M = camera.c - pixelWidth * double(w) / 2.0f - pixelHeight * double(h) / 2.0f;
	
	//for each pixel of the image
	#pragma omp parallel for
	for (long long int y = 0; y < h; y++) {
		for (long long int x = 0; x < w; x++) {

			//We are going to sample a pixel color by tracing multiple rays
			Color SuperSamplingColor = Color(0.0f, 0.0f, 0.0f);
			Point pixel1 = M + double(x) * pixelWidth + double(y) * pixelHeight;

			//Sampling the pixel with super sampling factor
			for (int i = 0; i < camera.superSampling; i++) {
				for (int j = 0; j < camera.superSampling; j++) {

					//shifting to the right position of pixel in wolrd coords
					Point pixel((pixelHeight / double(camera.superSampling)) * double(i)
						+ (pixelWidth	/ double(camera.superSampling)) * double(j)
						+ (pixelWidth	/ (double(camera.superSampling) * double(camera.superSampling)))
						+ (pixelHeight	/ (double(camera.superSampling) * double(camera.superSampling))));

					pixel += pixel1;

					//We trace the ray
					Ray ray(camera.eye, (pixel - camera.eye).normalized());
					Color col = trace(ray, 0);
					//adding to the other samples
					SuperSamplingColor += col;
				}
			}
			//doing the mean of each subpixel for the current pixel
			SuperSamplingColor = SuperSamplingColor / (double(camera.superSampling) * double(camera.superSampling));
			SuperSamplingColor.clamp();
			img(x, y) = SuperSamplingColor; //print to the image file
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

void Scene::setGoochParams(const GoochParameters& g) {
	gooch->alpha = g.alpha;
	gooch->beta = g.beta;
	gooch->b = g.b;
	gooch->y = g.y;
}