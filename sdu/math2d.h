#pragma once

#ifndef MATH2D_H
#define MATH2D_H

#include <cmath>

namespace Crown
{

inline double DistanceSquared(double x1, double y1, double x2, double y2)
{
	double dx = x2 - x1;
	double dy = y2 - y1;
	return dx * dx + dy * dy;
}

/*
	Return the distance of the given point in 2D.
*/
inline double Distance(double x1, double y1, double x2, double y2)
{
	double dx = x2 - x1;
	double dy = y2 - y1;
	return sqrt(dx * dx + dy * dy);
}

/*
 * Normalize angle to [-pi, pi).
 */
inline double NormalizeAngle(double angle, double centre = 0)
{
    // The result will be [centre - pi, centre + pi).
	const double TWO_PI = M_PI * 2;
    return angle - TWO_PI * floor((angle + M_PI - centre) / TWO_PI);
}

/*
	Return the angle of the given vector in 2D.
	The value of angle will be in [-pi, pi).
*/
inline double Angle(double startX, double startY, double endX, double endY)
{
	double dx = endX - startX;
	double dy = endY - startY;
    return atan2(dy, dx);
}

} // namespace

#endif // MATH2D_H
