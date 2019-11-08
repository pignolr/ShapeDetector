#include <cmath> 
#include <map>
#include <iostream>
#include "ShapeDetector.h"

namespace shapedetector {
	static std::set<std::tuple<int, int, int>> getCirclePoints(const std::set<std::pair<int, int>> edgePoint, int steps)
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

	static int getXCenter(const std::set<std::pair<int, int>>& edgePoint)
	{
		return edgePoint.rbegin()->first - ((edgePoint.rbegin()->first - edgePoint.begin()->first) / 2);
	}

	static std::pair<int, int> getYMinMax(const std::set<std::pair<int, int>>& edgePoint)
	{
		std::pair<int, int> res(INT_MAX, INT_MIN);

		for (const auto& it : edgePoint) {
			if (it.second < res.first)
				res.first = it.second;
			if (it.second > res.second)
				res.second = it.second;
		}
		return res;
	}

	static int getYCenter(const std::set<std::pair<int, int>>& edgePoint)
	{
		auto yMinMax = getYMinMax(edgePoint);
		return yMinMax.second - ((yMinMax.second - yMinMax.first) / 2);
	}

	std::tuple<int, int, int> detectCircle(const std::unique_ptr<Shape>& shape)
	{
		int steps = 100;
		double treshold = 0.4;
		const auto &edgePoint = shape->getEdgePoint();
		auto points = getCirclePoints(edgePoint, steps);
		int xCenter = getXCenter(edgePoint);
		int yCenter = getYCenter(edgePoint);

		std::map<std::tuple<int, int, int>, int> acc;
		for (const auto &it: edgePoint)
			for (const auto& point : points) {
				int x = it.first - std::get<1>(point);
				int y = it.second - std::get<2>(point);
				if (x > xCenter - 2 && x < xCenter + 2 && y > yCenter - 2 && y < yCenter + 2) {
					std::tuple<int, int, int> possibleCircle(x, y, std::get<0>(point));
					if (++acc[possibleCircle] > steps* treshold) {
						// Circle detected
						return possibleCircle;
					}
				}
			}
		// Circle not detected
		return std::tuple<int, int, int>(-1, -1, -1);
	}

	static std::set<std::tuple<int, int, int, int>> getEllipsePoints(const std::set<std::pair<int, int>> edgePoint, int steps)
	{
		std::set<std::tuple<int, int, int, int>> points;
		auto aTmp = (edgePoint.rbegin()->first - edgePoint.begin()->first) / 2;
		int aMin = aTmp - 1;
		int aMax = aTmp + 1;

		auto yMinMax = getYMinMax(edgePoint);
		auto bTmp = (yMinMax.second - yMinMax.first) / 2;
		int bMin = bTmp - 1;
		int bMax = bTmp + 1;

		for (int a = aMin; a < aMax + 1; ++a)
			for (int b = bMin; b < bMax + 1; ++b)
				for (int t = 0; t < steps; ++t)
					points.insert(std::tuple<int, int, int, int>(
						a,
						b,
						static_cast<int>(std::round(a * cos(2 * M_PIl * t / steps))),
						static_cast<int>(std::round(b * sin(2 * M_PIl * t / steps)))
						));
		return points;
	}

	std::tuple<int, int, int, int> detectEllipse(const std::unique_ptr<Shape>& shape)
	{
		int steps = 40;
		double treshold = 0.5;
		const auto& edgePoint = shape->getEdgePoint();
		auto points = getEllipsePoints(edgePoint, steps);
		int xCenter = getXCenter(edgePoint);
		int yCenter = getYCenter(edgePoint);

		std::map<std::tuple<int, int, int, int>, int> acc;
		for (const auto& it : edgePoint)
			for (const auto& point : points) {
				int x = it.first - std::get<2>(point);
				int y = it.second - std::get<3>(point);

				if (x > xCenter - 2 && x < xCenter + 2 && y > yCenter - 2 && y < yCenter + 2) {
					std::tuple<int, int, int, int> possibleEllipse(x, y, std::get<0>(point), std::get<1>(point));
					if (++acc[possibleEllipse] > steps* treshold) {
						// Ellipse detected
						return possibleEllipse;
					}
				}
			}
		// Ellipse not detected
		return std::tuple<int, int, int, int>(-1, -1, -1, -1);
	}
}
