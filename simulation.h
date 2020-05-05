// File: simulation.h
// Author: Liam Clink <clink.6@osu.edu>
// 
// This class takes in an input file and sets up everything necessary
// to do the computation, so that the main function only has the job
// of initializing simulations as instructed by the user.

#pragma once

#include "boundary.h"
#include "node.h"
#include "particle.h"
#include "kernel.h"
#include <vector>
#include <string>

class Simulation
{
public:
    // Because of the Simulation's job of managing the computation,
    // it naturally needs to have manually defined constructor and destructor
    Simulation();
    ~Simulation();

    int run();

private:
    std::vector<SPHParticle> particle_list;
    double width, height;
    unsigned int step = 0;
    unsigned int max_step;
    double dt;
    double duration;
    arma::mat bounding_box;
    // The reason for using "Nodes" is that it allows the simulation to
    // dynamically add any members as necessary, instead of allocating all
    // possible members. A root node does need to be declared, and then
    // anything additional can be added dynamically to the node structure.
    //Node* root = new Node;

    //void grid_ptr;
    //void kernel_ptr;

    int dump_state();
    std::ifstream is;
    std::ofstream os;
    std::vector<std::string> next_line();
};

