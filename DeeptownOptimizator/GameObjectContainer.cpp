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


int GameObjectContainer::currentIndex = -1;


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
	container.read_objects_into(std::istringstream(sampleInputItems), container.items);
	// TODO read from file. Now hardcode a string
	std::string sampleInput = "5,COPPER:1,COPPER_BAR:SMELTER:10\n"
		"5,IRON:1,IRON_BAR:SMELTER:15\n"
		"5,GOLD:1,GOLD_BAR:SMELTER:60\n"
		"1,CLEAN_WATER:2,HYDROGEN,1,OXYGEN:CHEM:15m\n"
		"1,CIRCUIT,1,AMBER_INSULATION:1,INSULATED_WIRE:CRAFTER:200\n";

	//Another file ... processors
	std::string sampleInputProcessors = "CRAFTING 6\nCHEM 2\nSMELTER 8\nJEWELCRAFTER 2\nGREENHOUSE 3";
	container.read_objects_into(std::istringstream(sampleInputProcessors), container.processors);

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
		void* ptr = MyHelperUtils::findInVectorByString(container.processors, line_split[2]);
		int time = std::stoi(line_split[3]);

		container.processes.emplace_back(inputs, outputs, time, (Processor*)ptr);
	}

	return GameObjectContainer();
}

int GameObjectContainer::generateUniqueIndex()
{
	return ++currentIndex;
}

GameObjectContainer::~GameObjectContainer()
{
}
