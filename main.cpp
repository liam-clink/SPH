// Author: Liam Clink <clink.6@osu.edu>
//

#include "boundary.h"
#include <iostream>

int main()
{
    /*
    arma::arma_rng::set_seed_random();
    arma::vec point = {0.5,0.5};
    arma::mat vertices = {{0.1,0.1},
                          {0.9,0.1},
                          {0.9,0.9},
                          {0.1,0.9}};

    point.print();
    vertices.print();

    int test = point_inside_polygon(point, vertices.t());
    std::cout << "The point is: " << test << std::endl;
    */

    arma::vec point1 = {.6,.5};
    arma::vec point2 = {0.,0.};
    arma::vec point3 = {1.,1.};
    std::cout << "Test: " << point_between_points(point1, point2, point3) << std::endl;
   return 0;
}
