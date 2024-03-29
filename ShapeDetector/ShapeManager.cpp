#include <iostream>
#include "ShapeManager.h"
#include "ShapeDetector.h"

namespace shapedetector
{
	ShapeManager::ShapeManager(const std::list<std::string>& imagesPath)
	{
		for (const auto &path: imagesPath) {
			try {
				_shapes.push_back(std::unique_ptr<Shape>(new Shape(path)));
			}
			catch (std::invalid_argument &e) {
				std::cout << "Error: Could not open or find the image: " << path << std::endl;
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
#include <chrono>
	bool ShapeManager::detectEllipseInShape(const std::unique_ptr<Shape>& shape)
	{
		std::cout << "Ellipse Detection..." << std::endl;
		auto ellipsePos = detectEllipse(shape);
		if (std::get<0>(ellipsePos) == -1 && std::get<1>(ellipsePos) == -1
			&& std::get<2>(ellipsePos) == -1 && std::get<3>(ellipsePos) == -1) {
			std::cout << "Not an ellipse" << std::endl;
			return false;
		}
		else {
			std::cout << "It's a ellipse: "
				<< "{ x:" << std::get<0>(ellipsePos)
				<< ", y:" << std::get<1>(ellipsePos)
				<< ", a:" << std::get<2>(ellipsePos)
				<< ", b:" << std::get<3>(ellipsePos)
				<< " }" << std::endl;
			// draw result circcle
			shape->drawEllipse(std::get<0>(ellipsePos), std::get<1>(ellipsePos), std::get<2>(ellipsePos), std::get<3>(ellipsePos));
			cv::imshow("Shape Source", shape->getImage());
			return true;
		}
	}

	bool ShapeManager::detectTriangleInShape(const std::unique_ptr<Shape>& shape)
	{
		std::cout << "Triangle Detection..." << std::endl;
		auto trianglePos = detectTriangle(shape);
		if (std::get<0>(trianglePos).first == -1 && std::get<0>(trianglePos).second == -1) {
			std::cout << "Not a triangle" << std::endl;
			return false;
		}
		else {
			std::cout << "It's a triangle: "
				<< "{ A(" << std::get<0>(trianglePos).first
				<< ", " << std::get<0>(trianglePos).second
				<< "), B(" << std::get<1>(trianglePos).first
				<< ", " << std::get<1>(trianglePos).second
				<< "), C(" << std::get<2>(trianglePos).first
				<< ", " << std::get<2>(trianglePos).second
				<< ") }" << std::endl;
			// draw result triangle
			shape->drawTriangle(
				std::get<0>(trianglePos).first, std::get<0>(trianglePos).second,
				std::get<1>(trianglePos).first, std::get<1>(trianglePos).second,
				std::get<2>(trianglePos).first, std::get<2>(trianglePos).second);

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
			cv::waitKey(10);

			bool shapeIsFound = false;
			shapeIsFound = detectTriangleInShape(shape);
			if (!shapeIsFound)
				shapeIsFound = detectCircleInShape(shape);
			if (!shapeIsFound)
				shapeIsFound = detectEllipseInShape(shape);
			std::cout << "Press Key on Image for next Shape..." << std::endl << std::endl;
			cv::waitKey(0);
		}
	}
}
