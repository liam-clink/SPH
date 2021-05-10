//
// Created by liam on 1/30/20.
//

#pragma once
#include <vector>
#include <array>
#include "particle.h"

// The grid structure for tracking neighbors
// On construction, calculate bounding box and determine cell size
// from the particles, and then use a sorting function to sort the
// particles for increased cache hits and ease/speed of access. After
// sorting, neighbors are determined and tracked.
class Grid
{
    double cell_size;
    std::array<double, 2 * DIM> coordinate_limits;

    // List of pointers with each pointer going to the first particle
    // in the cell, represented either by a block of an array or a
    // linked list

public:
    std::vector<Particle> &particles;
};

// Take in grid and sort particles according to the cell
// index inherited from the grid
void index_sort(Grid);

// Sort according to z-curve, which has better spatial coherence
// than index_sort() and is accessed quickly through bitwise operations
void z_curve_sort(Grid);
