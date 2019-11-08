#pragma once
#include <list>
#include "Shape.h"


namespace shapedetector
{

	class ShapeManager
	{
		std::list<std::unique_ptr<Shape>> _shapes;

		bool detectCircleInShape(const std::unique_ptr<Shape> &shape);
		bool detectEllipseInShape(const std::unique_ptr<Shape>& shape);

	public:
		explicit ShapeManager(const std::list<std::string>& imagesPath);

		void ShowShapes();
	};
}
