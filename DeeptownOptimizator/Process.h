#pragma once
#include <vector>
#include <tuple>
#include "Item.h"
#include "Processor.h"
#include <iostream>


class Process
{
public:
	Process(Process & othr) :inputs(othr.inputs),
		outputs(othr.outputs),
		timeSeconds(othr.timeSeconds),
		processor(othr.processor)
	{
		std::cout << "CopyCtr" << std::endl;
	}
	const std::vector<std::tuple<int,Item*>> inputs;
	const std::vector<std::tuple<int, Item*>> outputs;
	const int timeSeconds;
	const Processor * const processor;
	Process(std::vector<std::tuple<int, Item*>> inputs, std::vector<std::tuple<int, Item*>> outputs, int timeSeconds, Processor* processor);
	~Process();
};

