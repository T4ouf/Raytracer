//
//  Framework for a raytracer
//  File: material.h
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

#ifndef MATERIAL_H_TWMNT2EJ
#define MATERIAL_H_TWMNT2EJ

#include <iostream>
#include "triple.h"
#include "image.h"

/**
 * Material struct :
 * A material in our raytracer is just a group of data defining the material properties to apply
 */
struct Material{

    Color color;        // base color
	Image* texture;		// texture Image
    Image* celTexture;  // cel shading texture
	Image* bumpMap;		// Object Bump map
    Image* ambOccMap;   // Object ambient occlusion map
    double ka;          // ambient intensity
    double kd;          // diffuse intensity
    double ks;          // specular intensity 
    double n;           // exponent for specular highlight size

    Material() { }
};

#endif /* end of include guard: MATERIAL_H_TWMNT2EJ */
