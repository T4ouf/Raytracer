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

#include "raytracer.h"
#include "object.h"
#include "sphere.h"
#include "triangle.h"
#include "model.h"
#include "Plane.h"
#include "quad.h"
#include "material.h"
#include "light.h"
#include "image.h"
#include "yaml/yaml.h"
#include "glm.h"
#include <ctype.h>
#include <fstream>
#include <assert.h>

// Functions to ease reading from YAML input
void operator >> (const YAML::Node& node, Triple& t);
Triple parseTriple(const YAML::Node& node);

/**
 * Function that reads a node with 3 data into a triple struct
 * @param[in] node, the YAML node to read
 * @param[out] t, the triple to fill
 * @return none
 */
void operator >> (const YAML::Node& node, Triple& t)
{
    assert(node.size()==3);
    node[0] >> t.x;
    node[1] >> t.y;
    node[2] >> t.z;
}

/**
 * Function that parses a node with 3 doubles, constructs and returns a triple struct
 * @param[in] node, the YAML node to read
 * @return t, the data (triple) from the YAML node
 */
Triple parseTriple(const YAML::Node& node)
{
    Triple t;
    node[0] >> t.x;
    node[1] >> t.y;
    node[2] >> t.z;	
    return t;
}

/**
 * Function that parses a node with 1 int (the rendering type)
 * @param[in] node, the YAML node to read
 * @return r, the raytracing type (enum value) from the YAML node
 */
raytracingType Raytracer::parseType(const YAML::Node& node){
	int r=0;
	if (node != NULL) {
		node >> r;
	}
	return (raytracingType)r;
}

/**
 * Function that parses a node with 1 int/bool (the shadow boolean)
 * @param[in] node, the YAML node to read
 * @return r, the bool stating if we need to render shadows or not
 */
bool Raytracer::parseShadow(const YAML::Node& node) {
	bool r = false;
	node >> r;
	return r;
}

/**
 * Function that parses a node with 1 int (the recursion depth of the "bounces" of the ray)
 * @param[in] node, the YAML node to read
 * @return r, the value stating how many times a ray "bounces"
 */
int Raytracer::parseReflectionDepth(const YAML::Node& node) {
	int r = 0;
	node >> r;
	return r;
}


/**
 * Function that parses a Material from the YAML file
 * @param[in] node, the YAML node to read
 * @return m, the material object (our Material class, cf. Material.h)
 */
Material* Raytracer::parseMaterial(const YAML::Node& node){

    Material *m = new Material(); //NOTE : dynamic allocation (we don't statically know how many materials are going to be used)

	//check for textures
	auto t = node.FindValue("texture");
	if (t == NULL) {
		m->texture = NULL; //no texture
	}
	else {

		//parsing the texture file
		std::string path ;
		node["texture"] >> path;
		m->texture = new Image(path.c_str());
	}

	//check for cel shading texture
	auto celTexture = node.FindValue("cel");
	if (celTexture == NULL) {
		char* path = "defaultCel.png";
		m->celTexture = new Image(path);
	}
	else {
		std::string path;
		node["cel"] >> path;
		m->celTexture = new Image(path.c_str());
	}

	//check for bump map
	auto bump = node.FindValue("bumpMap");
	if (bump == NULL) {
		m->bumpMap = NULL;
	}
	else {
		std::string pathBump;
		node["bumpMap"] >> pathBump;
		m->bumpMap = new Image(pathBump.c_str());
	}

	//check for ambiant occlusion map
	auto ambientOcclusion = node.FindValue("ambient");
	if (ambientOcclusion == NULL) {
		m->ambOccMap = NULL;
	}
	else {
		std::string pathAmbOcc;
		node["ambient"] >> pathAmbOcc;
		m->ambOccMap = new Image(pathAmbOcc.c_str());
	}

	//parse material's parameters
    node["color"] >> m->color;	
    node["ka"] >> m->ka;
    node["kd"] >> m->kd;
    node["ks"] >> m->ks;
    node["n"] >> m->n;

	//return the material
    return m;
}

/**
 * Function that parses an Object from the YAML file
 * @param[in] node, the YAML node to read
 * @return returnObject, the new object from what we parsed (Warning : return value is a pointer !)
 */
