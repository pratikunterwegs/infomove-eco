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

    void doMakeFood(const float& rho, const float& phi);

};

/// landscape constructor
landscape::landscape(){
    resources = std::vector<float> (n_patches, 0.f);
}

/// function to replenish food each generations
void landscape::doMakeFood(const float& phi, const float& rho)
{
    float max_land = 0.f, min_land = 0.f, range_land = 0.f;
    for (size_t l = 0; static_cast<int>(l) < n_patches; l++)
    {
        // r = sin(2pi * phi * l / n_patches)
        resources[l] = sinf((2.f * static_cast<float>(M_PIl) * (static_cast<float>(l) / static_cast<float>(n_patches)) * phi)) +
                (static_cast<float>(gsl_ran_gaussian(r, rho)));

        // normalise for negative values
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
