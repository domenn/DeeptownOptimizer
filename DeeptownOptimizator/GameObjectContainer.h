#pragma once
#include <vector>
#include "Process.h"
#include "HeightMap.h"
#include "Mine.h"
class GameObjectContainer
{
	std::vector<Process> processes;
	std::vector<Item> items;
	std::vector<Item*> chemMineItems;
	std::vector<HeightMapping> heightMap;
	std::vector<Processor> processors;
	int chem_mine_number;
	std::vector<Mine> mines;
	int maxDepth;
	double oilRate;

	std::vector<std::tuple<int, Item*>> get_inputs_outputs(std::string line_split);
	void fillChemicalMinesVector();
public:
	static const double CHEM_MINE_SPEED;

	void checkRecipesIntegrity();
	GameObjectContainer();
	int getMaxDepth() const;
	std::vector<HeightMapping>* ptrHeightMap();
	std::vector<Mine>* ptrMines();
	std::vector<Item>* ptrItems();
	std::vector<Item*>* ptrChemMineItems();
	std::vector<Processor>* ptrProcessors();
	std::vector<Process>* ptrProcesses();
	~GameObjectContainer();
	int getChemMineNumber();
	double getOilRate();
};
