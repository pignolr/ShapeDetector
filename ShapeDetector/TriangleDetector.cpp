#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

#include "Shape.h"

namespace shapedetector {


	int findLeftEdge(const cv::Mat &image)
	{
		cv::Vec3b color;
		uchar blue;
		uchar green;
		uchar red;

		for (uint x = 0; x < image.size().width; x++) {
			for (uint y = 0; y < image.size().height; y++) {
				color = image.at<cv::Vec3b>(cv::Point(x, y));
				blue = color.val[0];
				green = color.val[1];
				red = color.val[2];

				if (blue == 255 && green == 255 && red == 255) // white border found
					return x;
			}
		}
		return -1;
	}

	int findRightEdge(const cv::Mat &image)
	{
		cv::Vec3b color;
		uchar blue;
		uchar green;
		uchar red;

		for (uint x = image.size().width - 1; x > 0; x--) {
			for (uint y = 0; y < image.size().height; y++) {
				color = image.at<cv::Vec3b>(cv::Point(x, y));
				blue = color.val[0];
				green = color.val[1];
				red = color.val[2];

				if (blue == 255 && green == 255 && red == 255)
					return x;
			}
		}
		return -1;
	}

	int findTopEdge(const cv::Mat &image)
	{
		cv::Vec3b color;
		uchar blue;
		uchar green;
		uchar red;

		for (uint y = 0; y < image.size().height; y++) {
			for (uint x = 0; x < image.size().width; x++) {
				color = image.at<cv::Vec3b>(cv::Point(x, y));
				blue = color.val[0];
				green = color.val[1];
				red = color.val[2];

				if (blue == 255 && green == 255 && red == 255)
					return y;
			}
		}
		return -1;
	}

	int findBottomEdge(const cv::Mat &image)
	{
		cv::Vec3b color;
		uchar blue;
		uchar green;
		uchar red;

		for (uint y = image.size().height - 1; y > 0; y--) {
			for (uint x = 0; x < image.size().width - 1; x++) {
				color = image.at<cv::Vec3b>(cv::Point(x, y));
				blue = color.val[0];
				green = color.val[1];
				red = color.val[2];

				if (blue == 255 && green == 255 && red == 255)
					return (int)y;
			}
		}
		return -1;
	}

	std::vector<std::pair<int, int>> getPixelsTop(const cv::Mat &image, int topEdge)
	{
		std::vector <std::pair <int, int>> pxls;
		cv::Vec3b color;
		uchar blue;
		uchar green;
		uchar red;

		for (int y = topEdge; y < topEdge + 2; y++)
			for (int x = 0; x < image.size().width; x++) {
				color = image.at<cv::Vec3b>(cv::Point(x, y));
				blue = color.val[0];
				green = color.val[1];
				red = color.val[2];

				if (blue == 255 && green == 255 && red == 255)
					pxls.push_back(std::make_pair(x, y));
			}
		return pxls;
	}

	std::vector<std::pair<int, int>> getPixelsBottom(const cv::Mat &image, int bottomEdge)
	{
		std::vector <std::pair <int, int>> pxls;
		cv::Vec3b color;
		uchar blue;
		uchar green;
		uchar red;

		for (int y = bottomEdge; y > bottomEdge - 2; y--)
			for (int x = 0; x < image.size().width; x++) {
				color = image.at<cv::Vec3b>(cv::Point(x, y));
				blue = color.val[0];
				green = color.val[1];
				red = color.val[2];

				if (blue == 255 && green == 255 && red == 255)
					pxls.push_back(std::make_pair(x, y));
			}
		return pxls;
	}

	std::vector<std::pair<int, int>> getPixelsLeft(const cv::Mat &image, int leftEdge)
	{
		std::vector <std::pair <int, int>> pxls;
		cv::Vec3b color;
		uchar blue;
		uchar green;
		uchar red;

		for (int x = leftEdge; x < leftEdge + 2; x++)
			for (int y = 0; y < image.size().height; y++) {
				color = image.at<cv::Vec3b>(cv::Point(x, y));
				blue = color.val[0];
				green = color.val[1];
				red = color.val[2];

				if (blue == 255 && green == 255 && red == 255)
					pxls.push_back(std::make_pair(x, y));
			}
		return pxls;
	}

	std::vector<std::pair<int, int>> getPixelsRight(const cv::Mat &image, int rightEdge)
	{
		std::vector <std::pair <int, int>> pxls;
		cv::Vec3b color;
		uchar blue;
		uchar green;
		uchar red;
		
		for (int x = rightEdge - 1; x < rightEdge + 1; x++)
			for (int y = 0; y < image.size().height; y++) {
				color = image.at<cv::Vec3b>(cv::Point(x, y));
				blue = color.val[0];
				green = color.val[1];
				red = color.val[2];

				if (blue == 255 && green == 255 && red == 255)
					pxls.push_back(std::make_pair(x, y));
			}
		return pxls;
	}

