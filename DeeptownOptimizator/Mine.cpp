#include "Mine.h"
#include "HeightMap.h"

Mine::Mine(char pSpeed, char pLevel):
	speed(pSpeed),
	level(pLevel)
{
}

short Mine::getHeight()
{
	return mapping->getHeight();
}

void Mine::setHeightMapping(HeightMapping * pMapping)
{
	mapping = pMapping;
}

const std::vector<HeightResource>* const Mine::getProductions()
{
	return mapping->getProducingResources();
}

char Mine::getSpeed() const
{
	return speed;
}

double Mine::getSpeedFractional() const
{
	return speed * 0.01;
}

Mine::~Mine()
{
}
