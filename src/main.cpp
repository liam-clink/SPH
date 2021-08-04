// Author: Liam Clink <clink.6@osu.edu>
//

#include "../include/simulation.h"
#include <iostream>
#include <fstream>

template<typename... Args>
void easy_print(std::ostream& out, Args&&... args)
{
    // Uses C++17 Folding Expressions
    ((out << std::forward<Args>(args) << '\t'), ...);
    out << '\n';
}


int main()
{
    Simulation SPHsim;
    SPHsim.sample_density(1000,1000);

    return 0;
}
