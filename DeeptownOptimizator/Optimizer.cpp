#include "Optimizer.h"
#include "GameObjectContainer.h"
#include <algorithm>
#include <vector>
#include "MyHelperUtils.h"
#include "ProcessingDevices.h"
#include <fstream>
#include <chrono>

// TODO idea how to make everything better and optimizing in later stage. Array of numbers ... and bounds, min max. Instead of random generations, take items from it

void Optimizer::generateRandomSetup()
{
	std::random_shuffle(mineDistribution, mineDistribution + gameObject.getMaxDepth());
	std::vector<Mine> * mines = gameObject.ptrMines();
	std::vector<HeightMapping> * heights = gameObject.ptrHeightMap();
	for (int i = 0; i < mines->size(); ++i) {
		// Other way assignment is not needed.
		mines->at(i).setHeightMapping(&(gameObject.ptrHeightMap()->at(mineDistribution[i])));
	}
	// Set chemical mine randomness
	std::vector<Item*>* chems = gameObject.ptrChemMineItems();
	for (int i = 0; i < gameObject.getChemMineNumber(); ++i) {
		chemProductions.push_back(chems->at(MyHelperUtils::randomInt(0, gameObject.ptrChemMineItems()->size())));
	}
	// Set random process to each device
	activeProcesses.clear();

	setRandomProccess(ptrProcessSmelter, numSmelterProc);
	setRandomProccess(ptrProcessCrafter, numCrafterProc);
	setRandomProccess(ptrProcessChem, numChemProc);
	setRandomProccess(ptrProcessGemCrafter, numGemCrafterProc);
	setRandomProccess(ptrProcessGreenhouse, numGreenhouseProc);

}

std::vector<int> Optimizer::setInfiniteItemsIndices() const
{
	return std::vector<int>{
			((Item*)MyHelperUtils::findInVectorByString(*gameObject.ptrItems(), Item::findName(ItemName::WATER)))->getIndex(),
			((Item*)MyHelperUtils::findInVectorByString(*gameObject.ptrItems(), Item::findName(ItemName::LIANA_SEED)))->getIndex(),
			((Item*)MyHelperUtils::findInVectorByString(*gameObject.ptrItems(), Item::findName(ItemName::TREE_SEED)))->getIndex(),
			((Item*)MyHelperUtils::findInVectorByString(*gameObject.ptrItems(), Item::findName(ItemName::GRAPE_SEED)))->getIndex()
	};
}

void Optimizer::dumpFile() const
{
	// Now write out some stuff ... debugging purposes
	std::string fn(resultsFileName);
	std::ofstream outFile(fn);

	auto mines = gameObject.ptrMines();

	outFile << "Generated height distribution: " << std::endl;
	for (auto & mn : *mines) {
		outFile << (int)mn.getLevel() << " : " << mn.getHeight() << std::endl;
	}

	outFile << std::endl << std::endl << "How much of each item and money." << std::endl;

	for (int i = 0; i < gameObject.ptrItems()->size(); ++i) {
		if (itemIncomeArrayByIndex[i] > 0.0001 || itemIncomeArrayByIndex[i] < -0.0001) {
			outFile << gameObject.ptrItems()->at(i).itemName() << " : " << itemIncomeArrayByIndex[i] << " : "
				<< gameObject.ptrItems()->at(i).price * itemIncomeArrayByIndex[i] << std::endl;
		}
	}
	outFile << "TOTAL MONEY: " << bestMoney << std::endl;
	outFile.close();
}

const Process* Optimizer::firstProcessOfType(Devices device) const
{
	auto strToFind = Item::findName(device);
	for (auto i = gameObject.ptrProcesses()->data(); ; ++i)
	{
		if (strToFind == i->processor->itemName())
		{
			return i;
		}
	}
}

char Optimizer::countProcessesOfType(const Process * const begin) const
{
	auto ptrEndOfVector = &gameObject.ptrProcesses()->back();
	char num = 0;
	const auto seekName = &begin->processor->itemName();
	auto iterator = begin;
	while (iterator <= ptrEndOfVector && *seekName == iterator->processor->itemName())
	{
		++num;
		++iterator;
	}
	return num;
}


