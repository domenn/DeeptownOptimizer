#include "Optimizer.h"
#include "GameObjectContainer.h"
#include <algorithm>
#include <vector>

Optimizer::Optimizer(GameObjectContainer & pGameObject):gameObject(pGameObject)
{
	// TODO testing code ... just calculate numbers. Later add logic.
	mineDistribution = new int[gameObject.getMaxDepth()];
	for (int i = 0; i < gameObject.getMaxDepth(); ++i) {
		mineDistribution[i] = i;
	}
	
	std::vector<Mine> * mines = gameObject.ptrMines();
	std::vector<HeightMapping> * heights = gameObject.ptrHeightMap();
	// TODO TO FUNCTION

	std::random_shuffle(mineDistribution, mineDistribution + gameObject.getMaxDepth());
	for (int i = 0; i < mines->size(); ++i) {
		// TODO: on setHeight, get actual item from heights map and assign it to mine. So mine will know how much it produces.
		// Other way assignment is not needed.
		mines->at(i).setHeight(mineDistribution[i]);
	}
}

Optimizer::~Optimizer()
{
	delete[] mineDistribution;
}
