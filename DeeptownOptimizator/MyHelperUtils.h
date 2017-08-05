#pragma once


#include <vector>
#include <string>
#include <sstream>

class MyHelperUtils
{
public:
	static std::vector<std::string> split(std::string str, char sep);
	template <typename T>
	static const void * const findInVectorByString(std::vector<T> &the_vector, std::string to_find);
	static bool stringContains(std::string source, std::string seekThis);
	static void toUpper(std::string & str);
	template<typename T>
	static void read_objects_into(std::istream & itemstream, std::vector<T> & the_vector);
};

template<typename T>
inline const void * const MyHelperUtils::findInVectorByString(std::vector<T> &the_vector, std::string to_find)
{
	T* vec_array = the_vector.data();
	for (int i = 0; i<the_vector.size(); ++i)
	{
		T* itm = vec_array + i;
		if (to_find == *itm->itemName())
		{
			return itm;
		}
	}
	return 0;
}

template<typename T>
inline void MyHelperUtils::read_objects_into(std::istream & itemstream, std::vector<T> & the_vector)
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