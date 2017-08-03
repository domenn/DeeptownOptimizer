#include "Mine.h"

Mine::Mine(char pSpeed, char pLevel):
	speed(pSpeed),
	level(pLevel)
{
}

void Mine::setHeight(short h)
{
	height = h;
}

short Mine::getHeight()
{
	return height;
}

Mine::~Mine()
{
}
