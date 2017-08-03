#include "Item.h"


Item::Item(std::string &name, int price):NamedGameThing(name), price(price)
{
}

Item::~Item()
{
}
