#pragma once
#include "triple.h"
#define _USE_MATH_DEFINES
#include <math.h>


struct Transformations {

	static Triple& rotation(const Triple& p, const Vector& rotationAxis, double radiansAngle) {

		Vector u = rotationAxis.normalized();
		double cosA = cos(radiansAngle);
		double sinA = sin(radiansAngle);

		Vector row1 = Vector(
			cosA + u.x * u.x * (1 - cosA),
			u.x * u.y * (1 - cosA) - u.z * sinA,
			u.x * u.z * (1 - cosA) + u.y * sinA
		);

		Vector row2 = Vector(
			u.y * u.x * (1 - cosA) + u.z * sinA,
			cosA + u.y * u.y * (1 - cosA),
			u.y * u.z * (1 - cosA) - u.x * sinA
		);

		Vector row3 = Vector(
			u.z * u.x * (1 - cosA) - u.y * sinA,
			u.z * u.y * (1 - cosA) + u.x * sinA,
			cosA + u.z * u.z * (1 - cosA)
		);

		return Vector(row1.dot(p), row2.dot(p), row3.dot(p));

	}
	static Triple& rotationDeg(const Triple& p, const Vector& v, double angle) {
		return rotation(p, v, angle * (M_PI) / 180);
	}

};

