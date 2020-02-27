// kernel.h
//
// Author: Liam Clink <clink.6@osu.edu>
//
// This header defines an interface for the kernel functions that
// I use in my simulations.
//

#pragma once

#include <array>

// SPH

// A minimum of 33 neighbors in three dimensions is necessary for good accuracy.
// An idea for this is to periodically adjust the radius using binary search
// Kernel cubic interpolant
// Only needed for interpolating values, including density initialization
 
// This kernel is normalized for 3 dimensional simulation
double cubic_sph_kernel(double scaled_distance);
std::array<double, 3> gradient_cubic_sph_kernel(double scaled_distance);


