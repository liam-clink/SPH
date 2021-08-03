// File: particle.h
// Author: Liam Clink <clink.6@osu.edu>
// 
// This header contains the prototypes for various kinds of particles

#pragma once

#include <armadillo>

struct Particle
{
    arma::vec position;
    arma::vec velocity;
};

struct SPHParticle : Particle
{
    double mass;
    double range;
    double pressure;
};

// While in principle the kernel shape could vary from particle to particle,
// this is not sensible and is never done, so the kernel is an attribute of the
// simulation. This is because having a copy of the kernel for every particle
// would be pointless.
