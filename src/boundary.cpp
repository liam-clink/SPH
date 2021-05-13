// File: boundary.cpp
// Author: Liam Clink <clink.6@osu.edu>
//
// Implementation of the boundary.h header
//

#include "boundary.h"
#include <cfloat>
#include <algorithm> // for max(a,b) and max_element()
#include <cmath>

// Construct polygon from sequence of vertices
Polygon::Polygon(std::vector<arma::vec> _vertices)
{
    vertices = _vertices;
}

/*
Calculate the distance from a point to a line specified
by its endpoints, using geometry. 
Consider a triangle made by the point and the endpoints of the
line. The numerator is twice the area of the triangle, and the
denominator is the length of the line.
*/
double dist_to_line_segment(const arma::vec& point, const Line_Segment& segment)
{
    const double length_squared =
        arma::dot(segment.end - segment.start, segment.end - segment.start);
    if (length_squared == 0.0)
        return arma::norm(point-segment.start);

    // Consider the line extending the segment, parameterized as v + t (w - v).
    // We find projection of point p onto the line.
    // It falls where t = [(p-v) . (w-v)] / |w-v|^2
    // We clamp t from [0,1] to handle points outside the segment vw.
    const double dot_product =
        arma::dot(point - segment.start, segment.end - segment.start);
    const double t = std::max(0., std::min(1., dot_product/length_squared));
    const arma::vec projection =
        segment.start + t * (segment.end - segment.start);
    
    return arma::norm(point - projection);
}

//TODO: fix vertex intersection bug
bool point_inside_polygon(const arma::vec& point, const Polygon& polygon)
{
    // First do a coarse test using the bounding rectangle
    double xmin = polygon.vertices[0](0);
    double xmax = xmin;
    double ymin = polygon.vertices[0](1);
    double ymax = ymin;
    
    for (auto vertex : polygon.vertices)
    {
        if (vertex(0) < xmin)
            xmin = vertex(0);
        else if (vertex(0) > xmax)
            xmax = vertex(0);
        if (vertex(1) < ymin)
            ymin = vertex(1);
        else if (vertex(1) > ymax)
            ymax = vertex(1);
    }

    if (point(0)<xmin || point(0)>xmax || point(1)<ymin || point(1)>ymax)
    {
        return false;
    }

    // At this stage, the point is at least inside the bounding box. Start
    // raycasting by choosing a point outside the bounding box to start the ray.
    // It is simpler to check vertex intersection if ray is horizontal or
    // vertical.
    Line_Segment ray;

    int intersections = 0;
    // Do a loop over the number of edges
    for (int i=0; i<polygon.vertices.size(); i++)
    {
        // Choose ray to be horizontal to the left
        ray.start = {xmin, polygon.vertices[i](1)}; // May need to shift xmin
        ray.end = point;

        // Check intersection of ray with each edge
        // The modulo % is used because indices don't have wraparound in arma
        if (line_segment_intersect(
            ray,
            Line_Segment(polygon.vertices[i],
                         polygon.vertices[(i+1) % polygon.vertices.size()])))
        {
            // It is also possible that the ray intersects a vertex, which will
            // give two intersections, when only one would be expected sometimes
            intersections++;
        }
    }

    // Check if number of intersections is odd
    if ((intersections & 1) == 1)
        return true; // Inside of polygon
    else
        return false; // Outside of polygon
}

// Return 0 if no intersect, 1 if intersect, and -1 if collinear
bool line_segment_intersect(const Line_Segment& segment1,
                            const Line_Segment& segment2)
{
    // Convert line segment 1 (endpoint1 to endpoint2) to a line of infinite
    // length in linear equation standard form: Ax + By + C = 0. The equivalent
    // in higher dimensions is calculating the intersection of a hyperplane with
    // the ray.
    double a1 = segment1.end(1) - segment1.start(1);
    double b1 = segment1.start(0) - segment1.end(0);
    double c = segment1.end(0)*segment1.start(1) 
               - segment1.start(0)*segment1.end(1);

    // If a point is not on the line, then Ax + By + C != 0. If two points
    // give the same sign, then they are on the same side of the line, and
    // vice versa.
    // So now we test whether the second line segment endpoints are on
    // opposite sides of the extended first line, which tells us whether
    // there is an intersection.
    double d1 = a1*segment2.start(0) + b1*segment2.start(1) + c;
    double d2 = a1*segment2.end(0) + b1*segment2.end(1) + c;

    if ( (d1>0 && d2>0) || (d1<0 && d2<0) )
    {
        return 0;
    }

    // However, just because the extended first line and the second line
    // segment intersect, that doesn't necessarily mean that the segments
    // intersect. We also need to test the reverse case.
    double a2 = segment2.end(1) - segment2.start(1);
    double b2 = segment2.start(0) - segment2.end(0);
    c = segment2.end(0)*segment2.start(1) - segment2.start(0)*segment2.end(1);

    d1 = a2*segment1.start(0) + b2*segment1.start(1) + c;
    d2 = a2*segment2.end(0) + b2*segment1.end(1) + c;

    if ( (d1>0 && d2>0) || (d1<0 && d2<0) )
    {
        return 0;
    }

    // At this point, the line segments either intersect at one point or
    // are collinear, so we also need to test for this possibility.
    if (a1*b2 - a2*b1 == 0.)
    {
        return -1;
    }

    // The only remaining possibility is intersection of the lines at one point
    return 1;
}
