#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "Launcher.h"
#include <vector>
#include "MyHelperUtils.h"

Launcher::Launcher(int argc, std::string * argv)
{

	delete[] argv;
}

Launcher::~Launcher()
{

}

int main(int argc, char *argv[]) {

	auto splitterTest = MyHelperUtils::split("this:shall_be:splittin", ':');
	std::string* args = new std::string[argc];
	for (int i = 0; i < argc; ++i) {
		args[i].assign(argv[i]);
	}
	Launcher(argc, args);
}