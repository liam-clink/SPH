// kernel.cpp
//
// Author: Liam Clink <clink.6@osu.edu>
//
// Implementation of the integration and interpolation kernels that I
// commonly use

#include "kernel.h"
#include <armadillo>

#ifndef PI
#include <cmath>
#define PI 4*atan(1.)
#endif

// SPH

// q is the scaled distance. q=1 is the range of influence.
double cubic_sph_kernel_2d(double q)
{
    if (0 <= q <= 0.5)
        return 10. / (3.*PI) * (1. - 6. * (q*q - q*q*q));
    else if (q <= 1)
        return 20. / (3.*PI) * (1. - q) * (1. - q) * (1. - q);
    else
        return 0.;
}

double cubic_sph_kernel_3d(double q)
{
    if (0 <= q <= 0.5)
        return 8. / PI * (1. - 6. * (q*q - q*q*q));
    else if (q <= 1)
        return 16. / PI * (1. - q) * (1. - q) * (1. - q);
    else
        return 0.;
}

arma::vec gradient_cubic_sph_kernel_2d(double q, arma::vec q_hat)
{
    return q_hat;
}

/*
# Gradient of the kernel
# Used in the time evolution equations.
q = np.linalg.norm(separation) / limit_distance
q_hat = separation / q

    if 0 <= q <= 0.5:
        return 8 / np.pi * (-12 * q ** 2 + 18 * q ** 3) * q_hat
    elif q <= 1:
        return -48 / np.pi * (1 - q) ** 2 * q_hat
    else:
        return 0 * q_hat


*/
