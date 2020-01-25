import numpy as np
import matplotlib.pyplot as plt

K_0 = 2.15e9  # GPa
K_0_prime = 7.15
rho_0 = 1000  # kg/m^3
P_0 = 101325  # Pa


def pressure(rho):
    return K_0 / K_0_prime * ((rho / rho_0) ** K_0_prime - 1) + P_0


# A minimum of 33 neighbors in three dimensions is necessary for good accuracy.
# An idea for this is to periodically adjust the radius using binary search
def kernel(separation, limit_distance):
    # Kernel interpolant
    # Only needed for interpolating values, including density initialization
    q = np.linalg.norm(separation) / limit_distance

    if 0 <= q <= 0.5:
        return 8 / np.pi * (1 - 6 * (q ** 2 - q ** 3))
    elif q <= 1:
        return 16 / np.pi * (1 - q) ** 3
    else:
        return 0


def grad_kernel(separation, limit_distance):
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


class Particle:
    def __init__(self, mass, position, velocity):
        self.mass = mass
        self.position = position
        self.velocity = velocity
        self.density = 0
        self.pressure = 0


def gaussian(x, mean, stdev):
    distribution = np.exp(-(x - mean) ** 2 / (2 * stdev ** 2))

    # Normalize the distribution
    norm = 0
    for i in range(len(x)):
        norm += distribution[i]
    distribution /= norm

    return distribution


def plot_positions(simulation, name):
    # Gather positions from simulation (so inefficient...)
    positions = []
    for particle in simulation.particles:
        positions.append(particle.position)

    # Separate positions into 2 lists for plotting
    x_vals, y_vals = zip(*positions)
    plt.scatter(x_vals, y_vals)
    plt.savefig(name)


class Simulation:
    def __init__(self, number, mass, avg_density, avg_velocity, std_velocity, dimensions):
        self.width = dimensions[0]
        self.height = dimensions[1]

        # Create an initial set of velocities to sample from with a normal distribution
        initial_velocities = np.linspace(-2 * std_velocity, 2 * std_velocity, 1000)
        velocity_distribution = gaussian(initial_velocities, avg_velocity, std_velocity)

        # Initialize particle list
        self.number = number
        self.particles = []
        effective_radius = (3 / (4 * np.pi) * mass / avg_density) ** (1 / 3)
        for i in range(self.number):
            # Initialize positions (SWITCH TO PERTURBED EVEN SPACING)
            position = np.zeros(2)
            for j in range(2):
                position[j] = np.random.uniform(effective_radius, dimensions[j] - effective_radius)

            # Enforce Dam Breaking initial condition
            if position[0] > 0.5 * self.width and position[1] > 0.5 * self.height:
                continue

            # Set initial velocity
            velocity = np.zeros(2)
            for j in range(2):
                velocity[j] = np.random.choice(initial_velocities, p=velocity_distribution)

            self.particles.append(Particle(mass, position, velocity))

        plot_positions(self, "initial_positions.png")


Simulation = Simulation(1000, 1, 1, 0, 1, (10, 20))
