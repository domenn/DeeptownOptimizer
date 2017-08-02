#pragma once
#include "Item.h"
#include <string>
class MatchStringToEnum
{
public:
	MatchStringToEnum(const std::string& itm);
	bool operator()(const Item& obj) const;
private:
	const std::string& itm_;

public:
	~MatchStringToEnum();
};

