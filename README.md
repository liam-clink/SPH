# Usage
Currently only meant to be run on Linux.

Compile by running 'make'
Open 'input.txt' and edit the numbers after each parameter label.
Run 'sph.x'
To get an animation of the result using ffmpeg, run "make\_movie.sh"

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
