// File: boundary.cpp
// Author: Liam Clink <clink.6@osu.edu>
//
// Implementation of the boundary.h header
//

#include "boundary.h"
#include <cfloat>
#include <algorithm> // for max(a,b)
#include <cmath>

/*
Calculate the distance from a point to a line specified
by its endpoints, using geometry. 
Consider a triangle made by the point and the endpoints of the
line. The numerator is twice the area of the triangle, and the
denominator is the length of the line.
*/
double dist_to_line_segment(const arma::vec& position,
                            const arma::vec& point1,
                            const arma::vec& point2)
{
    const double length_squared = arma::dot(point1-point2,point1-point2);
    if (length_squared == 0.0) return arma::norm(position-point1);

    // Consider the line extending the segment, parameterized as v + t (w - v).
    // We find projection of point p onto the line.
    // It falls where t = [(p-v) . (w-v)] / |w-v|^2
    // We clamp t from [0,1] to handle points outside the segment vw.
    const double dot_product = arma::dot(position-point1,point2-point1);
    const double t = std::max(0., std::min(1., dot_product/length_squared));
    const arma::vec projection = point1 + t * (point2 - point1);
    return arma::norm(position-projection);
}

//TODO: fix vertex intersection bug
int point_inside_polygon(const arma::vec& point, const arma::mat& vertices)
{
    // First do a coarse test using the bounding rectangle
    double xmin = arma::min(vertices.row(0));
    double xmax = arma::max(vertices.row(0));
    double ymin = arma::min(vertices.row(1));
    double ymax = arma::max(vertices.row(1));

    if (point(0)<xmin || point(0)>xmax || point(1)<ymin || point(1)>ymax)
    {
        return 0;
    }

    // At this point, the point is at least inside the bounding box. Start
    // raycasting by choosing a point outside the bounding box to start the ray
    double epsilon = 0.1*std::max(xmax-xmin,ymax-ymin);
    arma::vec start = {xmin-epsilon, ymin-epsilon};

    int intersections = 0;
    // Do a loop over the number of faces
    for (int i=0; i<vertices.n_cols; i++)
    {
        // Check intersection of ray with each edge
        // The modulo % is used because indices don't have wraparound in arma
        if (line_segment_intersect(start, point,
                vertices.col(i), vertices.col((i+1) % vertices.n_cols)) == 1)
        {
            /*
            // It is also possible that the ray intersects a vertex, which will
            // give two intersections, when only one would be expected. First
            // we check whether the vertex is close to being in between the
            // endpoints of the segment
            if ( point_between_points(vertices.col(i), start, point) )
            {
                // If the vertex is close to being in between the endpoints of
                // the segment, we need to consider whether an edge double
                // count should happen. This solution only works for 2D
                arma::vec vertex1 = vertices.col(
                        (i-1 + vertices.n_cols) % vertices.n_cols);
                arma::vec vertex2 = vertices.col(i);
                arma::vec vertex3 = vertices.col((i+1) % vertices.n_cols);
                double angle1 = atan2(vertex1(1)-vertex2(1),
                                      vertex1(0)-vertex2(0));
                double angle2 = atan2(point(1)-start(1),
                                      point(0)-start(0));
                double angle3 = atan2(vertex3(1)-vertex2(1),
                                      vertex3(0)-vertex2(0));
                if (std::signbit(angle2-angle1) == std::signbit(angle3-angle2))
                {
                    std::cout << "angle1: " << angle1/3.14*180 << " angle2: "
                        << angle2/3.14*180 << " angle3: " << angle3/3.14*180
                        << " x: " << point(0) << " y: " << point(1)
                        << " i: " << i << std::endl;
                    intersections++;
                }
            }*/
            intersections++;
        }
    }

    if ((intersections & 1) == 1)
        return 1; // Inside of polygon
    else
        return 0; // Outside of polygon
}

// Return 0 if no intersect, 1 if intersect, and -1 if collinear
int line_segment_intersect(const arma::vec& point1, const arma::vec& point2,
                           const arma::vec& point3, const arma::vec& point4)
{
    // Convert line segment 1 (point1 to point2) to a line of infinite length
    // in linear equation standard form: Ax + By + C = 0. The equivalent in
    // higher dimensions is calculating the intersection of a hyperplane with
    // the ray.
    double a1 = point2(1) - point1(1);
    double b1 = point1(0) - point2(0);
    double c = point2(0)*point1(1) - point1(0)*point2(1);

    // If a point is not on the line, then Ax + By + C != 0. If two points
    // give the same sign, then they are on the same side of the line, and
    // vice versa.
    // So now we test whether the second line segment endpoints are on
    // opposite sides of the extended first line, which tells us whether
    // there is an intersection.
    double d1 = a1*point3(0) + b1*point3(1) + c;
    double d2 = a1*point4(0) + b1*point4(1) + c;

    if ( (d1>0 && d2>0) || (d1<0 && d2<0) )
    {
        return 0;
    }

    // However, just because the extended first line and the second line
    // segment intersect, that doesn't necessarily mean that the segments
    // intersect. We also need to test the reverse case.
    double a2 = point4(1) - point3(1);
    double b2 = point3(0) - point4(0);
    c = point4(0)*point3(1) - point3(0)*point4(1);

    d1 = a2*point1(0) + b2*point1(1) + c;
    d2 = a2*point2(0) + b2*point2(1) + c;

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
