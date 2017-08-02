#pragma once
#include <vector>
#include "Process.h"
class GameObjectContainer
{
	static int currentIndex;

	GameObjectContainer();
	std::vector<Process> processes;
	std::vector<Item> items;
	std::vector<Processor> processors;
	std::vector<std::tuple<int, Item*>> get_inputs_outputs(std::string line_split);
	
	template<typename T>
	void read_objects_into(std::istream & itemstream, std::vector<T> & the_vector);
public:
	static GameObjectContainer createDefaultGameObjectContainer();
	static int generateUniqueIndex();
	~GameObjectContainer();
};

template<typename T>
inline void GameObjectContainer::read_objects_into(std::istream & itemstream, std::vector<T> & the_vector)
{
	std::string line;
	while (std::getline(itemstream, line)) {
		std::istringstream item_line_stream(line);
		std::string item_name;
		int price;
		item_line_stream >> item_name >> price;
		the_vector.emplace_back(item_name, price);
	}
}