	std::pair<int, int> checkCommon(std::vector<std::pair<int, int>> &first, std::vector<std::pair<int, int>> &second)
	{
		for (const auto& p : first)
			for (const auto& q : second)
				if (p.first >= q.first - 10 && p.first <= q.first + 10 && p.second >= q.second - 10 && p.second <= q.second + 10)
					return p;
		return std::make_pair(-1, -1);
	}

	std::vector < std::pair<int, int>> findTwoPoints(std::vector<std::pair<int, int>> &topPxls, std::vector<std::pair<int, int>> &bottomPxls, std::vector<std::pair<int, int>> &leftPxls, std::vector<std::pair<int, int>> &rightPxls)
	{
		std::vector<std::pair<int, int>> trianglePoints;
		std::pair<int, int> point;

		point = checkCommon(topPxls, rightPxls);
		if (point.first != -1 && point.second != -1 && trianglePoints.size() < 2)
			trianglePoints.push_back(std::make_pair(rightPxls[0].first, topPxls[0].second));
		point = checkCommon(rightPxls, bottomPxls);
		if (point.first != -1 && point.second != -1 && trianglePoints.size() < 2)
			trianglePoints.push_back(std::make_pair(rightPxls[0].first, bottomPxls[0].second));
		point = checkCommon(bottomPxls, leftPxls);
		if (point.first != -1 && point.second != -1 && trianglePoints.size() < 2)
			trianglePoints.push_back(std::make_pair(leftPxls[0].first, bottomPxls[0].second));
		point = checkCommon(leftPxls, topPxls);
		if (point.first != -1 && point.second != -1 && trianglePoints.size() < 2)
			trianglePoints.push_back(std::make_pair(leftPxls[0].first, topPxls[0].second));

		return trianglePoints;
	}

	bool addToY(std::vector<std::pair<int, int>> &allXY, int y, const cv::Mat &image)
	{
		cv::Vec3b color;

		for (const auto& p : allXY)
			if (p.first >= 0 && p.first < image.size().width && p.second + y >= 0 && p.second + y < image.size().height) {
				color = image.at<cv::Vec3b>(cv::Point(p.first, p.second + y));
				if (color.val[0] == 255 && color.val[1] == 255 && color.val[2] == 255)
					return true; // STILL FOUND WHITE
			}
		return false;
	}

	bool addToX(std::vector<std::pair<int, int>> &allXY, int x, const cv::Mat &image)
	{
		cv::Vec3b color;

		for (const auto& p : allXY)
			if (p.first + x >= 0 && p.first + x < image.size().width && p.second >= 0 && p.second < image.size().height) {
				color = image.at<cv::Vec3b>(cv::Point(p.first + x, p.second));
				if (color.val[0] == 255 && color.val[1] == 255 && color.val[2] == 255)
					return true; // STILL FOUND WHITE
			}
		return false;
	}

	std::pair<int, int> getThirdPointY(int y, std::vector<std::pair<int, int>>& allXY, const cv::Mat& image)
	{
		cv::Vec3b color;

		for (const auto& p : allXY)
			if (p.first >= 0 && p.first < image.size().width && p.second + y >= 0 && p.second + y < image.size().height) {
				color = image.at<cv::Vec3b>(cv::Point(p.first, p.second + y));
				if (color.val[0] == 255 && color.val[1] == 255 && color.val[2] == 255)
					return std::make_pair(p.first, p.second + y);
			}
	}

	std::pair<int, int> getThirdPointX(int x, std::vector<std::pair<int, int>> &allXY, const cv::Mat& image)
	{
		cv::Vec3b color;

		for (const auto& p : allXY)
			if (p.first + x >= 0 && p.first + x < image.size().width && p.second >= 0 && p.second < image.size().height) {
				color = image.at<cv::Vec3b>(cv::Point(p.first + x, p.second));
				if (color.val[0] == 255 && color.val[1] == 255 && color.val[2] == 255)
					return std::make_pair(p.first + x, p.second);
			}
	}

	std::pair<int, int> findThirdPoint(std::vector<std::pair<int, int>> &twoTrianglePoints, const cv::Mat &edges)
	{

		double x1 = twoTrianglePoints[0].first;
		double y1 = twoTrianglePoints[0].second;
		double x2 = twoTrianglePoints[1].first;
		double y2 = twoTrianglePoints[1].second;

		double coef = (y2 - y1) / (x2 - x1);
		double k = y1 - (coef * x1);

		int y;
		int x;

		std::vector<std::pair<int, int>> allXY;

		for (int x = -edges.size().width; x < 2 * edges.size().width; x++) {
			y = x * coef + k;
			allXY.push_back(std::make_pair(x, y));
		}

		if (coef <= 1) { // moving y
			y = 1;
			while (addToY(allXY, y, edges)) { // looking up
				y++;
			}

			if (y < 5) { // searched in wrong direction
				y = -1;
				while (addToY(allXY, y, edges)) // looking down
					y--;
				if (y > -5)
					return std::make_pair(-1, -1); // did not found any thrid point
				
				return getThirdPointY(y + 1, allXY, edges);
			}
			return getThirdPointY(y - 1, allXY, edges);
		}
		else {
			x = 1;
			while (addToX(allXY, x, edges)) { // looking right
				x++;
			}

			if (x < 5) { // searched in wrong direction
				x = -1;
				while (addToX(allXY, x, edges)) // looking left
					x--;
				if (x > -5)
					return std::make_pair(-1, -1); // did not found any 3rd point
				return getThirdPointX(x + 1, allXY, edges);
			}
			return getThirdPointX(x - 1, allXY, edges);
		}

	}

