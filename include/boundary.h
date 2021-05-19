// File: boundary.h
// Author: Liam Clink <clink.6@osu.edu
//
// This file defines functions that are useful for enforcing
// boundary conditions
//

#pragma once

#include <armadillo>

struct Line_Segment
{
    Line_Segment() = default;
    Line_Segment(arma::vec _start, arma::vec _end);

    arma::vec start;
    arma::vec end;
};

struct Polygon
{
    Polygon() = default;
    Polygon(std::vector<arma::vec> _vertices);

    std::vector<arma::vec> vertices;
};

// Calculate the distance from a point to a line specified
// by its endpoints, using geometry. 
double dist_to_line_segment(const arma::vec& point,
                            const Line_Segment& segment);

// Use raycasting to determine whether a point is inside a polygon
bool point_inside_polygon(const arma::vec& point, const Polygon& polygon);

// Determine if two line segments, specified by their endpoints, intersect.
// line1 and line2 are 4 element vectors formatted as (x1,y1,x2,y2)
int line_segment_intersect(const Line_Segment& segment1,
                            const Line_Segment& segment2);
