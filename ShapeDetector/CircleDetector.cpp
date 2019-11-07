#include <cmath> 
#include <map>
#include <iostream>
#include "ShapeDetector.h"

namespace shapedetector {
	std::set<std::tuple<int, int, int>> getPoints(const std::set<std::pair<int, int>> edgePoint, int steps)
	{
		std::set<std::tuple<int, int, int>> points;
		auto rtmp = (edgePoint.rbegin()->first - edgePoint.begin()->first) / 2;
		int rmin = rtmp - 1;
		int rmax = rtmp + 1;
		for (int r = rmin; r < rmax + 1; ++r)
			for (int t = 0; t < steps; ++t)
				points.insert(std::tuple<int, int, int>(
					r,
					static_cast<int>(std::round(r * cos(2 * M_PIl * t / steps))),
					static_cast<int>(std::round(r * sin(2 * M_PIl * t / steps)))
					));
		return points;
	}

	std::tuple<int, int, int> detectCircle(const std::unique_ptr<Shape>& shape)
	{
		int steps = 100;
		double treshold = 0.4;
		const auto &edgePoint = shape->getEdgePoint();
		auto points = getPoints(edgePoint, steps);

		std::map<std::tuple<int, int, int>, int> acc;
		for (const auto &it: edgePoint)
			for (const auto& point : points) {
				int a = it.first - std::get<1>(point);
				int b = it.second - std::get<2>(point);
				if (++acc[std::tuple<int, int, int>(a, b, std::get<0>(point))] > steps* treshold) {
					// Circle detected
					return std::tuple<int, int, int>(a, b, std::get<0>(point));
				}
			}
		// Circle not detected
		return std::tuple<int, int, int>(-1, -1, -1);
	}
}

