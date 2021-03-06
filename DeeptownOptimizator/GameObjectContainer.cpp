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
#include "ProcessingDevices.h"

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
	// Simple trick, manipulate prices of some items to make optimization easier
	hackItemPrices();
	std::ifstream recipesFile(FILENAME_PROCESSES);
	MyHelperUtils::checkFileOpen(recipesFile, FILENAME_ITEMS);

	//Another file ... processors
	std::string sampleInputProcessors = readProcessingDevicesFromConfig();
	MyHelperUtils::read_objects_into(std::istringstream(sampleInputProcessors), processors);

	// Finally we fill processes / recipes
	std::string line;
	while (std::getline(recipesFile, line)) {
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
	recipesFile.close();
	checkRecipesIntegrity();
	// Load height resources data


	std::ifstream sample_heights_stream(FILENAME_DEPTHS);
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
		double number;
		while (lineStream >> itmName) {
			MyHelperUtils::toUpper(itmName);
			auto itm_ptr = (Item*)MyHelperUtils::findInVectorByString(items, itmName);
			lineStream >> number;
			refCurrentMapping.addResource(itm_ptr, number);
		}
		++lineNumber;
	}
	sample_heights_stream.close();
	fillChemicalMinesVector();
	readUserConfigFile();
}

void GameObjectContainer::readUserConfigFile()
{
	std::ifstream cfgFile(FILENAME_PLAYER_CONFIG);
	std::string line;
	// Mine speeds can be baked into software. No big deal
	char mineSpeeds[]{ 3,4,5,6,8,12,15,17 };
	while (std::getline(cfgFile, line)) {
		if (MyHelperUtils::stringContains(line, "#"))
		{
			continue;
		}
		MyHelperUtils::toUpper(line);
		if(MyHelperUtils::stringContains(line,"NUMBER_CHEM_MINES"))
		{
			chem_mine_number = std::stoi(MyHelperUtils::split(line, ':')[1]);
		}
		if (MyHelperUtils::stringContains(line, "MINE_LEVELS"))
		{
			std::string value = MyHelperUtils::split(line, ':')[1];
			auto itms = MyHelperUtils::split(value, ' ');
			for (auto& itm : itms)
			{
				if(MyHelperUtils::stringContains(itm,","))
				{
					itm.erase(0, 1);
					itm.erase(itm.end() - 1);
					auto numbers = MyHelperUtils::split(itm, ',');
					int numberOfThem = std::stoi(numbers[0]);
					int levelOfBundle = std::stoi(numbers[1]);
					for (int i = 0; i < numberOfThem; ++i) {
						mines.push_back(Mine(mineSpeeds[levelOfBundle-1], levelOfBundle));
					}
				}else
				{
					int level = std::stoi(itm);
					mines.push_back(Mine(mineSpeeds[level-1], level));
				}
			}
		}
		if (MyHelperUtils::stringContains(line, "MAX_DEPTH"))
		{
			maxDepth = std::stoi(MyHelperUtils::split(line, ':')[1]);
		}
		if (MyHelperUtils::stringContains(line, "OIL_RATE"))
		{
			oilRate = std::stof(MyHelperUtils::split(line, ':')[1]);
		}
	}
}

//void parseInputs(const char* const text) {
//	char* values = _strdup(text);
//	char * token;
//}


void GameObjectContainer::hackItemPrices()
{
	auto water = (Item*)MyHelperUtils::findInVectorByString(items, Item::findName(ItemName::WATER));
	water->price = 0;
	//seeds
	for(int i = 0; i<items.size(); ++i)
	{
		if (MyHelperUtils::stringContains(items[i].itemName(), "_SEED"))
		{
			std::string plant = MyHelperUtils::split(items[i].itemName(), '_')[0];
			Item* ptrGrownPlant;
			if(i >0 && plant == items[i-1].itemName())
			{
				ptrGrownPlant = &items[i - 1];
			}
			else
			{
				ptrGrownPlant = (Item*)MyHelperUtils::findInVectorByString(items, plant);
			}
			ptrGrownPlant->price -= items[i].price;
			items[i].price = 0;
		}
	}
}

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

std::string GameObjectContainer::readProcessingDevicesFromConfig() const
{
	std::ifstream cfgFile(FILENAME_PLAYER_CONFIG);
	std::string line;
	std::ostringstream oss;
	while (std::getline(cfgFile, line)) {
		if (MyHelperUtils::stringContains(line, "#") || line == "")
		{
			continue;
		}
		MyHelperUtils::toUpper(line);
		if ((MyHelperUtils::stringContains(line, Item::findName(Devices::CHEM))
			|| MyHelperUtils::stringContains(line, Item::findName(Devices::CRAFTER))
			|| MyHelperUtils::stringContains(line, Item::findName(Devices::GREENHOUSE))
			|| MyHelperUtils::stringContains(line, Item::findName(Devices::JEWELCRAFTER))
			|| MyHelperUtils::stringContains(line, Item::findName(Devices::SMELTER)))
			&& !MyHelperUtils::stringContains(line, "NUMBER"))
		{
			oss << line << std::endl;
		}
		//bool whiteSpacesOnly = std::all_of(line.begin(), line.end(), isspace);
		//if (whiteSpacesOnly) { continue; }
	}
	return oss.str();
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

int GameObjectContainer::getChemMineNumber() const
{
	return chem_mine_number;
}

double GameObjectContainer::getOilRate() const
{
	return oilRate;
}

