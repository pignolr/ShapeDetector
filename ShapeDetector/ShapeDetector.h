#pragma once
#include "Shape.h"

namespace shapedetector {
	std::tuple<int, int, int> detectCircle(const std::unique_ptr<Shape>& shape);
	std::tuple<int, int, int, int> detectEllipse(const std::unique_ptr<Shape>& shape);
	std::tuple <std::pair<int, int>, std::pair<int, int>, std::pair<int, int>> detectTriangle(const std::unique_ptr<Shape>& shape);
}
