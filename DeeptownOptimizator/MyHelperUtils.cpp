#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif


#include "MyHelperUtils.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <chrono>
#include <ctime>
#include <sstream>

std::minstd_rand0 MyHelperUtils::generator(std::chrono::system_clock::now().time_since_epoch().count());

std::vector<std::string> MyHelperUtils::split(std::string str, char sep) {
	char separator[]{ sep,+'\0' };
	char* cstr = const_cast<char*>(str.c_str());
	std::vector<std::string> arr;
	char * current = strtok(cstr, separator);
	while (current != nullptr) {
		arr.emplace_back(current);
		current = strtok(nullptr, separator);
	}
	return arr;
}

bool MyHelperUtils::stringContains(std::string source, std::string seekThis)
{
	if (source.find(seekThis) != std::string::npos) {
		return true;
	}
	return false;
}

void MyHelperUtils::toUpper(std::string &str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::toupper);
}

void MyHelperUtils::errorExit(const char * str)
{
	std::cerr << str << std::endl;
	std::cout << "Press ENTER to continue!\n";
	std::cin.ignore();
	exit(EXIT_FAILURE);
}

void MyHelperUtils::checkFileOpen(std::ifstream &file, const std::string& fileName)
{
	if (!file.is_open()) {
		errorExit(std::string("ERROR: File " + fileName + " was not found or could not be opened!").c_str());
	}
}

std::string MyHelperUtils::currentTime()
{
	std::ostringstream mstr;
	time_t t = time(0);   // get time now
	struct tm * now = localtime(&t);
	mstr << (now->tm_year + 1900) << '-'
		<< (now->tm_mon + 1) << '-'
		<< now->tm_mday << '-'
		<< now->tm_hour << '-'
		<< now->tm_min << '-'
		<< now->tm_sec
		<< std::endl;
	return mstr.str();
}

int MyHelperUtils::randomInt(int from, int toExclusive)
{
	generator.discard(4);
	return generator() % (toExclusive - from) + from;
}
