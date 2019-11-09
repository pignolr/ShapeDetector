#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "ShapeManager.h"

int main()
{
	try
	{
		std::list<std::string> imagesPath = {
			"c:/input/1.jpg",
			"c:/input/2.jpg",
			"c:/input/3.jpg",
			"c:/input/4.jpg",
		};
		shapedetector::ShapeManager shapeManager(imagesPath);
		shapeManager.ShowShapes();
	}
	catch (std::invalid_argument & e)
	{
		std::cout << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

