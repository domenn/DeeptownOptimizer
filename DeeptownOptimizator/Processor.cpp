#include "Processor.h"
#include "GameObjectContainer.h"

Processor::Processor(std::string &name, int numberOfThem):
	NamedGameThing(name), numberOfThem(numberOfThem), uniqueIndex(GameObjectContainer::generateUniqueIndex())
{
}
