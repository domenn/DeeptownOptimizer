#include "NamedGameThing.h"
#include "GameObjectContainer.h"

int NamedGameThing::currentIndex = -1;

NamedGameThing::NamedGameThing(std::string &parameter_name) : name(parameter_name), uniqueIndex(generateUniqueIndex())
{
}

NamedGameThing::~NamedGameThing()
{
}

const std::string & const NamedGameThing::itemName() const
{
	return name;
}

bool NamedGameThing::equals(NamedGameThing * other)
{
	return uniqueIndex == other->uniqueIndex;
}

bool NamedGameThing::equals(NamedGameThing & other)
{
	return uniqueIndex == other.uniqueIndex;
}

bool NamedGameThing::operator==(NamedGameThing & other)
{
	return equals(other);
}

int NamedGameThing::getIndex() const
{
	return uniqueIndex;
}

int NamedGameThing::generateUniqueIndex()
{
	return ++currentIndex;
}