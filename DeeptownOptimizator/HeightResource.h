#pragma once

class Item;
struct HeightResource
{
	const double numberPercentage;
	const Item* const item;
	HeightResource(double cNumberP, const Item* const cItem) :numberPercentage(cNumberP), item(cItem) {}
	double numberPercentageFraction() const { return numberPercentage * 0.01; }
};