#pragma once

class GameObjectContainer;
class Optimizer
{
	int *mineDistribution;
	GameObjectContainer & gameObject;
public:
	Optimizer(GameObjectContainer & pGameObject);
	~Optimizer();
};

