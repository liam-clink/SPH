// File: boundary.h
// Author: Liam Clink <clink.6@osu.edu
//
// This file defines functions that are useful for enforcing
// boundary conditions
//

#include <armadillo>
#include <cmath>

// Calculate the distance from a position to a line specified
// by its endpoints, using geometry. Imagine a triangle made by the position
// and the endpoints of the line. The numerator is twice the area
// of the triangle, and the denominator is the length of the line.
double dist_to_line(const arma::vec& position,
                    const arma::vec& point1,
                    const arma::vec& point2)
{
    double numerator = fabs( (point2(1)-point1(1))*position(0)
                             - (point2(0)-point1(0))*position(1)
                             + point2(0)*point1(1) - point2(1)*point1(0) );

    double denominator = sqrt( (point2(1)-point1(1))*(point2(1)-point1(1))
                             + (point2(0)-point1(0))*(point2(0)-point1(0)) );

    return numerator / denominator;
}

