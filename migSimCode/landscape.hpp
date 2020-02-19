#pragma once

/// header controlling depletion dynamics
#include "params.hpp"
#include <cmath>
#include <vector>

// make gridcell class
class landscape
{
public:
    landscape();
    std::vector<float> resources;
    std::vector<int> foragers;
    float predation;

    void doMakeFood(const float& regrowth);

};

/// landscape constructor
landscape::landscape(){
    predation = predation_cost;
    resources = std::vector<float> (n_patches, maxFood);
    foragers = std::vector<int> (n_patches, 0);
}


/// function to replenish food each generations
void landscape::doMakeFood(const float& regrowth)
{
    for (size_t l = 0; static_cast<int>(l) < n_patches; l++)
    {
        float food_diff = maxFood - resources[l];
        resources[l] += ((food_diff > regrowth) ? regrowth : food_diff);
    }

}

// end LANDSCAPE_HPP
