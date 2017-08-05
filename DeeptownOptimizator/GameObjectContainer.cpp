#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "GameObjectContainer.h"
#include "MyHelperUtils.h"
#include <sstream>
#include <iostream>
#include "Item.h"
#include <algorithm>
#include <tuple>
#include "MatchStringToEnum.h"





GameObjectContainer::GameObjectContainer()
{

}

//void parseInputs(const char* const text) {
//	char* values = _strdup(text);
//	char * token;
//}


std::vector<std::tuple<int, Item*>> GameObjectContainer::get_inputs_outputs(std::string line_split)
{
	auto inputs_strings = MyHelperUtils::split(line_split, ',');
	std::vector<std::tuple<int, Item*>> inputs;
	for (int i = 0; i < inputs_strings.size(); i += 2) {
		int in1 = std::stoi(inputs_strings[i]);
		// TODO here allow uppercase, spaces
		// TODO this code is dangerous, check if any input string is wrong
		Item* pItem = (Item*)MyHelperUtils::findInVectorByString(items, inputs_strings[i + 1]);
		inputs.push_back(std::make_tuple(in1, pItem));
		// TODO error if item search was unsuccessful
	}
	return inputs;
}
/*
void GameObjectContainer::read_objects_into(std::istream & itemstream)
{
	std::string line;
	while (std::getline(itemstream, line)) {
		std::istringstream item_line_stream(line);
		std::string item_name;
		int price;
		item_line_stream >> item_name >> price;
		items.emplace_back(item_name, price);
	}
}
*/

GameObjectContainer GameObjectContainer::createDefaultGameObjectContainer()
{
	GameObjectContainer container = GameObjectContainer();
	//Create items .... Read from file probably
	std::string sampleInputItems = "COAL 1\nCOPPER 2\nIRON 3\nAMBER 4\nCOPPER_BAR 25\nIRON_BAR 40\nGOLD 10\nGOLD_BAR 250\nCIRCUITS 2070\nAMBER_INSULATION 125\nINSULATED_WIRE 750\nWATER 5\nCLEAN_WATER 1200\nHYDROGEN 400\nOXYGEN 900";
	MyHelperUtils::read_objects_into(std::istringstream(sampleInputItems), container.items);
	// TODO read from file. Now hardcode a string
	std::string sampleInput = "5,COPPER:1,COPPER_BAR:SMELTER:10\n"
		"5,IRON:1,IRON_BAR:SMELTER:15\n"
		"5,GOLD:1,GOLD_BAR:SMELTER:60\n"
		"1,CLEAN_WATER:2,HYDROGEN,1,OXYGEN:CHEM:15m\n"
		"1,CIRCUIT,1,AMBER_INSULATION:1,INSULATED_WIRE:CRAFTER:200\n";

	//Another file ... processors
	std::string sampleInputProcessors = "CRAFTING 6\nCHEM 2\nSMELTER 8\nJEWELCRAFTER 2\nGREENHOUSE 3";
	MyHelperUtils::read_objects_into(std::istringstream(sampleInputProcessors), container.processors);

	// Finally we fill processes / recepies
	std::istringstream strstream(sampleInput);
	std::string line;
	char * token;
	while (std::getline(strstream, line)) {
		auto line_split = MyHelperUtils::split(line, ':');
		// Inputs, outputs
		auto inputs = container.get_inputs_outputs(line_split[0]);
		auto outputs = container.get_inputs_outputs(line_split[1]);
		// Processor for process
		const void* const ptr = MyHelperUtils::findInVectorByString(container.processors, line_split[2]);
		int time = std::stoi(line_split[3]);

		container.processes.emplace_back(inputs, outputs, time, (Processor*)ptr);
	}
	// Load height resources data
	std::string sample_heights = "1; coal 100\n"
		"2; coal 70 copper 30\n"
		"3; coal 59 copper 28\n"
		"coal 54 copper 32\n"
		"5; coal 48 copper 36 iron 11\n6;coal 43 copper 40 iron 12\ncoal 38 copper 45 iron 13\n8;coal 33 copper 49 iron 14\ncoal 27 copper 53 iron 15\ncoal 22 copper 57 iron 16\n11;coal 17 copper 61 iron 17\ncoal 12 copper 65 iron 18\n13;copper 100\ncopper 70 iron 30\ncopper 58 iron 19\n16;copper 52 iron 19 amber 15\ncopper 46 iron 19 amber 20\n18;copper 40 amber 25 iron 18\ncopper 35 amber 30 iron 18\n20;amber 35 copper 29 iron 18 ALUMINIUM 11\n";
	
	std::istringstream sample_heights_stream(sample_heights);
	int lineNumber = 1;
	while (std::getline(sample_heights_stream, line)) {
		while (MyHelperUtils::stringContains(line, ";")) {
			line.erase(0, 1);
		}
		// TODO replace emplace
		container.heightMap.push_back(HeightMapping(lineNumber));
		HeightMapping& refCurrentMapping = container.heightMap.back();
		std::istringstream lineStream(line);
		std::string itmName;
		int number;
		while (lineStream >> itmName) {
			MyHelperUtils::toUpper(itmName);
			auto itm_ptr = (Item*)MyHelperUtils::findInVectorByString(container.items, itmName);
			lineStream >> number;
			refCurrentMapping.addResource(itm_ptr, number);
		}
		++lineNumber;
	}

	//TODO for now hardcode chem number ... later need to read from some config
	container.chem_mine_number = 9;
	// Mine speeds can be baked into software. No big deal
	char mineSpeeds[]{ 3,4,5,6,8,12,15,17 };
	// TODO mine levels from file later
	char mineLevels[]{ 8,8,8,7,7,4,5,7,8,8,7,8 };
	int itms = sizeof(mineLevels) / sizeof(char);
	for (int i = 0; i < itms; ++i) {
		container.mines.emplace_back(mineSpeeds[mineLevels[i]-1], mineLevels[i]);
	}

	//TODO maxheight hardcoded. Change to properties ..
	container.maxDepth = 20;
	
	return container;
}

int GameObjectContainer::getMaxDepth()
{
	return maxDepth;
}

std::vector<HeightMapping>* GameObjectContainer::ptrHeightMap()
{
	return &heightMap;
}

std::vector<Mine>* GameObjectContainer::ptrMines()
{
	return &mines;
}

std::vector<Item>* GameObjectContainer::ptrItems()
{
	return &items;
}

GameObjectContainer::~GameObjectContainer()
{
}