Object* Raytracer::parseObject(const YAML::Node& node)
{
    Object *returnObject = NULL;
    std::string objectType;
    node["type"] >> objectType; //We parse the type of object (what kind of object is it ?)

	// Parse a Sphere (3D object)
	if (objectType == "sphere") {
        Point pos;
        node["position"] >> pos;	//parse its position
        double r;
        node["radius"] >> r;		//parse its radius

		//Small trick to manage rotation for spheres 
		auto upValue = node.FindValue("rotation-axis");
		Vector up = Vector(0, 1, 0);
		if (upValue != NULL) {
			up = parseTriple(*upValue).normalized();
		}
				
		double rot = 0.0f;
		auto angleValue = node.FindValue("angle");
		if (angleValue != NULL) {
			*angleValue >> rot;
		}
		
		//construct and return the sphere
        Sphere *sphere = new Sphere(pos,r,rot,up);	
        returnObject = sphere;
    }
	// Parse a Triangle (2D object)
	else if (objectType == "triangle") {
		//parse all points of the triangle
		Point p1;
		Point p2;
		Point p3;
		node["p1"] >> p1;
		node["p2"] >> p2;
		node["p3"] >> p3;
	
		//construct and return the triangle
		Triangle* triangle = new Triangle(p1,p2,p3);
		returnObject = triangle;
	}
	// Parse a Quad (2D object)
	else if (objectType == "quad") {
		//parse all points of the quad
		Point p1;
		Point p2;
		Point p3;
		Point p4;
		node["p1"] >> p1;
		node["p2"] >> p2;
		node["p3"] >> p3;
		node["p4"] >> p4;

		//construct and return the triangle
		Quad* quad = new Quad(p1, p2, p3, p4);
		returnObject = quad;
	}
	// Parse a Plane (2D infinite object)
	else if (objectType == "plane") {
		//parse all the points defining the plane in 3D space
		Point p1;
		Point p2;
		Point p3;
		node["p1"] >> p1;
		node["p2"] >> p2;
		node["p3"] >> p3;

		//Small trick to manage rotation for planes (textures)
		auto upValue = node.FindValue("rotation-axis");
		Vector up = Vector(0, 1, 0);
		if (upValue != NULL) {
			up = parseTriple(*upValue).normalized();
		}
		
		double rot = 0;
		auto angleValue = node.FindValue("angle");
		if (angleValue != NULL) {
			*angleValue >> rot;
		}

		//construct and return the plane
		Plane* plane = new Plane(p1, p2, p3, up, rot);
		returnObject = plane;
	}
	// Parse a 3D Model (3D object from file)
	else if(objectType == "model"){

		//Load the model from .obj files using glm
		std::string path;
		node["path"] >> path;
		char* path2 = &path[0];
		auto model = glmReadOBJ(path2);

		double scaleFactor;
		node["scaleFactor"] >> scaleFactor;

		//More visual => x10 
		scaleFactor = scaleFactor * 10;

		//We properly scale our model
		glmUnitize(model);
		glmScale(model, float(scaleFactor));

		Point position = parseTriple(node["position"]);
		
		//Array to store all the vertices
		double* dblArray = new double[model->numtriangles * 9];

		//We get the dimensions of our model
		float dim[3];
		glmDimensions(model, dim);


		double r = 0.0; //radius of the bounding sphere (minimize computation for ray's hit detection with an easier "global" hitbox)
		if (dim[0] >= dim[1] && dim[0] >= dim[2]) {
			r = dim[0] * scaleFactor;
		}
		else if (dim[1] >= dim[0] && dim[1] >= dim[2]) {
			r = dim[1] * scaleFactor;
		}
		else if (dim[2] >= dim[1] && dim[2] >= dim[0]) {
			r = dim[2] * scaleFactor;
		}

		//we create a blank model object...
		Model* m = new Model(position, r, 0, Vector(0, 1, 0));

		//we parse all the triangles
		for (size_t i = 0; i < model->numtriangles; i++) {

			for (size_t j = 0; j < 3; j++){
				//cout << model->triangles[i].vindices[j] << '\t';

				for (size_t k = 0; k < 3; k++){
					
					dblArray[9 * i + 3 * j + k] = model->vertices[3*model->triangles[i].vindices[j]+k];
				}
				
			}
			//cout << '\n';
			
			Point p1 = Point(dblArray[9 * i]	, dblArray[9 * i + 1], dblArray[9 * i + 2]) + position;
			Point p2 = Point(dblArray[9 * i + 3], dblArray[9 * i + 4], dblArray[9 * i + 5]) + position;
			Point p3 = Point(dblArray[9 * i + 6], dblArray[9 * i + 7], dblArray[9 * i + 8]) + position;

			Triangle t = Triangle(p1, p2, p3);
			m->meshes.push_back(t);
		}
		
		/* Debug print
		cout << "\n-------------------------------------------------------\n";

		for (int i2 = 0; i2 < model->numtriangles * 9; i2++) {
			cout << dblArray[i2] << '\t';

			if ((i2+1) % 3 == 0) {
				cout << '\n';
			}
			
		}
		cout << '\n';*/

		//return the 3D model
		returnObject = m;

		//clean memory
		glmDelete(model);
		delete[] dblArray;

	}

	//If we found an object to parse...
    if (returnObject != NULL) {
        //... we read the material and attach to object
        returnObject->material = parseMaterial(node["material"]);
    }

    return returnObject;
}

