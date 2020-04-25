// File: simulation.cpp
// Author: Liam Clink <clink.6@osu.edu>
//
// This file implements the simulation object, including selection of
// simulation type and resource management
//

#include "simulation.h"
#include <typeinfo>
#include <fstream>
#include <stdexcept>

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

    // Initialize particles
    unsigned int particle_num;
    tokens = next_line();
    if (tokens[0] != "particle_num")
        throw std::invalid_argument("line 2 is not particle_num");
    particle_num = stoi(tokens[1]);
    this->particle_list = std::vector<SPHParticle>(particle_num);
    std::cout << "Number of Particles: " << particle_num << std::endl;

    // Random number generation, for sampling particles uniformly
    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(0.,1.);
    
    for (unsigned int i=0; i<particle_num; i++)
    {
        particle_list[i] = SPHParticle();
        particle_list[i].position = 
            { this->width*distribution(generator),
              this->height*distribution(generator) };
        particle_list[i].velocity = {0.,0.};
    }

    // Dump the initial state
    system("mkdir -p data/positions");
    system("mkdir -p data/velocities");
    dump_state();

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
    this->os.open("data/positions/"+std::to_string(this->step)+".csv");

    for (int i=0; i<this->particle_list.size(); i++)
    {
        os << i << ','
           << this->particle_list[i].position[0] << ','
           << this->particle_list[i].position[1] << std::endl;
    }

    this->os.open("data/velocities/"+std::to_string(this->step)+".csv");

    for (int i=0; i<this->particle_list.size(); i++)
    {
        os << i << ','
           << this->particle_list[i].velocity[0] << ','
           << this->particle_list[i].velocity[1] << std::endl;
    }

    return 0;
}

