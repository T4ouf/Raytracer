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


#include "scene.h"
#include "material.h"
#include <omp.h>

/**
 * Method that defines if the hit point of a ray is hidden (regarding a given light source)
 * @param[in] ray, the ray from the object to the light source l
 * @param[in] l, the light source regarding which we check if the ray's hit point is influenced by it
 * @return a boolean stating if the ray's hit point is on the hidden surface of the object
 */
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
	   
	Point hit = ray.at(min_hit.t);	//the hit point

	double distObj = (hit-ray.O).length();
	double distLight = (l.position - ray.O).length();

	//if there is an object between us and the light source...
	if (distObj < distLight) { 
		//... then our surface is "hidden" the sense that it requires shadow computation
		return true;
	}

	return false;

}

/**
 * Method that computes the color of a given pixel (depending on the ray shot)
 * @param[in] ray, the ray that the raytracer has shot
 * @param[in] recurDepth, integer that defines how many times "bounces" the ray that we shot
 * @return color, the color of the current pixel (where the ray went from)
 */
Color Scene::trace(const Ray &ray, int recurDepth){
    // looking for hit object and distance
    Hit min_hit(std::numeric_limits<double>::infinity(),Vector());
    Object *obj = NULL;
    for (unsigned int i = 0; i < objects.size(); ++i) {
        Hit hit(objects[i]->intersect(ray));
        if (hit.t<min_hit.t) {
            min_hit = hit;
            obj = objects[i];
        }
    }

    // No hit? Return black background color.
    if (!obj) return Color(0.0, 0.0, 0.0);

    Material *material = obj->material;            //the hit object's material
    Point hit = ray.at(min_hit.t);                 //the hit point
    Vector N = min_hit.N;                          //the normal at hit point
    Vector V = -ray.D;                             //the view vector (used in standard graphics)


    /****************************************************
    * Color calculation phase
    *
    * Given: material, hit, N, V, lights[]
    * Sought: color
	*
    *****************************************************
    *        Triple.dot(Vector) dot product
    *        Vector+Vector      vector sum
    *        Vector-Vector      vector difference
    *        Point-Point        yields vector
    *        Vector.normalize() normalizes vector, returns length
    *        double*Color        scales each color component (r,g,b)
    *        Color*Color        dito
    *        pow(a,b)           a to the power of b
    ****************************************************/

	Color color = Color(0.0,0.0,0.0); //pixel color

	// Phong shading
	if (this->type == PHONG) {

		//We use either the texture pixel color or the material color
		Color mat;

		//Ensure normalization
		N = N.normalized();
		V = V.normalized();
	
		Vector Diffuse = Color(0.0, 0.0, 0.0);
		Vector Specular= Color(0.0, 0.0, 0.0); //Note : The specular value doesn't include the object color
		
		std::pair<double, double> UV = obj->getTextureCoords(hit, obj->rotationAxis, obj->rotationAngleDeg); //retrieve texture coords

		Color ambientOcclusionFactor = Color(1.0,1.0,1.0);	//ambiant occlusion factor (no AO by default)

		//Question : No Phong Shading if the object has a texture ?
		if (obj->material->texture != NULL) {
			
			mat = Color(obj->material->texture->colorAt(UV.first, UV.second));
			//material->ks = 0;
		}
		else {
			mat = material->color;
		}

		Vector Ambient = material->ka * mat; //compute ambiant light

		//modify the normal if the object has a bump texture
		if (obj->material->bumpMap != NULL) {
			N = (N + (2.0 * obj->material->bumpMap->colorAt(UV.first, UV.second) - 1)).normalized();
		}

		//update ambiant occlusion factor if the object has an ambiant occlusion map
		if (obj->material->ambOccMap != NULL) {
			ambientOcclusionFactor = obj->material->ambOccMap->colorAt(UV.first, UV.second);
			//Ambient = Color(0, 0, 0);
		}
				
		color += Ambient;	//adding ambiant light to the color

		//Compute diffuse and Specular part for each light source
		for (auto l : lights) {

			//We compute Light vector
			Vector L = (l->position - hit).normalized();

			//We compute the reflected ray vector
			Vector R = 2.0 * (N.dot(L)) * N - L;
			R = R.normalized();

			//do we compute diffuse and specular ? (false for hidden surface in the shadow)
			bool diffSpecOK = true;

			//if the scene needs to compute shadows...
			if (shadowComputation) {
				diffSpecOK = !hiddenSurface(Ray(hit, L), *l); //... we look for an obstacle between the hit point and the light source
			}

			//adding the diffuse component
			auto input = L.dot(N) / (L.length() * N.length());
			if (L.dot(N) > 0.0 && diffSpecOK) {
				color += material->kd * l->color * input * mat * ambientOcclusionFactor;
			}

			// adding the specular component
			if (R.dot(V) > 0.0 && diffSpecOK) {
				color += material->ks * pow((R.dot(V)), material->n) * l->color;
			}
		}


		//We compute the reflected ray vector (recursive)
		if (recurDepth < maxRecurDepth - 1) {
			Vector R2 = -2.0 * (N.dot(V)) * N + V;
			R2 = R2.normalized();
			Color addedColor = trace(Ray(hit, -R2), recurDepth + 1);
			color += material->ks * addedColor;
		}
		if (recurDepth >= maxRecurDepth - 1) {
			color += Color(0.0, 0.0, 0.0);
		}

	}
	// Normal shading (debug mode)
	else if (this->type == NORMALS) {

		std::pair<double, double> UV = obj->getTextureCoords(hit, obj->rotationAxis, obj->rotationAngleDeg);

		//if the object has a bump map => modify the normal value
		if (obj->material->bumpMap != NULL) {
			N = (N + (2.0*obj->material->bumpMap->colorAt(UV.first, UV.second)-1.0)).normalized();
		}
		
		Color normals = (N + 1.0f) / 2.0f;
		color = normals;
	}
	// Z-buffer shading (debug mode)
	else if (this->type == ZBUFFER) {

		double farClipping	= -100;
		double closeClipping= 500;

		//distance between the clipping planes
		double distance = sqrt((farClipping - closeClipping) * (farClipping - closeClipping));

		double I = (hit - camera.eye).length();
		double colorValue = (1 - (I - closeClipping) / (distance));

		color = Color(colorValue, colorValue, colorValue);
		
	}
	// UV shading (debug mode)
	else if (this->type == TEXTURECOORDS) {

		std::pair<double,double> UV = obj->getTextureCoords(hit, obj->rotationAxis, obj->rotationAngleDeg);
		color = Color(UV.first, 0.0f, UV.second);

	}
	// Gooch shading (cf. https://users.cs.northwestern.edu/~bgooch/PDFs/gooch98.pdf)
	else if (this->type == GOOCH) {

		//We use either the texture pixel color or the material color
		Color mat;

		//if the object has a texture (not standardized in the gooch shading process)"
		if (obj->material->texture != NULL) {
			std::pair<double, double> UV = obj->getTextureCoords(hit, obj->rotationAxis, obj->rotationAngleDeg);
			mat = Color(obj->material->texture->colorAt(UV.first, UV.second));
			material->ks = 0.0;
		}
		else {
			mat = material->color;
		}

		//Ensure normalization
		N = N.normalized();
		V = V.normalized();

		double angle = N.dot(V);
		//Basic technique for outlining (based on normal buffer)
		if (angle < 0.3 && angle > -0.3) {
			return Color(0.0, 0.0, 0.0);
		}

		Vector Diffuse = Color(0.0, 0.0, 0.0);
		Vector Specular = Color(0.0, 0.0, 0.0); //The specular value doesn't include the object color
		//color += material->ka * mat;

		//Compute diffuse and Specular part for each light source
		for (auto l : lights) {

			//We compute Light vector
			Vector L = (l->position - hit).normalized();

			//We compute the reflected ray vector
			Vector R = 2.0 * (N.dot(L)) * N - L;
			R = R.normalized();

			Color diffGooch = l->color * mat * material->kd;
			Color kCool = Color(0.0f, 0.0f, gooch->b) + gooch->alpha * diffGooch;
			Color kWarm = Color(gooch->y, gooch->y, 0.0f) + gooch->beta * diffGooch;

			//do we compute diffuse and specular ? (false for hidden surface in the shadow)
			bool diffSpecOK = true;

			if (shadowComputation) {
				diffSpecOK = !hiddenSurface(Ray(hit, L), *l);
			}

			color += kCool*(1.0 - N.dot(L))/2.0 + kWarm*(1.0 + N.dot(L))/2.0; //compute the pixel color
			
			// adding the specular componenent
			if (R.dot(V) > 0.0 && diffSpecOK) {
				color += material->ks * pow((R.dot(V)), material->n) * l->color;
			}
		}


		//We compute the reflected ray vector (recursive)
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
	// Blinn-Phong shading (cf. http://cs.uns.edu.ar/cg/clasespdf/p192-blinn.pdf)
	else if(this->type == BLINNPHONG) {

		//We use either the texture pixel color or the material color
		Color mat;
		//Ensure normalization
		N = N.normalized();
		V = V.normalized();

		Vector Diffuse = Color(0.0, 0.0, 0.0);
		Vector Specular = Color(0.0, 0.0, 0.0); //The specular value doesn't include the object color

		std::pair<double, double> UV = obj->getTextureCoords(hit, obj->rotationAxis, obj->rotationAngleDeg);

		Color ambientOcclusionFactor = Color(1.0, 1.0, 1.0);

		if (obj->material->texture != NULL) {

			mat = Color(obj->material->texture->colorAt(UV.first, UV.second));
			//material->ks = 0;
		}
		else {
			mat = material->color;
		}

		Vector Ambient = material->ka * mat;

		//modify the normal if the object has a bump map
		if (obj->material->bumpMap != NULL) {
			N = (N + (2.0 * obj->material->bumpMap->colorAt(UV.first, UV.second) - 1)).normalized();
		}

		//update ambiant occlusion factor if the object has an ambiant occlusion map
		if (obj->material->ambOccMap != NULL) {
			ambientOcclusionFactor = obj->material->ambOccMap->colorAt(UV.first, UV.second);
			//Ambient = Color(0, 0, 0);
		}

		color += Ambient;	//Adding ambient component

		//Compute diffuse and Specular part for each light source
		for (auto l : lights) {

			//We compute Light vector
			Vector L = (l->position - hit).normalized();

			//We compute the halfway vector
			Vector H = (L + V) / (L+V).length() ;
			
			//do we compute diffuse and specular ? (false for hidden surface in the shadow)
			bool diffSpecOK = true;

			if (shadowComputation) {
				diffSpecOK = !hiddenSurface(Ray(hit, L), *l);
			}

			//adding the diffuse component
			auto input = L.dot(N) / (L.length() * N.length());
			if (L.dot(N) > 0.0 && diffSpecOK) {
				color += material->kd * l->color * input * mat * ambientOcclusionFactor;
			}

			// adding the specular componenent
			if (diffSpecOK) {
				color += material->ks * pow((N.dot(H)), 2*material->n) * l->color;
			}
		}


		//We compute the reflected ray vector (recursive)
		if (recurDepth < maxRecurDepth - 1) {
			Vector R2 = -2.0 * (N.dot(V)) * N + V;
			R2 = R2.normalized();
			Color addedColor = trace(Ray(hit, -R2), recurDepth + 1);
			color += material->ks * addedColor;
		}
		if (recurDepth >= maxRecurDepth - 1) {
			color += Color(0.0, 0.0, 0.0);
		}

	}
	// Cel shading
	else if (this->type == CEL) {

		//We use either the texture pixel color or the material color
		Color mat;
	
		//Ensure normalization
		N = N.normalized();
		V = V.normalized();
		double angle = N.dot(V);
		//Primary technique for outlining
		if (angle < 0.3 && angle > -0.3) {
			return Color(0.0, 0.0, 0.0);
		}

		std::pair<double, double> UV = obj->getTextureCoords(hit, obj->rotationAxis, obj->rotationAngleDeg);

		Color ambientOcclusionFactor = Color(1.0, 1.0, 1.0);

		//No Phong Shading if the object has a texture
		if (obj->material->texture != NULL) {

			mat = Color(obj->material->texture->colorAt(UV.first, UV.second));
			//material->ks = 0;
		}
		else {
			mat = material->color;
		}

		if (obj->material->bumpMap != NULL) {
			N = (N + (2.0 * obj->material->bumpMap->colorAt(UV.first, UV.second) - 1)).normalized();
		}

		//update ambiant occlusion factor if the object has an ambiant occlusion map
		if (obj->material->ambOccMap != NULL) {
			ambientOcclusionFactor = obj->material->ambOccMap->colorAt(UV.first, UV.second);
			//Ambient = Color(0, 0, 0);
		}

		obj->material->color = Color(1.0, 1.0, 1.0);

		Vector Diffuse = Color(0.0, 0.0, 0.0);
		Vector Specular = Color(0.0, 0.0, 0.0); //The specular value doesn't include the object color
		//color += material->ka * mat;

		//Compute diffuse and Specular part for each light source
		for (auto l : lights) {

			//We compute Light vector
			Vector L = (l->position - hit).normalized();

			//We compute the incomming ray vector
			//Vector I = (-L).normalized();

			//We compute the reflected ray vector
			Vector R = 2.0 * (N.dot(L)) * N - L;
			R = R.normalized();

			//do we compute diffuse and specular ? (false for hidden surface in the shadow)
			bool diffSpecOK = true;

			if (shadowComputation) {
				diffSpecOK = !hiddenSurface(Ray(hit, L), *l);
			}

			//adding the diffuse component
			auto input = L.dot(N) / (L.length() * N.length());
			if (L.dot(N) > 0.0 && diffSpecOK) {
				color += material->kd * l->color * input * mat * ambientOcclusionFactor;
			}

			// adding the specular componenent
			if (R.dot(V) > 0.0 && diffSpecOK) {
				//color += material->ks * pow((R.dot(V)), material->n) * l->color;
			}
		}

		//we take the intensity value to search into the cel map
		double intensity = (color.r + color.g + color.b)/3.0;

		// Sanity checks
		if (intensity > 1.0) intensity = 1.0;
		if (intensity < 0.0) intensity = 0.0;

		//pixel color is defined by the value in the 1D Cel-texture
		color = obj->material->celTexture->colorAt(intensity, 0); 
	}
	// unkown shading type => we don't render
	else {
		cerr << "UNSUPPORTED SCENE TYPE : " << this->type << " !\n";
		exit(EXIT_FAILURE);
	}
		
	return color;
}

/**
 * Method that renders the scene to an image
 * @param[out] img, the result image
 * @return none
 */
void Scene::render(Image& img){

	//dimensions of the image
	long long int w = img.width();
	long long int h = img.height();

	//dimensions of a pixel
	Vector pixelWidth = camera.side;
	Vector pixelHeight = -camera.up;

	//MidPoint of the image
	Vector M = camera.c - pixelWidth * double(w) / 2.0f - pixelHeight * double(h) / 2.0f;
	
	//for each pixel of the image
	#pragma omp parallel for
	for (long long int y = 0; y < h; y++) {
		for (long long int x = 0; x < w; x++) {

			//We are going to sample a pixel color by tracing multiple rays (SSAA)
			Color SuperSamplingColor = Color(0.0f, 0.0f, 0.0f);
			Point pixel1 = M + double(x) * pixelWidth + double(y) * pixelHeight; //subpixel "position"

			//Sampling the pixel with super sampling factor
			for (int i = 0; i < camera.superSampling; i++) {
				for (int j = 0; j < camera.superSampling; j++) {

					//offset to the subpixel center in wolrd coords
					Point pixel((pixelHeight / double(camera.superSampling)) * double(i)
						+ (pixelWidth	/ double(camera.superSampling)) * double(j)
						+ (pixelWidth	/ (double(camera.superSampling) * double(camera.superSampling)))
						+ (pixelHeight	/ (double(camera.superSampling) * double(camera.superSampling))));

					//shifting to the subpixel center
					pixel += pixel1;

					//We trace the ray
					Ray ray(camera.eye, (pixel - camera.eye).normalized());
					Color col = trace(ray, 0);

					//adding the color to the other samples
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


/**
 * Method to add objects to the scene
 * @param[in] o, the object (pointer) that we want to add
 * @return none
 */
void Scene::addObject(Object *o)
{
    objects.push_back(o);
}

/**
 * Method to add light sources to the scene
 * @param[in] l, the light source (pointer) that we want to add
 * @return none
 */
void Scene::addLight(Light *l)
{
    lights.push_back(l);
}

/**
 * Setter for the scene's camera
 * @param[in] c, the camera (pointer) that we want to set
 * @return none
 */
void Scene::setCamera(Camera* c)
{
	this->camera = *c;
}

/**
 * Setter for the shading type
 * @param[in] r, raytracing type (enum value)
 * @return none
 */
void Scene::setRaytracingType(raytracingType r) {
	this->type = r;
}

/**
 * Setter that defines if the scene "contains" shadows
 * @param[in] shadow, the boolean stating if we should compute shadows or not
 * @return none
 */
void Scene::setShadowBool(bool shadow) {
	this->shadowComputation = shadow; 
}

/**
 * Setter that defines how many times "bounces" the ray that we shoot
 * @param[in] recursionLimit, the limit for the "bouncing" recursion
 * @return none
 */
void Scene::setMaxRecursion(int recursionLimit) {
	this->maxRecurDepth = recursionLimit;
}

/**
 * Setter for Gooch parameters in Gooch Shading
 * @param[in] g, the gooch parameters as they are required in the struct type
 * @return none
 */
void Scene::setGoochParams(const GoochParameters& g) {
	gooch->alpha = g.alpha;
	gooch->beta = g.beta;
	gooch->b = g.b;
	gooch->y = g.y;
}