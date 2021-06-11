// File: simulation.cpp
// Author: Liam Clink <clink.6@osu.edu>
//
// This file implements the simulation object, including selection of
// simulation type and resource management
//

#include "simulation.h"
#include "geometry.h"
#include <typeinfo>
#include <fstream>
#include <stdexcept>
#include <cmath> // for zero filling

/*
// The grid structure for tracking neighbors
// On construction, calculate bounding box and determine cell size
// from the particles, and then use a sorting function to sort the
// particles for increased cache hits and ease/speed of access. After
// sorting, neighbors are determined and tracked.
class Grid
{
    double cell_size;

    // Kitty corner vertices, 2 x dimension
    arma::Mat<double> coordinate_limits;

    // List of pointers with each pointer going to the first particle
    // in the cell, represented either by a block of an array or a
    // linked list

public:
    std::vector<Particle> &particles;
};
*/
// Take in grid and sort particles according to the cell
// index inherited from the grid
// void index_sort(Grid);

// Sort according to z-curve, which has better spatial coherence
// than index_sort() and is accessed quickly through bitwise operations
// void z_curve_sort(Grid);

// Compact hashing




//TODO: Make constructor able to take terminal or file input
Simulation::Simulation()
{
    is.open("input.txt");
    std::vector<std::string> tokens;

    // Initialize simulation box
    tokens = next_line();
    if (tokens[0] != "height")
        throw std::invalid_argument("line 0 is not height");
    height = std::stod(tokens[1]);

    tokens = next_line();
    if (tokens[0] != "width")
        throw std::invalid_argument("line 1 is not width");
    width = std::stod(tokens[1]);

    std::cout << "Height: " << height
        << " Width: " << width << std::endl;

    // Get number of particles and initialize particles
    tokens = next_line();
    if (tokens[0] != "particle_num")
        throw std::invalid_argument("line 2 is not particle_num");
    unsigned int particle_num = stoi(tokens[1]);
    particles = std::vector<SPHParticle>(particle_num);
    std::cout << "Number of Particles: " << particle_num << std::endl;

    // Set up time
    tokens = next_line();
    if (tokens[0] != "timestep")
        throw std::invalid_argument("line 3 is not timestep");
    dt = stod(tokens[1]);
    std::cout << "Timestep: " << dt << ' ';

    tokens = next_line();
    if (tokens[0] != "duration")
        throw std::invalid_argument("line 4 is not duration");
    duration = stod(tokens[1]);
    std::cout << "Duration: " << duration << '\n';
    max_step = int(duration/dt);

    is.close();

    // Read in vertices of polygon boundary
    Polygon domain;

    is.open("boundary.txt");
    do
    {
        tokens = next_line();
        domain.vertices.push_back(arma::vec(tokens[0]+' '+tokens[1]));
    } while(tokens.empty() == false);
    is.close();

    std::cout << "Vertices of the bounding polygon: \n";
    for (int i = 0; i<domain.vertices.size(); i++)
    {
        std::cout << domain.vertices[i](0) << domain.vertices[i](1) << '\n';
    }
    
    double xmin = domain.vertices[0](0);
    double xmax = xmin;
    double ymin = domain.vertices[0](1);
    double ymax = ymin;
    
    for (auto vertex : domain.vertices)
    {
        if (vertex(0) < xmin)
            xmin = vertex(0);
        else if (vertex(0) > xmax)
            xmax = vertex(0);
        if (vertex(1) < ymin)
            ymin = vertex(1);
        else if (vertex(1) > ymax)
            ymax = vertex(1);
    }
    
    arma::vec point;

    spacing = 0.01;
    for (double x = xmin-5.*spacing; x <= xmax+5.*spacing; x += spacing)
    {
        for (double y = ymin-5.*spacing; y <= ymax+5.*spacing; y += spacing)
        {
            point = {x,y};
            if (point_inside_polygon(point, domain) == 0)
            {
                for (int i = 0; i<domain.vertices.size(); i++)
                {
                    double distance = distance_to_line_segment(
                        point,
                        Line_Segment(
                            domain.vertices[i],
                            domain.vertices[(i+1)
                                %domain.vertices.size()]));

                    if ( distance <= boundary_thickness*spacing )
                    {
                        boundary.push_back(SPHParticle());
                        boundary.back().position = point;
                    }
                }
            }
        }
    }

    // For now, simulation bounding box will be hardcoded. In future,
    // this will be input via a file.

    
    // Initialize particles

    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(0.,1.);
    
    // Fill particle list
    for (unsigned int i=0; i<particle_num; i++)
    {
        particles[i] = SPHParticle();
        particles[i].position = 
            { width*distribution(generator),
              height*distribution(generator) };
        particles[i].velocity = {0.,0.};
    }
    

    // Set up directories for data dumping
    //TODO: Make system agnostic
    system("mkdir -p data/positions");
    system("mkdir -p data/velocities");

}


int Simulation::run()
{
    for(step=0; step<max_step; step++)
    {
        dump_state();

        // run
        std::cout << "step " << step << '\n';

    }

    return 0;
}


Simulation::~Simulation()
{
    os.close();
    std::cout << "Done!" << std::endl;
}


// Filter out comment lines, skips through file until a non-comment line
// is reached
std::vector<std::string> Simulation::next_line()
{
    // Find the next line that isn't a comment
    std::string line;
    while(true)
    {
        std::getline(is,line);
        if (line.length() == 0 || line[0] == '#')
            continue;
        else
            break;
    }
    //TODO: Need to throw error if end of file is reached

    // Tokenize the line
    std::string delimiter = " ";
    size_t pos=0;
    std::string token;
    std::vector<std::string> tokenized_line;
    while ((pos = line.find(delimiter)) != std::string::npos)
    {
        token = line.substr(0,pos);
        tokenized_line.push_back(token);
        line.erase(0, pos + delimiter.length());
    }
    tokenized_line.push_back(line);

    return tokenized_line;
}


int Simulation::dump_state()
{
    // Do zero filling for filename
    std::string step_string = std::to_string(step);
    step_string.insert(step_string.begin(),
            log10(max_step)+1 - step_string.length(), '0');

    // Output position data
    os.open("data/positions/"+step_string+".csv");

    for (int i=0; i<particles.size(); i++)
    {
        os << i << ','
           << particles[i].position[0] << ','
           << particles[i].position[1] << std::endl;
    }
    
    os.close();

    // Output velocity data
    os.open("data/velocities/"+step_string+".csv");

    for (int i=0; i<particles.size(); i++)
    {
        os << i << ','
           << particles[i].velocity[0] << ','
           << particles[i].velocity[1] << std::endl;
    }
    os.close();

    return 0;
}
