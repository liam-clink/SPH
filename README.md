# Usage
Currently only meant to be run on Linux.

Compile by running 'make'
Open 'input.txt' and edit the numbers after each parameter label.
Run 'sph.x'
To get frames (eventually for an animation of the result using ffmpeg), run "python3 make\_frames.py"

# About
Currently, I have implemented I/O. This took a lot more time and effort
to do than I expected, but now that it's out of the way, about all that
is left to do is the implementation of the physics.
I have implemented the timestepping, so to get the physics working, all
that is needed is force calculation and particle pushing. Because the
time that I've been able to allocate to this has been more limited than
I would've liked, I'm going to have to do this part as a regrade.
Hopefully though I have demonstrated that I am competent at doing what
has been implemented. Unfortunately, the dynamic memory management ate
up a lot of time and I've had to put that off for later on.

I have a python script for generating plots from the output position
data as well, which can be stitched together into an animation for
further troubleshooting. This is hardly the final means of visualization,
but I have to stop now to turn in what I have.

The primary files that you should read are simulation.cpp and simulation.h,
as pretty much all of the currently used code is there. However, I have
started work on the kernel.h and kernel.cpp, which is required for SPH.
