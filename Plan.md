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
