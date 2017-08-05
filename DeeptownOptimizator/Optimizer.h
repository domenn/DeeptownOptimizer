#pragma once

class GameObjectContainer;
class Optimizer
{
	// TODO Array that will be used for deciding stuff. Later if implementation gets multithreaded, each thread needs it's own
	int *mineDistribution;
	GameObjectContainer & gameObject;
	
	void calculateMoney();
	void generateRandomSetup();

public:
	Optimizer(GameObjectContainer & pGameObject);
	~Optimizer();
};

