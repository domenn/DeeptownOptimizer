#pragma once
#include "NamedGameThing.h"

class Processor : public NamedGameThing {
private:
	int numberOfThem;
	int uniqueIndex;

public:
	Processor(std::string &name, int numberOfThem);
	/*CRAFTER,
	SMELTER,
	JEWELCRAFTER,
	GREENHOUSE,
	CHEM*/
};