#pragma once
#include "ItemNames.h"
#include "NamedGameThing.h"
#include "MyHelperUtils.h"
class Item : public NamedGameThing
{
public:
	int price;
	Item(std::string &name, int price);
	template <typename T>
	static std::string findName(T enumName);
	~Item();
};

template<typename T>
inline std::string Item::findName(T enumName)
{
	std::ostringstream sstr;
	sstr << enumName;
	auto mstr = sstr.str();
	while (MyHelperUtils::stringContains(mstr, ":")) {
		mstr.erase(0, 1);
	}
	return mstr;
}
