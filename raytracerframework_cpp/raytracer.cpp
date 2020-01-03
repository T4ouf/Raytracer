//
//  Framework for a raytracer
//  File: raytracer.cpp
//
//  Created for the Computer Science course "Introduction Computer Graphics"
//  taught at the University of Groningen by Tobias Isenberg.
//
//  Author: Maarten Everts
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

#include "raytracer.h"
#include "object.h"
#include "sphere.h"
#include "triangle.h"
#include "Plane.h"
#include "material.h"
#include "light.h"
#include "image.h"
#include "yaml/yaml.h"
#include <ctype.h>
#include <fstream>
#include <assert.h>

// Functions to ease reading from YAML input
void operator >> (const YAML::Node& node, Triple& t);
Triple parseTriple(const YAML::Node& node);

void operator >> (const YAML::Node& node, Triple& t)
{
    assert(node.size()==3);
    node[0] >> t.x;
    node[1] >> t.y;
    node[2] >> t.z;
}

Triple parseTriple(const YAML::Node& node)
{
    Triple t;
    node[0] >> t.x;
    node[1] >> t.y;
    node[2] >> t.z;	
    return t;
}

raytracingType Raytracer::parseType(const YAML::Node& node){
	int r=0;
	if (node != NULL) {
		node >> r;
	}
	return (raytracingType)r;
}

bool Raytracer::parseShadow(const YAML::Node& node) {
	bool r = false;
	node >> r;
	return r;
}

int Raytracer::parseReflectionDepth(const YAML::Node& node) {
	int r = 0;
	node >> r;
	return r;
}


Material* Raytracer::parseMaterial(const YAML::Node& node)
{
    Material *m = new Material();
	auto t = node.FindValue("texture");

	if (t == NULL) {
		m->texture = NULL;
	}
	else {

		std::string path ;
		node["texture"] >> path;
		m->texture = new Image(path.c_str());
	}

    node["color"] >> m->color;	
    node["ka"] >> m->ka;
    node["kd"] >> m->kd;
    node["ks"] >> m->ks;
    node["n"] >> m->n;
    return m;
}

Object* Raytracer::parseObject(const YAML::Node& node)
{
    Object *returnObject = NULL;
    std::string objectType;
    node["type"] >> objectType;

    if (objectType == "sphere") {
        Point pos;
        node["position"] >> pos;
        double r;
        node["radius"] >> r;

		auto upValue = node.FindValue("rotation-axis");
		Vector up = Vector(0, 1, 0);
		if (upValue != NULL) {
			up = parseTriple(*upValue).normalized();
		}
				
		/*
		Vector side = parseTriple(node["side-axis"]).normalized();
		Vector S2 = up.cross(side).normalized();
		side = S2.cross(up).normalized();
		*/
		double rot = 0;
		auto angleValue = node.FindValue("angle");
		if (angleValue != NULL) {
			*angleValue >> rot;
		}
		
        Sphere *sphere = new Sphere(pos,r,rot,up);	
        returnObject = sphere;
    }
	else if (objectType == "triangle") {
		Point p1;
		Point p2;
		Point p3;
		node["p1"] >> p1;
		node["p2"] >> p2;
		node["p3"] >> p3;
	
		Triangle* triangle = new Triangle(p1,p2,p3);
		returnObject = triangle;
	}
	else if (objectType == "plane") {
		Point p1;
		Point p2;
		Point p3;
		node["p1"] >> p1;
		node["p2"] >> p2;
		node["p3"] >> p3;

		Plane* plane = new Plane(p1, p2, p3);
		returnObject = plane;
	}

    if (returnObject) {
        // read the material and attach to object
        returnObject->material = parseMaterial(node["material"]);
    }

    return returnObject;
}

Light* Raytracer::parseLight(const YAML::Node& node)
{
    Point position;
    node["position"] >> position;
    Color color;
    node["color"] >> color;
    return new Light(position,color);
}

int Raytracer::parseSuperSampling(const YAML::Node& node) {
	int r = 0;
	node["factor"] >> r;
	return r;
}

GoochParameters Raytracer::parseGooch(const YAML::Node& node){

	double alpha = 0;
	double beta = 0;
	double b = 0;
	double y = 0;

	node["alpha"] >> alpha;
	node["beta"] >> beta;
	node["b"] >> b;
	node["y"] >> y;

	GoochParameters g = GoochParameters(alpha, beta, b, y);
	return g;
}

