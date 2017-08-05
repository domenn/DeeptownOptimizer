#pragma once
#include <vector>
#include "HeightResource.h"

class HeightMapping;

class Mine
{
	const char speed;
	const char level;
	HeightMapping *mapping;

public:
	Mine(char pSpeed, char pLevel);
	short getHeight();
	void setHeightMapping(HeightMapping * pMapping);
	const std::vector<HeightResource>* const getProductions();
	char getSpeed() const;
	double getSpeedFractional() const;
	~Mine();
};