Optimizer::Optimizer(GameObjectContainer & pGameObject) :gameObject(pGameObject),
itemIncomeArrayByIndex(gameObject.ptrItems()->size()),
ptrOil(static_cast<const Item* const>(MyHelperUtils::findInVectorByString(*gameObject.ptrItems(), Item::findName(ItemName::OIL)))),
ptrProcessSmelter(firstProcessOfType(Devices::SMELTER)),
ptrProcessGemCrafter(firstProcessOfType(Devices::JEWELCRAFTER)),
ptrProcessCrafter(firstProcessOfType(Devices::CRAFTER)),
ptrProcessGreenhouse(firstProcessOfType(Devices::GREENHOUSE)),
ptrProcessChem(firstProcessOfType(Devices::CHEM)),
indicesInfiniteItems(setInfiniteItemsIndices()),
numSmelterProc(countProcessesOfType(ptrProcessSmelter)),
numGemCrafterProc(countProcessesOfType(ptrProcessGemCrafter)),
numCrafterProc(countProcessesOfType(ptrProcessCrafter)),
numGreenhouseProc(countProcessesOfType(ptrProcessGreenhouse)),
numChemProc(countProcessesOfType(ptrProcessChem)),
resultsFileName("optimizationResult" + std::to_string(MyHelperUtils::randomInt(0, 2000000)) + ".txt")
{
	// Oil is kind of special item, so I make pointer to it
	//ptrOil = static_cast<Item*>MyHelperUtils::findInVectorByString(*gameObject.ptrItems(),
	//	Item::findName(ItemName::OIL));
	mineDistribution = new int[gameObject.getMaxDepth()];
	for (int i = 0; i < gameObject.getMaxDepth(); ++i)
	{
		mineDistribution[i] = i;
	}
}



void Optimizer::calculateMoney() {
	// Reset vector to zero
	std::fill(itemIncomeArrayByIndex.begin(), itemIncomeArrayByIndex.end(), 0);
	// Set some item income to very high, becouse there are weird ways of getting them
	for (auto indices_infinite_item_index : indicesInfiniteItems)
	{
		itemIncomeArrayByIndex[indices_infinite_item_index] = 60000;
	}

	// First, calculate what mines give me ..
	auto mines = gameObject.ptrMines();
	for (auto &m : *mines) {
		auto income = m.getProductions();
		for (auto &incomeItem : *income) {
			auto speed = m.getSpeed();
			itemIncomeArrayByIndex[incomeItem.item->getIndex()] += incomeItem.numberPercentageFraction() * speed;
		}
	}

	// Then, chems 
	for (int i = 0; i < chemProductions.size(); ++i)
	{
		Item* chemProduced = chemProductions[i];
		itemIncomeArrayByIndex[chemProduced->getIndex()] += GameObjectContainer::CHEM_MINE_SPEED;
	}

	// Oil? Totally special
	itemIncomeArrayByIndex[ptrOil->getIndex()] += gameObject.getOilRate();

	// Then processing devices ... Now there's a problem. If I evaluate smelter first, I 
	// don't yet have info on crafter produced resources ... Well probably this can be handled 
	// if I allow sub-zero quantities of materials

	// Calculate for all processing devices
	for (const Process* active_process : activeProcesses)
	{
		//std::cout << active_process->processor->itemName() << std::endl;
		for (int i = 0; i < active_process->itemIndices.size(); ++i)
		{
			itemIncomeArrayByIndex[active_process->itemIndices[i]] += active_process->itemRates[i];
		}
	}

	// Now calculate how much I get per minute / second and simply calculate money

	double money = 0;
	for (int i = 0; i < itemIncomeArrayByIndex.size(); ++i)
	{
		money += gameObject.ptrItems()->at(i).price * itemIncomeArrayByIndex[i];
	}
	if(money > bestMoney)
	{
		bestMoney = money;
		dumpFile();
	}
}

void Optimizer::setRandomProccess(const Process* process, const char num_smelter_proc)
{
	auto numberOfDevices = process->processor->numberOfThem;
	for (int i = 0; i < numberOfDevices; ++i) {
		activeProcesses.push_back(process + MyHelperUtils::randomInt(0, numSmelterProc));
	}
}


Optimizer::~Optimizer()
{
	delete[] mineDistribution;
}

#define COUT_SPEED_AT 1000

void Optimizer::optimize()
{
	int evaluations = 0;
	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
	


	while (true) {
		generateRandomSetup();
		calculateMoney();
		++evaluations;
		if(evaluations > COUT_SPEED_AT)
		{
			std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count();
			std::cout << "Evaluations per second: " << (double)evaluations / duration <<std::endl;
			evaluations = 0;
		}
	}
}