Camera* Raytracer::parseCamera(const YAML::Node& node) {
	Camera* c = new Camera();

	// Read scene configuration options
	c->eye = parseTriple(node["eye"]);
	c->c = parseTriple(node["center"]);
	Vector upInit = parseTriple(node["up"]);
	Vector CEye = (c->c - c->eye).normalized();
	Vector side = CEye.cross(upInit);
	c->side = side;
	c->up = side.cross(CEye);

	double width = node["viewSize"][0];
	double height = node["viewSize"][1];
	c->aspectRatio = width / height;
	c->baseline = height;	//define the baseline as the height => we will multiply the width with the AspectRatio
	
	return c;
}


/*
* Read a scene from file
*/

bool Raytracer::readScene(const std::string& inputFilename)
{
    // Initialize a new scene
    scene = new Scene();

    // Open file stream for reading and have the YAML module parse it
    std::ifstream fin(inputFilename.c_str());
    if (!fin) {
        cerr << "Error: unable to open " << inputFilename << " for reading." << endl;;
        return false;
    }
    try {
        YAML::Parser parser(fin);
        if (parser) {
            YAML::Node doc;
            parser.GetNextDocument(doc);
			
			auto renderType = doc.FindValue("RaytracingType");
			if (renderType != NULL) {
				scene->setRaytracingType(parseType(*renderType));
			}
			else {
				scene->setRaytracingType(raytracingType::PHONG);
			}

			
			scene->setCamera(parseCamera(doc["Camera"]));

			auto shadowPresence = doc.FindValue("Shadows");
			if (shadowPresence != NULL) {
				scene->setShadowBool(parseType(*shadowPresence));
			}
			else {
				scene->setShadowBool(false);
			}

			auto recursionDepth = doc.FindValue("MaxRecursionDepth");
			if (recursionDepth != NULL) {
				scene->setMaxRecursion(parseReflectionDepth(*recursionDepth));
			}
			else {
				scene->setMaxRecursion(0);
			}
			
			auto superSampling = doc.FindValue("SuperSampling");
			if (superSampling != NULL) {
				scene->camera.superSampling = parseSuperSampling(*superSampling);
			}
			else {
				scene->camera.superSampling = 1;
			}

			auto GoochParameters = doc.FindValue("GoochParameters");
			if (GoochParameters != NULL) {
				scene->setGoochParams(parseGooch(*GoochParameters));
			}
			else {
				scene->setGoochParams({ 0,0,1,1 });
			}


            // Read and parse the scene objects
            const YAML::Node& sceneObjects = doc["Objects"];
            if (sceneObjects.GetType() != YAML::CT_SEQUENCE) {
                cerr << "Error: expected a sequence of objects." << endl;
                return false;
            }
            for(YAML::Iterator it=sceneObjects.begin();it!=sceneObjects.end();++it) {
                Object *obj = parseObject(*it);
                // Only add object if it is recognized
                if (obj) {
                    scene->addObject(obj);
                } else {
                    cerr << "Warning: found object of unknown type, ignored." << endl;
                }
            }

            // Read and parse light definitions
            const YAML::Node& sceneLights = doc["Lights"];
            if (sceneObjects.GetType() != YAML::CT_SEQUENCE) {
                cerr << "Error: expected a sequence of lights." << endl;
                return false;
            }
            for(YAML::Iterator it=sceneLights.begin();it!=sceneLights.end();++it) {
                scene->addLight(parseLight(*it));
            }
        }
        if (parser) {
            cerr << "Warning: unexpected YAML document, ignored." << endl;
        }
    } catch(YAML::ParserException& e) {
        std::cerr << "Error at line " << e.mark.line + 1 << ", col " << e.mark.column + 1 << ": " << e.msg << std::endl;
        return false;
    }

    cout << "YAML parsing results: " << scene->getNumObjects() << " objects read." << endl;
    return true;
}

void Raytracer::renderToFile(const std::string& outputFilename)
{
    Image img(scene->camera.aspectRatio * scene->camera.baseline, scene->camera.baseline);
    cout << "Tracing..." << endl;
    scene->render(img);
    cout << "Writing image to " << outputFilename << "..." << endl;
    img.write_png(outputFilename.c_str());
    cout << "Done." << endl;
}
