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




// Make constructor able to take terminal or file input
Simulation::Simulation()
{
    is.open("input.txt");
    std::vector<std::string> tokens;

    // Initialize simulation box
    tokens = next_line();
    if (tokens[0] != "height")
        throw std::invalid_argument("line 0 is not height");
    this->height = std::stod(tokens[1]);

    tokens = next_line();
    if (tokens[0] != "width")
        throw std::invalid_argument("line 1 is not width");
    this->width = std::stod(tokens[1]);

    std::cout << "Height: " << this->height
        << " Width: " << this->width << std::endl;

    // Hardcoded vertices
    std::vector<arma::vec> temp = {{0.0,0.6},
                                     {0.3,0.2},
                                     {0.7,0.0},
                                     {1.0,0.3},
                                     {0.8,0.6},
                                     {0.75,1.0},
                                     {0.4,0.5},
                                     {0.2,0.9}};
    
    Polygon domain(temp);
    
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

    this->spacing = 0.01;
    for (double x = xmin-5.*spacing; x <= xmax+5.*spacing; x += spacing)
    {
        for (double y = ymin-5.*spacing; y <= ymax+5.*spacing; y += spacing)
        {
            point = {x,y};
            if (point_inside_polygon(point, this->domain) == 0)
            {
                for (int i = 0; i<this->domain.vertices.size(); i++)
                {
                    double distance = dist_to_line_segment(
                        point,
                        Line_Segment(
                            this->domain.vertices[i],
                            this->domain.vertices[(i+1)
                                %this->domain.vertices.size()]));

                    if ( distance <= 5.*spacing )
                    {
                        this->boundary.push_back(SPHParticle());
                        this->boundary.back().position = point;
                    }
                }
            }
        }
    }

    // For now, simulation bounding box will be hardcoded. In future,
    // this will be input via a file.

    
    // Get number of particles and initialize particles
    unsigned int particle_num;
    tokens = next_line();
    if (tokens[0] != "particle_num")
        throw std::invalid_argument("line 2 is not particle_num");
    particle_num = stoi(tokens[1]);
    this->particles = std::vector<SPHParticle>(particle_num);
    std::cout << "Number of Particles: " << particle_num << std::endl;

    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(0.,1.);
    
    // Fill particle list
    for (unsigned int i=0; i<particle_num; i++)
    {
        particles[i] = SPHParticle();
        particles[i].position = 
            { this->width*distribution(generator),
              this->height*distribution(generator) };
        particles[i].velocity = {0.,0.};
    }
    

    // Set up directories for data dumping
    system("mkdir -p data/positions");
    system("mkdir -p data/velocities");

    // Set up time
    tokens = next_line();
    if (tokens[0] != "timestep")
        throw std::invalid_argument("line 3 is not timestep");
    this->dt = stod(tokens[1]);
    tokens = next_line();
    if (tokens[0] != "duration")
        throw std::invalid_argument("line 4 is not duration");
    this->duration = stod(tokens[1]);
    this->max_step = int(duration/dt);

    this->is.close();
}


int Simulation::run()
{
    for(this->step=0; this->step<this->max_step; this->step++)
    {
        dump_state();

        // run
        std::cout << "step " << step << '\n';

    }

    return 0;
}


Simulation::~Simulation()
{
    this->os.close();
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
        std::getline(this->is,line);
        if (line.length() == 0 || line[0] == '#')
            continue;
        else
            break;
    }

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
    std::string step_string = std::to_string(this->step);
    step_string.insert(step_string.begin(),
            log10(this->max_step)+1 - step_string.length(), '0');

    // Output position data
    this->os.open("data/positions/"+step_string+".csv");

    for (int i=0; i<this->particles.size(); i++)
    {
        os << i << ','
           << this->particles[i].position[0] << ','
           << this->particles[i].position[1] << std::endl;
    }
    
    os.close();

    // Output velocity data
    this->os.open("data/velocities/"+step_string+".csv");

    for (int i=0; i<this->particles.size(); i++)
    {
        os << i << ','
           << this->particles[i].velocity[0] << ','
           << this->particles[i].velocity[1] << std::endl;
    }
    os.close();

    return 0;
}
