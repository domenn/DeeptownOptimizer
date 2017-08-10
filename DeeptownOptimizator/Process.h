#pragma once
#include <vector>
#include <tuple>
#include "Item.h"
#include "Processor.h"
#include <iostream>


class Process
{
	static int currentPriorityNumber;
public:
	/*Process(Process & othr) :inputs(othr.inputs),
		outputs(othr.outputs),
		timeSeconds(othr.timeSeconds),
		processor(othr.processor)
	{
		std::cout << "CopyCtr" << std::endl;
	}*/
	const std::vector<std::tuple<int,Item*>> inputs;
	const std::vector<std::tuple<int, Item*>> outputs;
	const int timeSeconds;
	const Processor * const processor;
	const std::vector<double> itemRates;
	const std::vector<int> itemIndices;
	int priorityNumber;
	
	//bool operator>(Process& othr) const;
	std::vector<double> calculateRates() const;
	std::vector<int> calculateIndices() const;
	Process(std::vector<std::tuple<int, Item*>> inputs, std::vector<std::tuple<int, Item*>> outputs, int timeSeconds, Processor* processor);
	~Process();
};

