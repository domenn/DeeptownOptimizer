#pragma once
#include "ItemNames.h"
#include "NamedGameThing.h"
class Item : public NamedGameThing
{
private:
	int price;
	int index;
public:
	
	Item(std::string &name, int price);
	~Item();
};

