// Author: Liam Clink <clink.6@osu.edu>
//
#define DIM 2

#include "particle.h"
#include <iostream>

int main()
{
    Particle test;
    test.mass = 1.;
    test.position = {1.,2.};
    std::cout << test.position[1] << std::endl;
    return 0;
}