/**
 * Function that parses a light from the YAML file
 * @param[in] node, the YAML node to read
 * @return the new light object from what we parsed (Warning : return value is a pointer !)
 */
Light* Raytracer::parseLight(const YAML::Node& node)
{
    Point position;
    node["position"] >> position;
    Color color;
    node["color"] >> color;
    return new Light(position,color);
}

/**
 * Function that parses the super sampling factor (SSAA) from the YAML file
 * @param[in] node, the YAML node to read
 * @return r, the super sampling factor (how many subdivisions each pixel has... must be a power of 2 preferabily)
 */
int Raytracer::parseSuperSampling(const YAML::Node& node) {
	int r = 0;
	node["factor"] >> r;
	return r;
}

/**
 * Function that parses the Gooch parameters from the YAML file
 * @param[in] node, the YAML node to read
 * @return g, the new gooch parameters object
 */
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

/**
 * Function that parses the Gooch parameters from the YAML file
 * @param[in] node, the YAML node to read
 * @return c, the new camera object (Warning : return value is a pointer !)
 */
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
* Method that reads a full scene from a file
* @param[in] inputFilename, the path to the file
* @return a boolean stating if the method succeeded or not
*/
bool Raytracer::readScene(const std::string& inputFilename)
{
    // Initialize a new scene
    scene = new Scene();

    // Open file stream for reading and have the YAML module parse it
    std::ifstream fin(inputFilename.c_str());
    if (!fin) {
        std::cerr << "Error: unable to open " << inputFilename << " for reading." << std::endl;;
        return false;
    }
    try {
        YAML::Parser parser(fin);

		//if the parser was correctly created...
        if (parser) {

			//we check the file for the first node
            YAML::Node doc;
            parser.GetNextDocument(doc);
			
			//check for the raytracing type
			auto renderType = doc.FindValue("RaytracingType");
			if (renderType != NULL) {
				scene->setRaytracingType(parseType(*renderType));
			}
			else {
				scene->setRaytracingType(raytracingType::PHONG); //default rendering mode
			}

			//check for the camera param
			scene->setCamera(parseCamera(doc["Camera"]));

			//check for the shadow boolean
			auto shadowPresence = doc.FindValue("Shadows");
			if (shadowPresence != NULL) {
				scene->setShadowBool(parseType(*shadowPresence));
			}
			else {
				scene->setShadowBool(false);
			}

			//check for the recursion depth
			auto recursionDepth = doc.FindValue("MaxRecursionDepth");
			if (recursionDepth != NULL) {
				scene->setMaxRecursion(parseReflectionDepth(*recursionDepth));
			}
			else {
				scene->setMaxRecursion(0);
			}
			
			//check for the super sampling factor
			auto superSampling = doc.FindValue("SuperSampling");
			if (superSampling != NULL) {
				scene->camera.superSampling = parseSuperSampling(*superSampling);
			}
			else {
				scene->camera.superSampling = 1;
			}

			//check for the Gooch parameters
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
				std::cerr << "Error: expected a sequence of objects." << std::endl;
                return false;
            }
            for(YAML::Iterator it=sceneObjects.begin();it!=sceneObjects.end();++it) {
                Object *obj = parseObject(*it);

                // Only add object to the scene if it is recognized
                if (obj) {
                    scene->addObject(obj);
                } else {
					std::cerr << "Warning: found object of unknown type, ignored." << std::endl;
                }
            }

            // Read and parse light definitions
            const YAML::Node& sceneLights = doc["Lights"];
            if (sceneObjects.GetType() != YAML::CT_SEQUENCE) {
				std::cerr << "Error: expected a sequence of lights." << std::endl;
                return false;
            }
            for(YAML::Iterator it=sceneLights.begin();it!=sceneLights.end();++it) {
                scene->addLight(parseLight(*it));
            }
        }
        if (parser) {
			std::cerr << "Warning: unexpected YAML document, ignored." << std::endl;
        }
    } catch(YAML::ParserException& e) {
        std::cerr << "Error at line " << e.mark.line + 1 << ", col " << e.mark.column + 1 << ": " << e.msg << std::endl;
        return false;
    }

	//print result for information
	std::cout << "YAML parsing results: " << scene->getNumObjects() << " objects read." << std::endl;
    return true;
}

/*
* Method that renders the scene in an image file (via the raytracing process)
* @param[in] outputFilename, the path/name to the output file
* @return none
*/
void Raytracer::renderToFile(const std::string& outputFilename)
{
    Image img(scene->camera.aspectRatio * scene->camera.baseline, scene->camera.baseline); //create an image
    cout << "Tracing..." << endl;
    scene->render(img);
    cout << "Writing image to " << outputFilename << "..." << endl;
    img.write_png(outputFilename.c_str()); //render our image to a file
    cout << "Done." << endl;
}
