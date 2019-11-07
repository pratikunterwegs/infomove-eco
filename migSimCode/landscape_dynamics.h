#pragma once

/// header controlling depletion dynamics
// to be imported after agents.h

#include "params.h"

// make gridcell class
class gridcell
{
public:
	gridcell() : dFood(10.f), dPos(0.f) {};
	~gridcell() {};
	// each gridcell stores nAgents and food
	float dFood, dPos;
};

// init landscape of length landPoints
std::vector<gridcell> landscape(landPoints);

/// function to make positions
void makePositions(std::vector<gridcell>& landscape)
{
	for (int i = 0; i < landPoints; i++)
	{
		landscape[i].dPos = static_cast<float>(i) * maxLandPos / static_cast<float>(landPoints);
	}
}

/// function for wrapped distance
float getWrappedDist(const float& x1, const float& x2)
{
    float tempdist = abs(x2 - x1);
    assert(tempdist <= maxLandPos && "tempdist is beyond land");
    tempdist = std::min(tempdist, maxLandPos - tempdist);
    return tempdist;
}

/// function to replenish food each generations
void doMakeFood()
{
	for (int l = 0; l < landPoints; l++)
	{
		landscape[l].dFood = maxFood;
	}
}
/// function to print landscape values
void printLand(const int& gen_p)
{
	// open or append
	std::ofstream landofs;
	landofs.open("dataLand.csv", std::ofstream::out | std::ofstream::app);
	// col header
	if (gen_p == 0) { landofs << "gen,pos,food\n"; }
	// print for each land cell
	{
		for (int landcell = 0; landcell < landscape.size(); landcell++)
		{
			landofs
				<< gen_p << ","
				<< landscape[landcell].dPos << ","
				<< landscape[landcell].dFood << "\n";
		}
		//close
		landofs.close();
	}
}

// ends here