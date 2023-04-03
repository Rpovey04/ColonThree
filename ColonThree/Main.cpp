#include <iostream>
#include "limSimple/Window.h"
#include "datasetHandling/pathLoader.h"

/*
	DATASET TEST
	Attempting to establish a method for trimming / compressing the images in the dataset such that they can all be handled by the GAN
*/

int main() {
	std::cout << "../archive" << std::endl;
	std::vector<std::string> paths = getAvailableDirectories("../archive/*");
	std::vector<std::string> paths2;
	int count = 0;
	for (int i = 0; i < paths.size(); i++) {
		std::cout << "\t>" << paths[i] << std::endl;
		paths2 = getAvailableDirectories("../archive/" + paths[i] + "/*");
		for (int j = 0; j < paths2.size(); j++) {
			// std::cout << "\t\t>" << paths2[j] << std::endl;
			if (paths2[j][paths2[j].length() - 1] != 'b') { count += 1; }
			else { std::cout << "\t\t> " << paths2[j] << " not counted" << std::endl; }
		}
	}
	std::cout << ".jpg files: " << count << std::endl;
	return 1;
}