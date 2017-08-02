#include "NamedGameThing.h"

NamedGameThing::NamedGameThing(std::string &parameter_name) : name(parameter_name)
{
}

NamedGameThing::~NamedGameThing()
{
}

const std::string * const NamedGameThing::itemName() const
{
	return &name;
}
