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
#include <algorithm>
#include <fstream>
#include "FileNamesMacros.h"

void checkInputsOutputs(const std::vector<std::tuple<int, Item*>> & itms, int processIndex)
{
	for (const std::tuple<int, Item*>& input : itms)
	{
		if (std::get<0>(input) < 1)
		{
			std::ostringstream os;
			os << "Something is wrong with processes / recipes input file. Please check (line " << processIndex << ") input / output rate.";
			MyHelperUtils::errorExit(os.str().c_str());
		}
		if (std::get<1>(input) == nullptr)
		{
			std::ostringstream os;
			os << "Something is wrong with processes / recipes input file. Please check (line " << processIndex << ") input / output mats.";
			MyHelperUtils::errorExit(os.str().c_str());
		}
	}
}

const double GameObjectContainer::CHEM_MINE_SPEED = 0.5;

void GameObjectContainer::checkRecipesIntegrity()
{
	int processIndex = 0;
	for (Process & process : processes)
	{
		checkInputsOutputs(process.inputs, processIndex);
		checkInputsOutputs(process.outputs, processIndex);
		if (process.timeSeconds < 1)
		{
			std::ostringstream os;
			os << "Something is wrong with processes / recipes input file. Please check (line " << processIndex << ") time.";
			MyHelperUtils::errorExit(os.str().c_str());
		}
		if (process.processor == nullptr)
		{
			std::ostringstream os;
			os << "Something is wrong with processes / recipes input file. Please check (line " << processIndex << ") processor.";
			MyHelperUtils::errorExit(os.str().c_str());
		}
		++processIndex;
	}
}

GameObjectContainer::GameObjectContainer()
{
	// TODO search possible directories for configs
	//Create items .... Read from file probably
	std::ifstream itemsFile(FILENAME_ITEMS);
	MyHelperUtils::checkFileOpen(itemsFile, FILENAME_ITEMS);
	MyHelperUtils::read_objects_into(itemsFile, items);
	itemsFile.close();
	// TODO read from file. Now hardcode a string
	std::ifstream recipesFile(FILENAME_PROCESSES);
	MyHelperUtils::checkFileOpen(recipesFile, FILENAME_ITEMS);

	//Another file ... processors
	std::string sampleInputProcessors = "CRAFTER 6\nCHEM 2\nSMELTER 8\nJEWELCRAFTER 2\nGREENHOUSE 3";
	MyHelperUtils::read_objects_into(std::istringstream(sampleInputProcessors), processors);

	// Finally we fill processes / recipes
	std::string line;
	char * token;
	while (std::getline(recipesFile, line)) {
		if (MyHelperUtils::stringContains(line, "REFI"))
		{
			std::cout << "hir";
		}
		if (MyHelperUtils::stringContains(line, "#") || line == "")
		{
			continue;
		}
		bool whiteSpacesOnly = std::all_of(line.begin(), line.end(), isspace);
		if (whiteSpacesOnly) { continue; }
		auto line_split = MyHelperUtils::split(line, ':');
		// Inputs, outputs
		auto inputs = get_inputs_outputs(line_split[1]);
		auto outputs = get_inputs_outputs(line_split[0]);
		// Processor for process
		const void* const ptrProcessor = MyHelperUtils::findInVectorByString(processors, line_split[2]);
		int time = std::stoi(line_split[3]);
		if (MyHelperUtils::stringContains(line_split[3], "m"))
		{
			time *= 60;
		}

		processes.emplace_back(inputs, outputs, time, (Processor*)ptrProcessor);
	}
	checkRecipesIntegrity();
	// Load height resources data
	std::string sample_heights = "1; coal 100\n"
		"2; coal 70 copper 30\n"
		"3; coal 59 copper 28\n"
		"coal 54 copper 32\n"
		"5; coal 48 copper 36 iron 11\n6;coal 43 copper 40 iron 12\ncoal 38 copper 45 iron 13\n8;coal 33 copper 49 iron 14\ncoal 27 copper 53 iron 15\ncoal 22 copper 57 iron 16\n11;coal 17 copper 61 iron 17\ncoal 12 copper 65 iron 18\n13;copper 100\ncopper 70 iron 30\ncopper 58 iron 19\n16;copper 52 iron 19 amber 15\ncopper 46 iron 19 amber 20\n18;copper 40 amber 25 iron 18\ncopper 35 amber 30 iron 18\n20;amber 35 copper 29 iron 18 ALUMINIUM 11";

	std::istringstream sample_heights_stream(sample_heights);
	int lineNumber = 1;
	while (std::getline(sample_heights_stream, line)) {
		while (MyHelperUtils::stringContains(line, ";")) {
			line.erase(0, 1);
		}
		// TODO replace emplace
		heightMap.push_back(HeightMapping(lineNumber));
		HeightMapping& refCurrentMapping = heightMap.back();
		std::istringstream lineStream(line);
		std::string itmName;
		int number;
		while (lineStream >> itmName) {
			MyHelperUtils::toUpper(itmName);
			auto itm_ptr = (Item*)MyHelperUtils::findInVectorByString(items, itmName);
			lineStream >> number;
			refCurrentMapping.addResource(itm_ptr, number);
		}
		++lineNumber;
	}

	// Chem mine items need to get filled
	fillChemicalMinesVector();

	//TODO for now hardcode chem number ... later need to read from some config
	chem_mine_number = 9;
	// Mine speeds can be baked into software. No big deal
	char mineSpeeds[]{ 3,4,5,6,8,12,15,17 };
	// TODO mine levels from file later
	char mineLevels[]{ 8,8,8,7,7,4,5,7,8,8,7,8 };
	int itms = sizeof(mineLevels) / sizeof(char);
	for (int i = 0; i < itms; ++i) {
		mines.emplace_back(mineSpeeds[mineLevels[i] - 1], mineLevels[i]);
	}

	//TODO maxheight hardcoded. Change to properties ..
	maxDepth = 20;

	//TODO oil number hardcoded ... later read from string
	oilRate = 0.08;
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

int GameObjectContainer::getMaxDepth() const
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

void GameObjectContainer::fillChemicalMinesVector() {
	Item* pointerIterator = items.data();
	std::string targets[]{ Item::findName(ItemName::SILICON),
		Item::findName(ItemName::SULFUR),
		Item::findName(ItemName::SODIUM)
	};

	int targetsLen = sizeof(targets) / sizeof(std::string);
	GameObjectContainer* ptrObt = this;
	std::for_each(targets, targets + targetsLen,
		[ptrObt](std::string & lambdaParameter) {
		ptrObt->ptrChemMineItems()->push_back((Item*)MyHelperUtils::findInVectorByString(*ptrObt->ptrItems(), lambdaParameter));
	});


	/* (std::string & var in targets)
	{

	}*/
	/*
	while (pointerIterator < items.data() + items.size()) {
		if (pointerIterator->itemName() == Item::findName(ItemName::SILICON)) {
			chemMineItems.push_back(pointerIterator);
			++pointerIterator;
		}
	}*/
}

std::vector<Item*>* GameObjectContainer::ptrChemMineItems() {
	return &chemMineItems;
}

std::vector<Processor>* GameObjectContainer::ptrProcessors()
{
	return &processors;
}

std::vector<Process>* GameObjectContainer::ptrProcesses()
{
	return &processes;
}

GameObjectContainer::~GameObjectContainer()
{
}

int GameObjectContainer::getChemMineNumber()
{
	return chem_mine_number;
}

double GameObjectContainer::getOilRate()
{
	return oilRate;
}
