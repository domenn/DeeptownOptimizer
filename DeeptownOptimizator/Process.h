#pragma once
#include <vector>
#include <tuple>
#include "Item.h"
#include "Processor.h"


class Process
{
public:
	const std::vector<std::tuple<int,Item*>> inputs;
	const std::vector<std::tuple<int, Item*>> outputs;
	const int timeSeconds;
	const Processor * const processor;
	Process(std::vector<std::tuple<int, Item*>> inputs, std::vector<std::tuple<int, Item*>> outputs, int timeSeconds, Processor* processor);
	~Process();
};

