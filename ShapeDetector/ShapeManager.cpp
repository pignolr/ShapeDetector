#include <iostream>
#include "ShapeManager.h"

namespace shapedetector
{
	ShapeManager::ShapeManager(int ac, char** av)
	{
		std::list<cv::Mat> images;
		for (int i = 1; i < ac; ++i) {
			try {
				_shapes.push_back(std::unique_ptr<Shape>(new Shape(av[i])));
			}
			catch (std::invalid_argument &e) {
				std::cout << "Error: Could not open or find the image: " << av[i] << std::endl;
			}
		}
		if (!_shapes.size())
			throw std::invalid_argument("Error: No valid image found");
	}

	void ShapeManager::ShowShapes()
	{
		cv::namedWindow("Shape Source", cv::WINDOW_AUTOSIZE);
		cv::namedWindow("Shape Detector", cv::WINDOW_AUTOSIZE);
		for (auto& shape : _shapes) {
			cv::imshow("Shape Source", shape->getImage());
			cv::imshow("Shape Detector", shape->getEdge());
			cv::waitKey(0);
		}
	}
}
