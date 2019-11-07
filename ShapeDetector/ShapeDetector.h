#pragma once
#include "Shape.h"

namespace shapedetector {
	std::tuple<int, int, int> detectCircle(const std::unique_ptr<Shape>& shape);
}
