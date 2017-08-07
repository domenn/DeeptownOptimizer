#pragma once
#include "NamedGameThing.h"

class Processor : public NamedGameThing {
public:
	const int numberOfThem;
	Processor(std::string &name, int numberOfThem);
	/*CRAFTER,
	SMELTER,
	JEWELCRAFTER,
	GREENHOUSE,
	CHEM*/
};