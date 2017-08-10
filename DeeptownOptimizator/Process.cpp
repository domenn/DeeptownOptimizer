#include "Process.h"


int Process::currentPriorityNumber = 1;

std::vector<double> Process::calculateRates() const
{
	std::vector<double> returnVec;
	for (const std::tuple<int, Item*> & output : outputs)
	{
		int howManyItems = std::get<0>(output);
		returnVec.emplace_back(((double)howManyItems / (double)timeSeconds) * 60.0);
	}

	for (const std::tuple<int, Item*> & output : inputs)
	{
		int howManyItems = std::get<0>(output);
		returnVec.emplace_back(((double)howManyItems / (double)timeSeconds) * (-60.0));
	}
	return returnVec;
}

std::vector<int> Process::calculateIndices() const
{
	std::vector<int> returnVec;
	for (const std::tuple<int, Item*> & output : outputs)
	{
		returnVec.emplace_back(std::get<1>(output)->getIndex());
	}

	for (const std::tuple<int, Item*> & output : inputs)
	{
		returnVec.emplace_back(std::get<1>(output)->getIndex());
	}
	return returnVec;
}

Process::Process(std::vector<std::tuple<int, Item*>> inputs, std::vector<std::tuple<int, Item*>> outputs, int timeSeconds, Processor *processor) :inputs(inputs),
	outputs(outputs),
	timeSeconds(timeSeconds),
	processor(processor),
	itemRates(calculateRates()),
	itemIndices(calculateIndices())
{
}

Process::~Process()
{
}
