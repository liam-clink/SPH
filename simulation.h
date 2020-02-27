// simulation.h
//
// Author: Liam Clink <clink.6@osu.edu>
// 
// This class takes in an input file and sets up everything necessary
// to do the computation, so that the main function only has the job
// of initializing simulations as instructed by the user.


class Simulation
{
    // Because of the Simulation's job of managing the computation,
    // it naturally needs to have manually defined constructor and destructor
    Simulation();
    ~Simulation();

    // Use pointers to access memory allocated by the constructor, because
    // memory allocation and variables will be dependent on the input file.
    // I think it should be possible to use pointer/data structure initialized
    // by the constructor instead of multiple pointers like this, but for now
    // I will leave it this way.
    void particles_ptr;
    void grid_ptr;
    void kernel_ptr;
};
