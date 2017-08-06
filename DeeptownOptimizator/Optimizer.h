#pragma once
#include <vector>
#include "Processor.h"
#include <vector>

class Item;
class GameObjectContainer;
class Optimizer
{
	// TODO Array that will be used for deciding stuff. Later if implementation gets multithreaded, each thread needs it's own
	int *mineDistribution;
	GameObjectContainer & gameObject;
	std::vector<Item*> chemProductions;
	const Item* const ptrOil;
	const Processor* ptrSmelter;
	const Processor* ptrGemCrafter;
	const Processor* ptrCrafter;
	const Processor* ptrGreenhouse;
	const Processor* ptrChem;

	void calculateMoney();
	void generateRandomSetup();

public:
	Optimizer(GameObjectContainer & pGameObject);
	~Optimizer();
};

