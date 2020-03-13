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

std::normal_distribution<float> res_error(0.f, 0.1f);

/// function to replenish food each generations
void landscape::doMakeFood(const float& rho, const float& phi, const float& delta)
{
    for (size_t l = 0; static_cast<int>(l) < n_patches; l++)
    {
        resources[l] = sinf(static_cast<float>(M_PIl) * l * phi + delta) + (rho * res_error(rng));
        std::cout << resources[l] << " ";
    }
    std::cout << std::endl;

}

// end LANDSCAPE_HPP
