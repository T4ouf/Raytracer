//
//  Framework for a raytracer
//  File: triple.h
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

#ifndef TRIPLE_H_SEVQHPTA
#define TRIPLE_H_SEVQHPTA

#include <math.h>
#include <iostream>
using namespace std;

/**
 * Triple class :
 * Representation of a group of 3 doubles (without semantic)
 * this class is providing a large toolkit for managing data with 3 doubles (operators, function, etc.)
 * this class is then renamed regarding what the triple is (a Vector (3D), a Point (in a 3D space), a Color) and thus provide semantic
 */
class Triple {
public:

    //basic default constructor
    explicit Triple(double X = 0, double Y = 0, double Z = 0) : x(X), y(Y), z(Z) {}

    //redefining addition for triple+triple
    Triple operator+(const Triple &t) const
    {
        return Triple(x+t.x, y+t.y, z+t.z);
    }

    //redefining addition for triple+double
    Triple operator+(double f) const
    {
        return Triple(x+f, y+f, z+f);
    }

    //redefining addition for double+triple
    friend Triple operator+(double f, const Triple &t)
    {
        return Triple(f+t.x, f+t.y, f+t.z);
    }

    //redefining negation operator
    Triple operator-() const
    {
        return Triple( -x, -y, -z);
    }

    //redefining substraction for triple-triple
    Triple operator-(const Triple &t) const
    {
        return Triple(x-t.x, y-t.y, z-t.z);
    }

    //redefining substraction for triple-double
    Triple operator-(double f) const
    {
        return Triple(x-f, y-f, z-f);
    }

    //redefining substraction for double-triple
    friend Triple operator-(double f, const Triple &t)
    {
        return Triple(f-t.x, f-t.y, f-t.z);
    }

    //redefining multiplication for triple*triple
    Triple operator*(const Triple &t) const
    {
        return Triple(x*t.x,y*t.y,z*t.z);
    }

    //redefining multiplication for triple*double
    Triple operator*(double f) const
    {
        return Triple(x*f, y*f, z*f);
    }

    //redefining multiplication for double*triple
    friend Triple operator*(double f, const Triple &t)
    {
        return Triple(f*t.x, f*t.y, f*t.z);
    }

    //redefining division by a double (scaling)
    Triple operator/(double f) const
    {
        double invf = 1.0/f;
        return Triple(x*invf, y*invf, z*invf);
    }

    //redefining increment operator with a triple
    Triple& operator+=(const Triple &t)
    {
        x += t.x;
        y += t.y;
        z += t.z;
        return *this;
    }

    //redefining increment operator with a double
    Triple& operator+=(double f)
    {
        x += f;
        y += f;
        z += f;
        return *this;
    }

    //redefining decrement operator with a triple
    Triple& operator-=(const Triple &t)
    {
        x -= t.x;
        y -= t.y;
        z -= t.z;
        return *this;
    }

    //redefining decrement operator with a double
    Triple& operator-=(double f)
    {
        x -= f;
        y -= f;
        z -= f;
        return *this;
    }

    //redefining multiplication increment operator with a double
    Triple& operator*=(const double f)
    {
        x *= f;
        y *= f;
        z *= f;
        return *this;
    }

    //redefining division decrement operator with a double
    Triple& operator/=(const double f)
    {
        double invf = 1.0/f; //small trick : doing 1 division instead of 3
        x *= invf;
        y *= invf;
        z *= invf;
        return *this;
    }

    /** 
     * Method defining the dot product of two triple (2 vectors for instance)
     * @param[in] t, the triple with which we do the dot product
     * @return the dot product of the two triples
     */
    double dot(const Triple &t) const
    {
        return x*t.x + y*t.y + z*t.z;
    }

    /**
    * Method defining the cross product of two triple (2 vectors for instance)
    * @param[in] t, the triple with which we do the cross product
    * @return the cross product of the two triple (meaning the "vector" (triple) perpendicular to the two "vectors" (triple)) 
    */
    Triple cross(const Triple &t) const
    {
        return Triple( y*t.z - z*t.y,
            z*t.x - x*t.z,
            x*t.y - y*t.x);
    }

    // Method that return the length of a vector
    double length() const
    {
        return sqrt(length_2());
    }

    // Method that return the square length of a vector
    double length_2() const
    {
        return x*x + y*y + z*z;
    }

    // Method that return the direction of a vector (the normalized version of it)
    Triple normalized() const
    {
        return (*this) / length();
    }

    // Method that normalizes a vector (Sum of its component = 1)
    void normalize()
    {
        double l = length();
        double invl = 1/l;
        x *= invl;
        y *= invl;
        z *= invl;
    }	

    //defining I/O operators
    friend istream& operator>>(istream &s, Triple &v);
    friend ostream& operator<<(ostream &s, const Triple &v);

    //////////////////////////////////////
    // Functions for when used as a Color:

    // set a color (global)
    void set(double f)
    {
        r = g = b = f;
    }

    // set a color (with a threshold)
    void set(double f, double maxValue)
    {
        set(f/maxValue);
    }

    // set a color (per channel)
    void set(double red, double green, double blue)
    {
        r = red;
        g = green;
        b = blue;
    }

    // set a color (per channel + with a threshold)
    void set(double r, double g, double b, double maxValue)
    {
        set(r/maxValue,g/maxValue,b/maxValue);
    }

    // clamp values of a color to a max value
    void clamp(double maxValue = 1.0)
    {
        if (r > maxValue) r = maxValue;
        if (g > maxValue) g = maxValue;
        if (b > maxValue) b = maxValue;
    }

    //Making two representation for this 3 double (x,y,z)/(r,g,b)
    union {
        double data[3];
        struct {
            double x;
            double y;
            double z;
        };
        struct {
            double r;
            double g;
            double b;
        };
    };
};

//Making aliases for the triple class
typedef Triple Color;
typedef Triple Point;
typedef Triple Vector;

#endif /* end of include guard: TRIPLE_H_SEVQHPTA */
