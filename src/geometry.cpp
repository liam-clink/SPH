// File: geometry.cpp
// Author: Liam Clink <clink.6@osu.edu>
//
// Implementation of the geometry.h header
//

#include "geometry.h"
#include <cfloat>
#include <algorithm> // for max(a,b) and max_element()
#include <cmath>

Line_Segment::Line_Segment(arma::vec _start, arma::vec _end)
{
    start = _start;
    end = _end;
}

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
double distance_to_line_segment(const arma::vec& point,
                                const Line_Segment& segment)
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


bool point_inside_polygon(const arma::vec& point, const Polygon& polygon)
{
    // Determine bounding rectangle
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

    
    // Is point in bounding rectangle?
    if (point(0)<xmin || point(0)>xmax || point(1)<ymin || point(1)>ymax)
    {
        return false;
    }

    // At this stage, the point is at least inside the bounding box.
    // Now ray casting is done to check whether point is inside.
    // It is simpler to check vertex intersection if ray is horizontal or
    // vertical. I choose the ray to be horizontal to the right.

    int intersections = 0;
    Line_Segment edge;
    // Do a loop over the number of edges
    for (int i=0; i<polygon.vertices.size(); i++)
    {
        // TODO: faster if changed to move constructor
        edge = Line_Segment(polygon.vertices[i],
            polygon.vertices[(i+1) % polygon.vertices.size()]);

        // Do a bunch of fast checks of trivial conditions
        if ((point(0) >= edge.start(0)) && (point(0) >= edge.end(0)))
        {
            // The point is to the right of the edge
            continue;
        }
        if ((point(1) > edge.start(1)) && (point(1) > edge.end(1)))
        {
            // The point is above the edge
            continue;
        }
        if ((point(1) < edge.start(1)) && (point(1) < edge.end(1)))
        {
            // The point is below the edge
            continue;
        }
        
        if (point(1) == edge.start(1))
        {
            // The point intersects the start vertex. If the vertex were
            // shifted up slightly, would this cause an intersection?
            // Shifting up or down doesn't matter as long as it is consistent.
            // The case of start == end is excluded because horizontal lines
            // can be ignored.
            if (edge.start(1) > edge.end(1)) intersections++;
            else continue;
        }
        if (point(1) == edge.end(1))
        {
            // The point intersects the end vertex, similar to above.
            if (edge.end(1) > edge.start(1)) intersections++;
            else continue;
        }

        if ((point(0) < edge.start(0)) && (point(0) < edge.end(0)))
        {
            // The point is to the left of the edge, in between top and bottom,
            // and so the ray definitely intersects the edge
            intersections++;
            continue;
        }

        // Now all that is left is the possibility that the point is inside
        // the bounding box of the edge, which is not horizontal. So we need
        // to determine which side of the line the point is.

        // Convert the edge to a line of infinite length in linear equation
        // standard form: Ax + By + C = 0. The equivalent in higher dimensions
        // is calculating the intersection of a hyperplane with the ray.
        
        double a = edge.end(1) - edge.start(1);
        double b = edge.start(0) - edge.end(0);
        double c = edge.end(0)*edge.start(1) - edge.start(0)*edge.end(1);

        // The normal vector for the segment is (a,b) so we dot product the
        // separation of the point from a vertex with the normal, and also
        // check whether the normal vector is in the +x or -x direction,
        // and we only count an intersection when the dot product and the x
        // component of the normal vector are opposite
        if ((std::signbit((point(0)-edge.start(0))*a
                         + (point(1)-edge.start(1))*b) > 0)
            xor (std::signbit(a) > 0))
        {
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
int line_segment_intersect(const Line_Segment& segment1,
                           const Line_Segment& segment2)
{
    // Convert line segment 1 (endpoint1 to endpoint2) to a line of infinite
    // length in linear equation standard form: Ax + By + C = 0. The equivalent
    // in higher dimensions is calculating the intersection of a hyperplane
    // with the ray.
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
    // TODO: This returns even if the line segments have no overlap, fix
    if (a1*b2 - a2*b1 == 0.)
    {
        return -1;
    }

    // The only remaining possibility is intersection of the lines at one point
    return 1;
}
