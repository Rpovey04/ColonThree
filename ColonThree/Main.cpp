#include <iostream>
#include "limSimple/Window.h"
#include "datasetHandling/pathLoader.h"

/*
	DATASET TEST
	Attempting to establish a method for trimming / compressing the images in the dataset such that they can all be handled by the GAN
*/

static void displayFullDataset() {
	std::vector<std::string> filePaths = datasetHandle::getJpgPaths();
	Window catDisplay(":3", 400, 400);
	int index = 0;
	double bound = 0;
	while (!catDisplay.shouldClose() && index < filePaths.size()) {
		catDisplay.displayImage(filePaths[index++]);
		catDisplay.pollOnce();
		if ((float)index / (float)filePaths.size() > bound + 0.1f) {
			bound = (float)index / (float)filePaths.size();
			std::cout << bound * 100 << "%" << std::endl;
		}
	}
	if ((float)index / (float)filePaths.size() > 0.999) { std::cout << "100%" << std::endl; }
}

int main() { 
	// displayFullDataset();
	return 1;
}