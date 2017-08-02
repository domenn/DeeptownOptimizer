#pragma once


#include <vector>
#include <string>
class MyHelperUtils
{
public:
	static std::vector<std::string> split(std::string str, char sep);

	template <typename T>
	static void* findInVectorByString(std::vector<T> &the_vector, std::string to_find);
};

template<typename T>
inline void * MyHelperUtils::findInVectorByString(std::vector<T> &the_vector, std::string to_find)
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