//
//  Framework for a raytracer
//  File: raytracer.h
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

#ifndef RAYTRACER_H_6GQO67WK
#define RAYTRACER_H_6GQO67WK

#include <iostream>
#include <string>
#include "triple.h"
#include "light.h"
#include "scene.h"
#include "yaml.h"

/**
 * Raytracer class :
 * Class that manages all the raytracing process and output an image
 *
 * @NOTE : All the complex data read from file are dynamically allocated on the heap and never free. It is a problem that will be fixed....
 * However it is not a big deal as the lifetime of all objects, materials, ect. is the same as the program (the program read one YAML file and generates one image)
 * (no real memory leak)
 */
class Raytracer {
private:
    Scene *scene; //Scene built out of the YAML scene file

    // Couple of private functions for parsing YAML nodes
	raytracingType parseType(const YAML::Node& node);
    Material* parseMaterial(const YAML::Node& node);
    Object* parseObject(const YAML::Node& node);
    Light* parseLight(const YAML::Node& node);
	bool parseShadow(const YAML::Node& node);
	int parseReflectionDepth(const YAML::Node& node);
	Camera* parseCamera(const YAML::Node& node);
	int parseSuperSampling(const YAML::Node& node);
	GoochParameters parseGooch(const YAML::Node& node);


public:
    Raytracer() { }

    bool readScene(const std::string& inputFilename);       //Method to read to construct the scene from a YAML file
    void renderToFile(const std::string& outputFilename);   //Method to render the scene in an image file (via the raytracing process)
};

#endif /* end of include guard: RAYTRACER_H_6GQO67WK */
