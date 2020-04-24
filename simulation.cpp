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
    std::ifstream ifile;
    ifile.open("input.txt");
    std::vector<std::string> tokens;

    // Initialize simulation box
    tokens = next_line(ifile);
    if (tokens[0] != "height")
        throw std::invalid_argument("line 0 is not height");
    this->height = std::stod(tokens[1]);

    tokens = next_line(ifile);
    if (tokens[0] != "width")
        throw std::invalid_argument("line 1 is not width");
    this->width = std::stod(tokens[1]);

    std::cout << "Height: " << this->height
        << " Width: " << this->width << std::endl;

    // Initialize particles
    // After each particle is added, the density needs to be subtracted from appropriately
    // to ensure good sampling
    unsigned int particle_num;
    tokens = next_line(ifile);
    if (tokens[0] != "particle_num")
        throw std::invalid_argument("line 2 is not particle_num");
    particle_num = stoi(tokens[1]);
    std::vector<SPHParticle> particle_list(particle_num);
    std::cout << "Number of Particles: " << particle_num << std::endl;

    for (unsigned int i=0; i<particle_num; i++)
        particle_list[i] = SPHParticle();

    ifile.close();
}

Simulation::~Simulation()
{
    std::cout << "Done!" << std::endl;
}

// Filter out comment lines, skips through file until a non-comment line
// is reached
std::vector<std::string> next_line(std::ifstream& is)
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

