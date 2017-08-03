#pragma once
#include "ItemNames.h"
#include "NamedGameThing.h"
class Item : public NamedGameThing
{
private:
	int price;
public:
	
	Item(std::string &name, int price);
	~Item();
};

