#include "HeightMap.h"
#include "Item.h"


HeightMapping::HeightMapping(short cHeight):
	height(cHeight)
{
}

void HeightMapping::addResource(const Item * const item, const double numberPercentage)
{
	// TODO emplace
	// resources.emplace_back(numberPercentage, item);
	resources.push_back(HeightResource(numberPercentage, item));
}

HeightMapping::~HeightMapping()
{
}

short HeightMapping::getHeight()
{
	return height;
}

const std::vector<HeightResource>* const HeightMapping::getProducingResources()
{
	return &resources;
}
