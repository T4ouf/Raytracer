//
//  Framework for a raytracer
//  File: light.h
//
//  Created for the Computer Science course "Introduction Computer Graphics"
//  taught at the University of Groningen by Tobias Isenberg.
//
//  Authors:
//    Maarten Everts
//    Jasper van de Gronde
//    adjustment of Hit class by Olivier Wavrin
//
//  This framework is inspired by and uses code of the raytracer framework of 
//  Bert Freudenberg that can be found at
//  http://isgwww.cs.uni-magdeburg.de/graphik/lehre/cg2/projekt/rtprojekt.html 
//

#ifndef LIGHT_H_PG2BAJRA
#define LIGHT_H_PG2BAJRA

#include <iostream>
#include <limits>
#include "triple.h"

/**
 * Light struct :
 * Representation of a basic light source (point source) in our raytracer
 */
struct Light {

    /**
     * Standard constructor for a light source
     * @param[in] pos, position of the light source
     * @param[in] c, color of the emitted light
     * @return none
     */
    Light(Point pos,Color c) : position(pos), color(c) { }

    Point position;
    Color color;
};

/**
 * Ray struct :
 * Representation of a ray that the raytracer will cast
 * The raytracing process is based on shooting rays through each pixel of an image 
 * and to then compute local illumination based on the collisions that the ray had
 */
struct Ray {
    Point O;    //Ray's origin
    Vector D;   //Ray's direction
    
    /**
     * Parametrized constructor for a ray
     * @param[in] from, the origin point of the ray
     * @param[in] dir, the direction vector of the ray
     * @return none
     */
    Ray(const Point &from, const Vector &dir) : O(from), D(dir) { }

    /**
     * Method to get the position of the ray at a certain distance
     * @param[in] t, the distance from the origin of the ray
     * @return the point on the ray "line" at this distance
     */
    Point at(double t) const { return O + t*D; }

};

/**
 * hit struct :
 * Representation of the hit point of a ray
 */
struct Hit {
    double t;   //distance from the origin of the ray
    Vector N;   //normal to the hit point surface
    bool no_hit;//is there a hit point ? (is the ray hitting an object in the scene ?)
    
    /**
     * Parametrized constructor for a Hit
     * @param[in] t, distance from the origin of the ray
     * @param[in] normal, the normal vector to the hit point surface
     * @param[in] nohit (optional), is there a hit point ?
     * @return none
     */
    Hit(const double t, const Vector &normal, bool nohit = false) : t(t), N(normal), no_hit(nohit) { }

    /**
     * Method defininig a hit point for no_hit value (for comparison sake)
     */
    static const Hit NO_HIT() { static Hit no_hit(std::numeric_limits<double>::quiet_NaN(),Vector(std::numeric_limits<double>::quiet_NaN(),std::numeric_limits<double>::quiet_NaN(),std::numeric_limits<double>::quiet_NaN()), true); return no_hit; }

};

#endif /* end of include guard: LIGHT_H_PG2BAJRA */
