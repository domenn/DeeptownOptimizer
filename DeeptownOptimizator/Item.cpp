#include "Item.h"
#include "GameObjectContainer.h"


Item::Item(std::string &name, int price):NamedGameThing(name), price(price), index(GameObjectContainer::generateUniqueIndex())
{
}

Item::~Item()
{
}
