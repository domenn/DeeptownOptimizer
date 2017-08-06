#pragma once
#include <vector>
#include "Processor.h"
#include <vector>
#include "ProcessingDevices.h"

class Process;
class Item;
class GameObjectContainer;
class Optimizer
{
	// TODO Array that will be used for deciding stuff. Later if implementation gets multithreaded, each thread needs it's own
	int *mineDistribution;
	GameObjectContainer & gameObject;
	std::vector<Item*> chemProductions;
	const Item* const ptrOil;
	const Process* ptrSmelter;
	const Process* ptrGemCrafter;
	const Process* ptrCrafter;
	const Process* ptrGreenhouse;
	const Process* ptrChem;

	const char numSmelterProc;
	const char numGemCrafterProc;
	const char numCrafterProc;
	const char numGreenhouseProc;
	const char numChemProc;



	void calculateMoney();
	void generateRandomSetup();

public:
	const Process* firstProcessorOfType(Devices devices) const;
	static char Optimizer::countDevices(const Process* const begin);
	Optimizer(GameObjectContainer & pGameObject);
	~Optimizer();
};