	int getYFromXInLine(double realX, double realY, double x1, double y1, double x2, double y2)
	{
		if ((x2 - x1 <= 2 && x2 - x1 >= -2) || (y2 - y1 <= 2 && y2 - y1 >= -2))
			return realY;
		double coef = (y2 - y1) / (x2 - x1);
		double k = y1 - (coef * x1);
		return realX * coef + k;
	}

	std::tuple <std::pair<int, int>, std::pair<int, int>, std::pair<int, int>> detectTriangleWith3Point(
		const std::set<std::pair<int, int>> &edgePoint,
		std::pair<int, int> a,
		std::pair<int, int> b,
		std::pair<int, int> c)
	{
		int acc = 0;
		double treshold = 0.8;

		int possibleY1;
		int possibleY2;
		int possibleY3;
		for (const auto& point : edgePoint) {
			possibleY1 = getYFromXInLine(point.first, point.second, a.first, a.second, b.first, b.second);
			if (possibleY1 > point.second - 10 && possibleY1 < point.second + 10) {
				++acc;
				continue;
			}

			possibleY2 = getYFromXInLine(point.first, point.second, a.first, a.second, c.first, c.second);
			if (possibleY2 > point.second - 10 && possibleY2 < point.second + 10) {
				++acc;
				continue;
			}

			possibleY3 = getYFromXInLine(point.first, point.second, b.first, b.second, c.first, c.second);
			if (possibleY3 > point.second - 10 && possibleY3 < point.second + 10) {
				++acc;
				continue;
			}
			else {
				continue;
			}
		}

		if (acc > edgePoint.size() * treshold)
			return std::make_tuple(a, b, c);
		else
			return std::make_tuple(std::make_pair(-1,-1), std::make_pair(-1,-1), std::make_pair(-1,-1));
	}

	std::tuple <std::pair<int, int>, std::pair<int, int>, std::pair<int, int>> detectTriangle(const std::unique_ptr<Shape>& shape) {

		auto &image = shape->getImage();
		auto &edges = shape->getEdge();
		auto &edgePoint = shape->getEdgePoint();

		int leftEdge = findLeftEdge(edges);
		int rightEdge = findRightEdge(edges);
		int topEdge = findTopEdge(edges);
		int bottomEdge = findBottomEdge(edges);

		if (leftEdge == -1 || rightEdge == -1 || topEdge == -1 || bottomEdge == -1 || leftEdge == rightEdge || topEdge == bottomEdge)
			return std::make_tuple(std::make_pair(-1, -1), std::make_pair(-1, -1), std::make_pair(-1, -1));

		auto topPxls = getPixelsTop(edges, topEdge);
		auto bottomPxls = getPixelsBottom(edges, bottomEdge);
		auto leftPxls = getPixelsLeft(edges, leftEdge);
		auto rightPxls = getPixelsRight(edges, rightEdge);

		if (!topPxls.empty() && !bottomPxls.empty() && !leftPxls.empty() && !rightPxls.empty()) {
			std::vector<std::pair<int, int>> twoTrianglePoints = findTwoPoints(topPxls, bottomPxls, leftPxls, rightPxls);
			if (twoTrianglePoints.size() < 2)
				return std::make_tuple(std::make_pair(-1, -1), std::make_pair(-1, -1), std::make_pair(-1, -1));

		/*	if (!confirmTwoPointValidity(twoTrianglePoints, edges))
				return std::make_tuple(std::make_pair(-1, -1), std::make_pair(-1, -1), std::make_pair(-1, -1));
				*/
			std::pair<int, int> thirdPoint = findThirdPoint(twoTrianglePoints, edges);
			if (thirdPoint.first == -1)
				return std::make_tuple(std::make_pair(-1, -1), std::make_pair(-1, -1), std::make_pair(-1, -1));
			else
				return detectTriangleWith3Point(edgePoint, twoTrianglePoints[0], twoTrianglePoints[1], thirdPoint);
		}
		else
			return std::make_tuple(std::make_pair(-1, -1), std::make_pair(-1, -1), std::make_pair(-1, -1));

		return std::make_tuple(std::make_pair(-1, -1), std::make_pair(-1, -1), std::make_pair(-1, -1));
	}

}