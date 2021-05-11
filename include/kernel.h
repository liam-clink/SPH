// kernel.h
//
// Author: Liam Clink <clink.6@osu.edu>
//
// This header defines an interface for the kernel functions that
// I use in my simulations.
//

#pragma once
#include <armadillo>

// SPH

// A minimum of 33 neighbors in three dimensions is necessary for good accuracy.
// An idea for this is to periodically adjust the radius using binary search
// Kernel cubic interpolant
// Only needed for interpolating values, including density initialization

// q is the scaled distance, such that q=1 is the range of influence
// This kernel is normalized for 2 dimensional simulation
double cubic_sph_kernel_2d(double q);
arma::vec gradient_cubic_sph_kernel_2d(double q, arma::vec q_hat);


