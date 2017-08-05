#pragma once
#include "NamedGameThing.h"
#include <vector>
#include "HeightResource.h"


// My design is so we can have multiple mappings for single height. ? numbers get ignored.
class Item;
class HeightMapping
{
	const short height;
	std::vector<HeightResource> resources;

public:
	HeightMapping(short cHeight);
	void addResource(const Item* const item, const int numberPercentage);
	short getHeight();
	const std::vector<HeightResource>* const getProducingResources();

	~HeightMapping();
};
