// File: particle.h
// Author: Liam Clink <clink.6@osu.edu>
// 
// This header contains the prototypes for the various kinds of particles
// that I use in my simulations.

#pragma once

#include <array>

struct Particle
{
    double mass;
    std::array<double,DIM> position;
    std::array<double,DIM> velocity;
};

struct SPHParticle : Particle
{
        double range;
};

// While in principle the kernel shape could vary from particle to particle,
// this is not sensible and is never done, so the kernel is an attribute of the simulation
