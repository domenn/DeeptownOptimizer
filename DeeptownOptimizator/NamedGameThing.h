#pragma once
#include <string>
class NamedGameThing
{
	static int currentIndex;
	static int generateUniqueIndex();
protected:
	NamedGameThing(std::string &parameter_name);
	const std::string name;
	const int uniqueIndex;

public:
	virtual ~NamedGameThing();

	const std::string& const itemName() const;
	bool equals(NamedGameThing* other);
	bool equals(NamedGameThing& other);
	bool operator==(NamedGameThing &other);
	int getIndex() const;
};

