#pragma once
#include <vector>
#include "Process.h"
#include "HeightMap.h"
#include "Mine.h"
class GameObjectContainer
{
	std::vector<Process> processes;
	std::vector<Item> items;
	std::vector<HeightMapping> heightMap;
	std::vector<Processor> processors;
	int chem_mine_number;
	std::vector<Mine> mines;
	int maxDepth;

	std::vector<std::tuple<int, Item*>> get_inputs_outputs(std::string line_split);
	
public:
	GameObjectContainer();
	int getMaxDepth();
	std::vector<HeightMapping>* ptrHeightMap();
	std::vector<Mine>* ptrMines();
	std::vector<Item>* ptrItems();
	~GameObjectContainer();
};
