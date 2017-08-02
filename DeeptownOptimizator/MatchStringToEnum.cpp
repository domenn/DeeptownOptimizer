#include "MatchStringToEnum.h"

MatchStringToEnum::MatchStringToEnum(const std::string& itm) : itm_(itm) {}

bool MatchStringToEnum::operator()(const Item & obj) const
{
	std::string enumstring;
	std::ostringstream stream;
	operator<<(stream, *obj.itemName());
	return stream.str() == itm_;
}

MatchStringToEnum::~MatchStringToEnum()
{

}
