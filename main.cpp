// Author: Liam Clink <clink.6@osu.edu>
//
#define DIM 2

#include "simulation.h"
#include <iostream>

int main()
{
    SPHParticle test;
    test.density = 1.;
    test.position = {1.,2.};
    std::cout << test.position[1] << std::endl;
    return 0;
}
