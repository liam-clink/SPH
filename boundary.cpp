// File: boundary.cpp
// Author: Liam Clink <clink.6@osu.edu>
//
// Implementation of the boundary.h header
//

#include "boundary.h"
#include <cfloat>
#include <algorithm> // for max(a,b)

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

int point_between_points(const arma::vec& point1,
                         const arma::vec& point2,
                         const arma::vec& point3)
{
    double length = arma::norm(point3-point2);
    
    // Check if the lines corresponding to the vectors are close to 
    // parallel using their cross product
    
    arma::vec p3d1, p3d2, p3d3;

    // Convert 2D to 3D
    if (point1.n_elem == 2)
    {
        p3d1 = {point1(0), point1(1), 0.};
        p3d2 = {point2(0), point2(1), 0.};
        p3d3 = {point3(0), point3(1), 0.};
    }
    else
    {
        p3d1 = point1;
        p3d2 = point2;
        p3d3 = point3;
    }

    double cross_product = arma::norm(arma::cross(p3d1-p3d2, p3d3-p3d2));

    if ( cross_product > 1e-6*length )
        return 0;

    // Check that the vector corresponding to the point is in the right
    // direction and has the right length for the point to be close to
    // the line
    double dot_product = arma::dot(point3-point2, point1-point2);
    if (dot_product < 0 || dot_product > length*length)
        return 0;

    // The only remaining possibility is that the first point is very
    // close to being in between the other two points
    return 1;
}

int point_inside_polygon(const arma::vec& point, const arma::mat& vertices)
{
    // First do a coarse test using the bounding rectangle
    double xmin = arma::min(vertices.row(0));
    double xmax = arma::max(vertices.row(0));
    double ymin = arma::min(vertices.row(1));
    double ymax = arma::max(vertices.row(1));

    if (point(0)<xmin || point(0)>xmax || point(1)<ymin || point(1)>ymax)
    {
        std::cout << "Outside of box, xmin: " << xmin << " xmax: "
            << xmax << " ymin: " << ymin << " ymax: " << ymax << std::endl;
        point.print();
        return 0;
    }

    // At this point, the point is at least inside the bounding box. Start
    // raycasting by choosing a point outside the bounding box to start the ray
    double epsilon = 0.1*std::max(xmax-xmin,ymax-ymin);
    arma::vec start = {xmin-epsilon, ymin-epsilon};

    int intersections = 0;
    std::cout << "Loop over faces\n";
    // Do a loop over the number of faces
    for (int i=0; i<vertices.n_cols-1; i++)
    {
        // Check intersection of ray with each edge
        std::cout << "Loop #" << i << std::endl;
        if (line_segment_intersect(start, point,
                vertices.col(i), vertices.col(i+1)) == 1)
            intersections++;
    }

    if (line_segment_intersect(start, point,
                vertices.col(0), vertices.col(vertices.n_cols-1)) == 1)
        intersections++;
    
    // It is also possible that the ray intersects a vertex, which will give
    // two intersections, when only one would be expected. We also need to make
    // sure to only count the vertex intersections in between the endpoints of
    // the line segment.
    // Check if any vertex is close to the line, and undo the double count
    for (int i=0; i<vertices.n_cols; i++)
    {
        if ( point_between_points(vertices.col(i), start, point) )
            intersections--;
        // TODO: But what if the line actually goes through two faces, and
        // just gets near a vertex?
    }

    if ((intersections & 1) == 1)
        return 1; // Inside of polygon
    else
        return 0; // Outside of polygon
}

// Return 0 if no intersect, 1 if intersect, 2 if intersect vertex, and -1 if collinear
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
        std::cout << "First case\n";
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
        std::cout << "Second case\n";
        return 0;
    }

    if ( fabs(d1) < 2*DBL_MIN || fabs(d2) < 2*DBL_MIN )
    {
        std::cout << "Vertex intersection 2\n";
        return 2;
    }
    // At this point, the line segments either intersect at one point or
    // are collinear, so we also need to test for this possibility.
    if (a1*b2 - a2*b1 == 0.)
    {
        std::cout << "Collinear\n";
        return -1;
    }

   

    // The only remaining possibility is intersection of the lines at one point
    std::cout << "Intersection\n";
    return 1;
}
