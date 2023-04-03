#include <iostream>
#include "limSimple/Window.h"

int main() {
	Window myWindow("test", 400, 400);
	while (!myWindow.shouldClose()) { 
		std::cout << "window open " << std::endl;
		myWindow.pollOnce();
	}
	std::cout << "closing" << std::endl;
	return 1;
}