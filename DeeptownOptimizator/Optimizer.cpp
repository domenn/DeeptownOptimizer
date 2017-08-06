#include "Optimizer.h"
#include "GameObjectContainer.h"
#include <algorithm>
#include <vector>
#include <iostream>
#include <memory>
#include <random>
#include "MyHelperUtils.h"
#include "ProcessingDevices.h"

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
		chemProductions.push_back(chems->at(MyHelperUtils::randomInt(0,gameObject.ptrChemMineItems()->size())));
	}

}


Optimizer::Optimizer(GameObjectContainer & pGameObject) :gameObject(pGameObject),
ptrOil(static_cast<const Item* const>(MyHelperUtils::findInVectorByString(*gameObject.ptrItems(),Item::findName(ItemName::OIL)))),
ptrSmelter(static_cast<const Processor*> (MyHelperUtils::findInVectorByString(*pGameObject.ptrProcessors(), Item::findName(Devices::SMELTER)))),
ptrGemCrafter(static_cast<const Processor*> (MyHelperUtils::findInVectorByString(*pGameObject.ptrProcessors(), Item::findName(Devices::JEWELCRAFTER)))),
ptrCrafter(static_cast<const Processor*> (MyHelperUtils::findInVectorByString(*pGameObject.ptrProcessors(), Item::findName(Devices::CRAFTER)))),
ptrGreenhouse(static_cast<const Processor*> (MyHelperUtils::findInVectorByString(*pGameObject.ptrProcessors(), Item::findName(Devices::GREENHOUSE)))),
ptrChem(static_cast<const Processor*> (MyHelperUtils::findInVectorByString(*pGameObject.ptrProcessors(), Item::findName(Devices::CHEM))))
{
	// Oil is kind of special item, so I make pointer to it
	//ptrOil = static_cast<Item*>MyHelperUtils::findInVectorByString(*gameObject.ptrItems(),
	//	Item::findName(ItemName::OIL));
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

	// Then, chems 
	for(int i = 0; i<chemProductions.size(); ++i)
	{
		Item* chemProduced = chemProductions[i];
		arr[chemProduced->getIndex()] += GameObjectContainer::CHEM_MINE_SPEED;
	}

	// Oil? Totally special
	arr[ptrOil->getIndex()] += gameObject.getOilRate();

	// Then processing devices ... Now there's a problem. If I evaluate smelter first, I 
	// don't yet have info on crafter produced resources ... Well probably this can be handled 
	// if I allow sub-zero quantities of materials

	// Smelter
	auto p = gameObject.ptrProcessors();
	auto p1 = gameObject.ptrProcesses();

	// Now calculate how much I get per minute / second and simply calculate money


	// Now write out some stuff ... debugging purposes
	std::cout << "Generated distribution: " << std::endl;
	for (auto & mn : *mines) {
		std::cout << (int)mn.getLevel() << " : " << mn.getHeight() << std::endl;
	}

	std::cout << std::endl << std::endl;

	for (int i = 0; i < gameObject.ptrItems()->size(); ++i) {
		if (arr[i] > 0.0001) {
			std::cout << gameObject.ptrItems()->at(i).itemName() << " : " << arr[i] << std::endl;
		}
	}
	//system("PAUSE");
}












Optimizer::~Optimizer()
{
	delete[] mineDistribution;
}

