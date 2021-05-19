// Author: Liam Clink <clink.6@osu.edu>
//

#include "../include/simulation.h"
#include <iostream>

int main()
{
    Line_Segment ray({1.,1.},{100.,1.});
    Line_Segment edge({0.,1.},{.9,1.});
    std::cout << line_segment_intersect(ray,edge) << std::endl;
    // Simulation sim;
    return 0;
}
