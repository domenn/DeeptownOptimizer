#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "GameObjectContainer.h"
#include "MyHelperUtils.h"
#include <sstream>
#include <iostream>
#include "Item.h"
#include <tuple>



GameObjectContainer::GameObjectContainer()
{

}

void parseInputs(const char* const text) {
	char* values = _strdup(text);
	char * token;
}


std::vector<std::tuple<int, Item>>* get_inputs(std::string line_split)
{
	auto inputs = MyHelperUtils::split(line_split, ',');
	std::vector<std::tuple<int, Item>>* inputs;
	for (int i = 0; i < inputs.size(); i += 2) {
		//int
	}
	return inputs;
}


GameObjectContainer GameObjectContainer::createDefaultGameObjectContainer()
{
	// TODO read from file. Now hardcode a string
	std::string sampleInput = "5,COPPER:1,COPPER_BAR:SMELTER:10\n"
		"5,IRON:1,IRON_BAR:SMELTER:15\n"
		"5,GOLD:1,GOLD_BAR:SMELTER:60\n"
		"1,CLEAN_WATER:2,HYDROGEN,1,OXYGEN:CHEM:15m\n"
		"1,CIRCUIT,1,AMBER_INSULATION:1,INSULATED_WIRE:CRAFTER:200\n";

	std::istringstream strstream(sampleInput);
	std::string line;
	char * token;
	while (std::getline(strstream, line)) {
		auto line_split = MyHelperUtils::split(line, ':');
		// Inputs
		auto inputs = get_inputs(line_split[0]);
	}

	GameObjectContainer container = GameObjectContainer();
	//Process()
	//container.processes.emplace_back()
	return GameObjectContainer();
}

GameObjectContainer::~GameObjectContainer()
{
}
