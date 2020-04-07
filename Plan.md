I have the basic structure of the program defined, but this took some doing because I wanted to make my
code adaptable/dynamic, and avoid using void pointers because using them is bad practice. I have verified
that the interpolation kernel and its gradient work correctly in Python, and am currently converting it to
C++. Because we are continuing to cover the GSL library, I may not get beyond just having a basic SPH
simulation by the due date. In the python script I also already had an initial condition specified, so
all that is needed beyond that is to get the force calculation working, time integration, and boundaries,
to have a working simulation. However, some more features should be added, and I've specified them in 
the list below.

I write a lot of simulation code, and as I am increasingly wanting to not have to rewrite common parts,
I am trying to write the code so that it is as modular as possible, and so that I can use the same code
base for a variety of different problems as I come to them (sort of like COMSOL). But I am trying to be
careful to not get too bogged down on this and actually accomplish some physics.

An important detail that Mantaflow mentions is that it is important to limit the amount of state contained
in particles, and as much as possible should be kept in the simulation object, so this is a guiding
principle in my coding.

1. First thing to do is get the kernel interpolation working correctly, and output the results for plotting.
This requires that the simulation initialize the particles, and sample the interpolation for output.
This also requires planning the format of the output file.

2. After this, it will be necessary to calculate the force from each particle given the current state,
which should also be plotted and output appropriately. This initial force should just be the pressure force,
as discussed in the Springel paper.

3. Next it will be necessary to output observables such as energy and momentum for verification purposes.

4. Once these are available, we can implement the time evolution/particle push. This will involve time step
selection, as well as interpolation range calculation (eventually).

5. At this point, I should add surface tension, and see the effect it has.

6. Once we get here, I can start implementing the Fast Multipole Method for calculating the magnetic field
from particles given dipole moments.

7. After that, the force calculation (and particles) will need to be modified to include the dipole/field
force (and directly dipole dipole forces if necessary)

8. profit
