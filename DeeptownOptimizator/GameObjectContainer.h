#pragma once
#include <vector>
#include "Process.h"
class GameObjectContainer
{
	GameObjectContainer();
	std::vector<Process> processes;
public:
	static GameObjectContainer createDefaultGameObjectContainer();
	~GameObjectContainer();
};

