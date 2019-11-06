#pragma once
#include <list>
#include "Shape.h"


namespace shapedetector
{

	class ShapeManager
	{
		std::list<std::unique_ptr<Shape>> _shapes;

	public:
		explicit ShapeManager(int ac, char** av);

		void ShowShapes();
	};
}
