#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif


#include "MyHelperUtils.h"



std::vector<std::string> MyHelperUtils::split (std::string str, char sep) {
	char separator[]{ sep,+'\0' };
	char* cstr = const_cast<char*>(str.c_str());
	char* current;
	std::vector<std::string> arr;
	current = strtok(cstr, separator);
	while (current != NULL) {
		arr.push_back(current);
		current = strtok(NULL, separator);
	}
	return arr;
}