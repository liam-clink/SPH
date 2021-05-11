// File: boundary.h
// Author: Liam Clink <clink.6@osu.edu
//
// This file defines functions that are useful for enforcing
// boundary conditions
//

#pragma once

#include <armadillo>
#include <cmath>

// Calculate the distance from a point to a line specified
// by its endpoints, using geometry. 
double dist_to_line_segment(const arma::vec& position,
                            const arma::vec& point1,
                            const arma::vec& point2);

// Use raycasting to determine whether a point is inside a polygon
int point_inside_polygon(const arma::vec& point, const arma::mat& vertices);

// Determine if two line segments, specified by their endpoints, intersect.
// line1 and line2 are 4 element vectors formatted as (x1,y1,x2,y2)
int line_segment_intersect(const arma::vec& point1, const arma::vec& point2,
                           const arma::vec& point3, const arma::vec& point4);
