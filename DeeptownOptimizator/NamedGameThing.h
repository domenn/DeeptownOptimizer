#pragma once
#include <string>
class NamedGameThing
{
protected:
	NamedGameThing(std::string &parameter_name);
	const std::string name;
public:
	virtual ~NamedGameThing();

	const std::string* const itemName() const;
};

