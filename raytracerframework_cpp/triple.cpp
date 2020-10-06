//
//  Framework for a raytracer
//  File: triple.cpp
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

#include "triple.h"

/**
 * Method redefining the standard output operator for triple
 * @param[in,out] s, the output stream where to stream the triple
 * @param[in] v, the triple to stream out
 * @return the updated output stream reference
 */
ostream& operator<<(ostream& s, const Triple& v) 
{
    return s << '[' << v.x << ',' << v.y << ',' << v.z << ']';
}
