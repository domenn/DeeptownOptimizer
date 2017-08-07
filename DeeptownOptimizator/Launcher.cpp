#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "Launcher.h"
#include <vector>
#include "MyHelperUtils.h"
#include "GameObjectContainer.h"
#include "Optimizer.h"
#include <ctime>

Launcher::Launcher(int argc, std::string * argv)
{
	GameObjectContainer gameObject;
	Optimizer optimizer(gameObject);
	optimizer.optimize();
	delete[] argv;
}

Launcher::~Launcher()
{
	
}

int main(int argc, char *argv[]) {
	// inputs: number of chems. Number and levels of mines. Number for all processors. Oil income approx. rate (later). Max depth.
	// For greenhouse things, substract the price of seeds, since it is the only way to get them
	// Looks like line comments will have to be supported
	srand(time(0));
	std::string* args = new std::string[argc];
	for (int i = 0; i < argc; ++i) {
		args[i].assign(argv[i]);
	}
	Launcher(argc, args);
	return 0;
}