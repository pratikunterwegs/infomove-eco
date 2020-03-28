#pragma once

/// header controlling depletion dynamics
#include "params.hpp"
#include <cmath>
#include <vector>

# define M_PIl          3.141592653589793238462643383279502884L /* pi */

// make gridcell class
class landscape
{
public:
    landscape();
    std::vector<float> resources;

    void doMakeFood(const float& rho, const float& phi, const float& delta);

};

/// landscape constructor
landscape::landscape(){
    resources = std::vector<float> (n_patches, maxFood);
}

/// function to replenish food each generations
void landscape::doMakeFood(const float& rho, const float& phi, const float& delta)
{
    float max_land, min_land, range_land;
    for (size_t l = 0; static_cast<int>(l) < n_patches; l++)
    {
        resources[l] = sinf(static_cast<float>(M_PIl) * l * phi + delta) +
                (rho * static_cast<float>(gsl_ran_gaussian(r, 0.1)));
        max_land = max_land > resources[l] ? max_land : resources[l];
        min_land = min_land < resources[l] ? min_land : resources[l];
        range_land = max_land - min_land;
    }

    // normalise values
    for (size_t l = 0; static_cast<int>(l) < n_patches; l++)
    {
        resources[l] = (resources[l] - min_land) / range_land;
    }
}

// end LANDSCAPE_HPP
