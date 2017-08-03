#pragma once
#include "NamedGameThing.h"


// My design is so we can have multiple mappings for single height. ? numbers get ignored.
class Item;
class HeightMapping
{
	const int height;
	const int numberPercentage;
	const Item* const item;

public:
	HeightMapping(int cNumberPercentage, int cHeight, Item* cName);
	~HeightMapping();
};

