// Author: Liam Clink <clink.6@osu.edu>
//

#include "simulation.h"
#include "boundary.h"
#include <iostream>
#include <fstream>

int main()
{

    arma::mat vertices = temp.t();
    vertices.save("vertices.dat",arma::raw_ascii);
    
    double spacing = 0.01;
    double xmin = arma::min(vertices.row(0));
    double xmax = arma::max(vertices.row(0));
    double ymin = arma::min(vertices.row(1));
    double ymax = arma::max(vertices.row(1));
    
    std::ofstream file;
    file.open("boundary.dat");
    
    arma::vec point;

    for (double x = xmin-5.*spacing; x <= xmax+5.*spacing; x += spacing)
    {
        for (double y = ymin-5.*spacing; y <= ymax+5.*spacing; y += spacing)
        {
            point = {x,y};
            if (point_inside_polygon(point, vertices) == 0)
            {
                for (int i = 0; i<vertices.n_cols; i++)
                {
                    if (dist_to_line_segment(point, vertices.col(i),
                        vertices.col((i+1) % vertices.n_cols)) <= 5.*spacing)
                    {
                        file << point(0) << '\t' << point(1) << '\n';
                    }
                }
            }
        }
    }

    file.close();

    return 0;
}
