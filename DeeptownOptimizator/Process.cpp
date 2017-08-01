#include "Process.h"


Process::Process(std::vector<std::tuple<int, Item>> inputs, std::vector<std::tuple<int, Item>> outputs, int timeSeconds, Processor processor) :inputs(inputs),
	outputs(outputs),
	timeSeconds(timeSeconds),
	processor(processor)
{
}

Process::~Process()
{
}
