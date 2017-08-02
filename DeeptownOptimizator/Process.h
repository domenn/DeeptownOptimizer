#pragma once
#include <vector>
#include <tuple>
#include "Item.h"
#include "Processor.h"


class Process
{
	std::vector<std::tuple<int,Item*>> inputs;
	std::vector<std::tuple<int, Item*>> outputs;
	int timeSeconds;
	Processor *processor;
public:
	Process(std::vector<std::tuple<int, Item*>> inputs, std::vector<std::tuple<int, Item*>> outputs, int timeSeconds, Processor* processor);
	~Process();
};

