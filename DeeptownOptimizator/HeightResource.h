#pragma once

class Item;
struct HeightResource
{
	const int numberPercentage;
	const Item* const item;
	HeightResource(int cNumberP, const Item* const cItem) :numberPercentage(cNumberP), item(cItem) {}
	double numberPercentageFraction() const { return numberPercentage * 0.01; }
};