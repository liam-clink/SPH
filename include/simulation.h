// File: simulation.h
// Author: Liam Clink <clink.6@osu.edu>
// 
// This class takes in an input file and sets up everything necessary
// to do the computation, so that the main function only has the job
// of initializing simulations as instructed by the user.

#pragma once

#include "boundary.h"
#include "particle.h"
#include "kernel.h"
#include <vector>
#include <string>

/*
// The grid structure for tracking neighbors
// On construction, calculate bounding box and determine cell size
// from the particles, and then use a sorting function to sort the
// particles for increased cache hits and ease/speed of access. After
// sorting, neighbors are determined and tracked.
class Grid
{
    double cell_size;

    // Kitty corner vertices, 2 x dimension
    arma::Mat<double> coordinate_limits;

    // List of pointers with each pointer going to the first particle
    // in the cell, represented either by a block of an array or a
    // linked list


};

// Take in grid and sort particles according to the cell
// index inherited from the grid
void index_sort(Grid);

// Sort according to z-curve, which has better spatial coherence
// than index_sort() and is accessed quickly through bitwise operations
void z_curve_sort(Grid);
*/

class Simulation
{
public:
    // Because of the Simulation's job of managing the computation,
    // it naturally needs to have manually defined constructor and destructor
    Simulation();
    ~Simulation();

    int run();

private:
    std::vector<SPHParticle> particles;
    double width, height;
    unsigned int step = 0;
    unsigned int max_step;
    double dt;
    double duration;
    Polygon domain;
    double spacing;
    std::vector<SPHParticle> boundary;

    int dump_state();
    std::ifstream is;
    std::ofstream os;
    std::vector<std::string> next_line();
};

