#pragma once
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
	std::vector<double> itemIncomeArrayByIndex;
	std::vector<Item*> chemProductions;
	const std::vector<int> indicesInfiniteItems;
	const Item* const ptrOil;
	const Process* ptrProcessSmelter;
	const Process* ptrProcessGemCrafter;
	const Process* ptrProcessCrafter;
	const Process* ptrProcessGreenhouse;
	const Process* ptrProcessChem;

	const char numSmelterProc;
	const char numGemCrafterProc;
	const char numCrafterProc;
	const char numGreenhouseProc;
	const char numChemProc;
	//const char numSmelters;
	//const char numGemCrafters;
	//const char numCrafters;
	//const char numGreenhouses;
	//const char numChems;

	std::vector<const Process*> activeProcesses;
	double bestMoney = 0;
	std::string resultsFileName;
	int fileWritesCounter = 0;
	std::string lastFileWrite = "never";


	char countProcessesOfType(const Process* const begin) const;
	void calculateMoney();
	void setRandomProccess(const Process* process, const char num_smelter_proc);
	void generateRandomSetup();
	std::vector<int> setInfiniteItemsIndices() const;
	void dumpFile();
public:
	const Process* firstProcessOfType(Devices devices) const;
	Optimizer(GameObjectContainer & pGameObject);
	~Optimizer();
	void optimize();
};

