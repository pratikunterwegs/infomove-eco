#pragma once

/// header controlling depletion dynamics
#include "params.hpp"
#include "agents.hpp"
#include "ann/rnd.hpp"
#include <cmath>

// make gridcell class
class gridcell
{
public:
	gridcell() : dFood(10.f), n_foragers(0) {};
	~gridcell() {};
	// each gridcell stores nAgents and food
	float dFood; int n_foragers;
};

// init landscape of length landPoints
std::vector<gridcell> landscape(n_patches);

///// function to make positions
//void makePositions(std::vector<gridcell>& landscape)
//{
//	for (int i = 0; i < landPoints; i++)
//	{
//		landscape[i].dPos = static_cast<float>(i) / static_cast<float>(landPoints);
//	}
//}

/// function for wrapped distance
//float getWrappedDist(const float& x1, const float& x2)
//{
//    float tempdist = abs(x2 - x1);
//    assert(tempdist <= 1.f && "tempdist is beyond land");
//    tempdist = std::min(tempdist, 1.f - tempdist);
//    return tempdist;
//}

/// function for smootherstep
// take distance from agent, deadzone and deplzone
//float smootherstep(float& x, const float& deadZone, const float& deplZone) {
//	
//	if (x < deadZone) { x = deadZone; }
//	if (x > deplZone) { x = deplZone; }
//	x = (x - deplZone) / (deadZone - deplZone);
//
//	assert(deadZone <= deplZone && "smootherstep: deplZone gt deadZone");
//	return x * x * x * (x * (x * 6 - 15) + 10);
//}

/// function to replenish food each generations
void doMakeFood()
{
	for (size_t l = 0; l < n_patches; l++)
	{
		float food_diff = maxFood - landscape[l].dFood;
		landscape[l].dFood += ((food_diff > max_regrowth) ? max_regrowth : food_diff);
	}

}

/// function to get energy
void agent::doGetFood()
{
	// check where agent is
	assert(pos <= n_patches - 1 && "func doGetFood: agent walked over max land!");
	assert(pos >= 0 && "func doGetFood: agent walked over min land!");

	// get energy
	energy += landscape[pos].dFood / static_cast<float> (landscape[pos].n_foragers);
	energy -= predation_cost / static_cast<float> (landscape[pos].n_foragers);
	energy = energy >= 0.000001f ? energy : 0.000001f;

	assert(energy >= 0.000001f && "func doGetFood: energy below 0");

}

/// function to deplete landscape
// update dFood based on wrapped agent effect
// agent effect is specified by smootherstep above
void agent::depleteFood()
{
	landscape[pos].dFood -= maxDepletion;
	if (landscape[pos].dFood < 0.f) { landscape[pos].dFood = 0.f; }
}

/// wrapper function
int wrapper(int distance, int current_val, int max_val) {

	int new_pos = (max_val + current_val + distance) % max_val;
	return new_pos;
}

void agent::goToLandscape()
{
	landscape[pos].n_foragers += 1;
}

/// function to walk along the circle
void agent::circleWalk()
{
	// check where agent is
	assert(pos <= n_patches - 1 && "func circleWalk: agent now over max land!");
	assert(pos >= 0 && "func circleWalk: agent now over min land!");

	// move up the peak
	int pos_left = wrapper(-1, pos, n_patches);
	int pos_right = wrapper(1, pos, n_patches);

	// remove agent from previous cell
	landscape[pos].n_foragers -= 1;
	assert(landscape[pos].n_foragers >= 0 && "func circleWalk: cell has neg agents!");

	pos = (landscape[pos_left].dFood > landscape[pos_right].dFood) ? pos_left : pos_right;

	landscape[pos].n_foragers += 1;

	assert(pos <= n_patches - 1 && "func circleWalk: agent walked over max land!");
	assert(pos >= 0 && "func circleWalk: agent walked over min land!");

}

/// function to choose explore or exploit
void agent::exploreOrExploit() {
    if (tradeOffParam <= tradeoff_picker(rnd::reng))
	{
		circleWalk();
		total_distance += 1;
		// deplete energy
		energy -= move_cost;
		energy = energy >= 0.000001f ? energy : 0.000001f;
	}
	else
	{
		doGetFood();
		depleteFood();
	}

	// remember food at current pos
	mem_last_pos = landscape[pos].dFood;
}


/// function to implement the foraging dynamic with turns and tradeoff
void do_foraging_dynamic(std::vector<agent>& population, int turns)
{
	// all agents go to landscape
	for (size_t indiv = 0; indiv < popsize; indiv++)
	{
		population[indiv].goToLandscape();
	}

	for (size_t turn_value = 0; turn_value < turns; turn_value++)
	{
		// over as many iterations as foraging_turns agents choose
		// to explore or exploit
		for (size_t indiv = 0; indiv < popsize; indiv++)
		{
			population[indiv].exploreOrExploit();
		}
	}
}


/// memory function to be added here?

/// function to print landscape values
void printLand(const int& gen_p, const int& t_p)
{
	// open or append
	std::ofstream landofs;
	landofs.open("dataLand.csv", std::ofstream::out | std::ofstream::app);
	// col header
	if ((gen_p == 0) && (t_p == 0)) { landofs << "gen,t,pos,food\n"; }
	// print for each land cell
	{
		for (int landcell = 0; landcell < landscape.size(); landcell++)
		{
			landofs
				<< gen_p << ","
				<< t_p << ","
				<< landcell << ","
				<< landscape[landcell].dFood << "\n";
		}
		//close
		landofs.close();
	}
}

// ends here
