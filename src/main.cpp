// Author: Liam Clink <clink.6@osu.edu>
//

#include "../include/simulation.h"
#include <iostream>
#include <fstream>

template<typename... Args>
void Print(std::ostream& out, Args&&... args)
{
    // Uses C++17 Folding Expressions
    ((out << std::forward<Args>(args) << '\t'), ...);
    out << '\n';
}


int main()
{
    Polygon boundary(
    {
        {0.00000000000000e+00,    6.00000000000000e-01},
        {3.00000000000000e-01,    2.00000000000000e-01},
        {7.00000000000000e-01,    0.00000000000000e+00},
        {1.00000000000000e+00,    3.00000000000000e-01},
        {8.00000000000000e-01,    6.00000000000000e-01},
        {7.50000000000000e-01,    1.00000000000000e+00},
        {4.00000000000000e-01,    5.00000000000000e-01},
        {2.00000000000000e-01,    9.00000000000000e-01}
    });
    
    std::ofstream fs_inside_points("./verification/boundary/round_2/inside.dat");
    std::ofstream fs_outside_points("./verification/boundary/round_2/outside.dat");

    for (double x=-0.1; x<= 1.1; x+=0.01)
    {
        for (double y=-0.1; y<= 1.1; y+=0.01)
        {
            if (point_inside_polygon({x,y},boundary))
            {
                Print(fs_inside_points,x,y);
            }
            else
            {
                Print(fs_outside_points,x,y);
            }
        }
    }

    fs_inside_points.close();
    fs_outside_points.close();

    return 0;
}
