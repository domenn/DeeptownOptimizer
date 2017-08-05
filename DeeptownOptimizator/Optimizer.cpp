#include "Optimizer.h"
#include "GameObjectContainer.h"
#include <algorithm>
#include <vector>
#include <iostream>
#include <memory>

void Optimizer::generateRandomSetup()
{
	std::random_shuffle(mineDistribution, mineDistribution + gameObject.getMaxDepth());
	std::vector<Mine> * mines = gameObject.ptrMines();
	std::vector<HeightMapping> * heights = gameObject.ptrHeightMap();
	for (int i = 0; i < mines->size(); ++i) {
		// Other way assignment is not needed.
		mines->at(i).setHeightMapping(&(gameObject.ptrHeightMap()->at(mineDistribution[i])));
	}
}

using namespace std;

Optimizer::Optimizer(GameObjectContainer & pGameObject) :gameObject(pGameObject)
{
	// TODO testing code ... just calculate numbers. Later add logic.
	mineDistribution = new int[gameObject.getMaxDepth()];
	for (int i = 0; i < gameObject.getMaxDepth(); ++i) {
		mineDistribution[i] = i;
	}

	generateRandomSetup();
	calculateMoney();




}

void Optimizer::calculateMoney() {
	// Prepare empty array of material income numbers to fill up and substract down in the proccess.
	std::unique_ptr<double[]> arr(new double[gameObject.ptrItems()->size()]());
	//TODO put arr to 0. Actually make somewhere prepared array, then just copy ... measure difference!

	// First, calculate what mines give me ..
	auto mines = gameObject.ptrMines();
	for (auto &m : *mines) {
		auto income = m.getProductions();
		for (auto &incomeItem : *income) {
			auto speed = m.getSpeed();
			arr[incomeItem.item->getIndex()] += incomeItem.numberPercentageFraction() * speed;
		}
	}


	// Now write out some stuff ... debugging purposes
	cout << "Generated distribution: " << endl;
	for (auto & mn : *mines) {
		cout << (int)mn.getLevel() << " : " << mn.getHeight() << endl;
	}

	cout << endl << endl;

	for (int i = 0; i < gameObject.ptrItems()->size(); ++i) {
		if (arr[i] > 0.0001) {
			cout << gameObject.ptrItems()->at(i).itemName() << " : " << arr[i] << endl;
		}
	}

	// Then, chems 

	// Then processing devices ... Now there's a problem. If I evaluate smelter first, I 
	// don't yet have info on crafter produced resources ... Well probably this can be handled 
	// if I allow sub-zero quantities of materials

	// Now calculate how much I get per minute / second and simply calculate money
}












Optimizer::~Optimizer()
{
	delete[] mineDistribution;
}

