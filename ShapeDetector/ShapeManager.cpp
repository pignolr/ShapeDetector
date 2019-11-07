#include <iostream>
#include "ShapeManager.h"
#include "ShapeDetector.h"

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

	bool ShapeManager::detectCircleInShape(const std::unique_ptr<Shape>& shape)
	{
		std::cout << "Circle Detection..." << std::endl;
		auto circlePos = detectCircle(shape);
		if (std::get<0>(circlePos) == -1 && std::get<1>(circlePos) == -1 && std::get<2>(circlePos) == -1) {
			std::cout << "Not a circle" << std::endl;
			return false;
		}
		else {
			std::cout << "It's a circle: "
				<< "{ x:" << std::get<0>(circlePos)
				<< ", y:" << std::get<1>(circlePos)
				<< ", r:" << std::get<2>(circlePos)
				<< " }" << std::endl;
			// draw result circcle
			shape->drawCircle(std::get<0>(circlePos), std::get<1>(circlePos), std::get<2>(circlePos));
			cv::imshow("Shape Source", shape->getImage());
			return true;
		}
	}

	void ShapeManager::ShowShapes()
	{
		cv::namedWindow("Shape Source", cv::WINDOW_AUTOSIZE);
		cv::namedWindow("Shape Detector", cv::WINDOW_AUTOSIZE);
		for (auto& shape : _shapes) {
			std::cout << "Process \"" << shape->getPath() << "\"..." << std::endl;
			cv::imshow("Shape Source", shape->getImage());
			cv::imshow("Shape Detector", shape->getEdge());

			detectCircleInShape(shape);

			std::cout << "Press Key for next Shape..." << std::endl << std::endl;
			cv::waitKey(0);
		}
	}
}
