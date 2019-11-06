#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "ShapeManager.h"

int main(int ac, char **av)
{
	if (ac < 2) {
		std::cout << "Usage: av[0] img1 [...]" << std::endl;
		return EXIT_FAILURE;
	}

	try
	{
		shapedetector::ShapeManager shapeManager(ac, av);
		shapeManager.ShowShapes();
	}
	catch (std::invalid_argument & e)
	{
		std::cout << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
