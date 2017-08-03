#include "HeightMap.h"
#include "Item.h"


HeightMapping::HeightMapping(int cNumberPercentage, int cHeight, Item* cName):
	numberPercentage(cNumberPercentage),
	height(cHeight),
	item(cName)
{
}

HeightMapping::~HeightMapping()
{
}
