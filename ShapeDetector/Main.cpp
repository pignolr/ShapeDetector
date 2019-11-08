#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
//#include <cmath>
#include "ShapeManager.h"

int findLeftEdge(cv::Mat image)
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
				
			if (blue == 0 && green == 0 && red == 0) // black found
				return x;
		}
	}
	return -1;
}

int findRightEdge(cv::Mat image)
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

			if (blue == 0 && green == 0 && red == 0)
				return x;
		}
	}
	return -1;
}

int findTopEdge(cv::Mat image)
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

			if (blue == 0 && green == 0 && red == 0)
				return y;
		}
	}
	return -1;
}

int findBottomEdge(cv::Mat image)
{
	cv::Vec3b color;
	uchar blue;
	uchar green;
	uchar red;

	for (uint y = image.size().height -1; y > 0; y--) {
		for (uint x = 0; x < image.size().width - 1; x++) {
			color = image.at<cv::Vec3b>(cv::Point(x, y));
			blue = color.val[0];
			green = color.val[1];
			red = color.val[2];

			if (blue == 0 && green == 0 && red == 0)
				return y;
		}
	}
	return -1;
}

std::vector<std::pair<int, int>> getPixelsTop(cv::Mat image, int topEdge)
{
	std::vector <std::pair <int, int>> pxls;
	cv::Vec3b color;
	uchar blue;
	uchar green;
	uchar red;

	for (int x = 0; x < image.size().width; x++) {
		color = image.at<cv::Vec3b>(cv::Point(x, topEdge));
		blue = color.val[0];
		green = color.val[1];
		red = color.val[2];

		if (blue == 0 && green == 0 && red == 0)
			pxls.push_back(std::make_pair(x, topEdge));
	}

	std::cout << "top edge: " << topEdge << std::endl;
	/*for (const auto& p : pxls)
		std::cout << p.first << ", " << p.second << std::endl;*/

	return pxls;
}

std::vector<std::pair<int, int>> getPixelsBottom(cv::Mat image, int bottomEdge)
{
	std::vector <std::pair <int, int>> pxls;
	cv::Vec3b color;
	uchar blue;
	uchar green;
	uchar red;

	for (int x = 0; x < image.size().width; x++) {
		color = image.at<cv::Vec3b>(cv::Point(x, bottomEdge));
		blue = color.val[0];
		green = color.val[1];
		red = color.val[2];

		if (blue == 0 && green == 0 && red == 0)
			pxls.push_back(std::make_pair(x, bottomEdge));
	}

	std::cout << "bottom edge: " << bottomEdge << std::endl;
/*	for (const auto& p : pxls)
		std::cout << p.first << ", " << p.second << std::endl;*/

	return pxls;
}

std::vector<std::pair<int, int>> getPixelsLeft(cv::Mat image, int leftEdge)
{
	std::vector <std::pair <int, int>> pxls;
	cv::Vec3b color;
	uchar blue;
	uchar green;
	uchar red;

	for (int y = 0; y < image.size().height; y++) {
		color = image.at<cv::Vec3b>(cv::Point(leftEdge, y));
		blue = color.val[0];
		green = color.val[1];
		red = color.val[2];

		if (blue == 0 && green == 0 && red == 0)
			pxls.push_back(std::make_pair(leftEdge, y));
	}
	std::cout << "left edge: " << leftEdge << std::endl;
/*	for (const auto& p : pxls)
		std::cout << p.first << ", " << p.second << std::endl;*/

	return pxls;
}

std::vector<std::pair<int, int>> getPixelsRight(cv::Mat image, int rightEdge)
{
	std::vector <std::pair <int, int>> pxls;
	cv::Vec3b color;
	uchar blue;
	uchar green;
	uchar red;

	for (int y = 0; y < image.size().height; y++) {
		color = image.at<cv::Vec3b>(cv::Point(rightEdge, y));
		blue = color.val[0];
		green = color.val[1];
		red = color.val[2];

		if (blue == 0 && green == 0 && red == 0)
			pxls.push_back(std::make_pair(rightEdge, y));
	}
	std::cout << "right edge: " << rightEdge << std::endl;
	/*for (const auto& p : pxls)
		std::cout << p.first << ", " << p.second << std::endl;*/

	return pxls;
}

std::pair<int, int> checkCommon(std::vector<std::pair<int, int>> first, std::vector<std::pair<int, int>> second)
{
	for (const auto& p : first)
		for (const auto& q : second)
			if (p.first >= q.first - 10 && p.first <= q.first + 10 && p.second >= q.second - 10 && p.second <= q.second + 10)
				return p;
	return std::make_pair(-1, -1);
}

std::vector < std::pair<int, int>> findTwoPoints(std::vector<std::pair<int, int>> topPxls, std::vector<std::pair<int, int>> bottomPxls, std::vector<std::pair<int, int>> leftPxls, std::vector<std::pair<int, int>> rightPxls)
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

void searchThirdPoint(std::vector<std::pair<int, int>> twoTrianglePoints)
{
	double a = std::abs(twoTrianglePoints[1].second - twoTrianglePoints[0].second);
	double b = std::abs(twoTrianglePoints[1].first - twoTrianglePoints[0].first);
	double c = a * twoTrianglePoints[0].first + b * twoTrianglePoints[0].second;

	std::cout << "a: " << a << " b: " << b << " c: " << c << std::endl;
}

bool addToY(std::vector<std::pair<int, int>> allXY, int y, cv::Mat image)
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

bool addToX(std::vector<std::pair<int, int>> allXY, int x, cv::Mat image)
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

int findThirdPoint(std::vector<std::pair<int, int>> twoTrianglePoints, int coef, int k, cv::Mat image)
{
	int y;
	int x;

	std::vector<std::pair<int, int>> allXY;

	for (int x = -image.size().width; x < 2 * image.size().width; x++) {
		y = x * coef + k;
			allXY.push_back(std::make_pair(x, y));
	}

	if (coef <= 1) { // moving y
		y = 1;
		while (addToY(allXY, y, image)) { // looking up
			y++;
		}

		if (y < 5) { // searched in wrong direction
			y = -1;
			while (addToY(allXY, y, image)) // looking down
				y--;
			if (y > -5)
				return 0; // did not found any thrid point
			return y+1;
		}
		return y-1;
	}
	else {
		x = 1;
		while (addToX(allXY, x, image)) { // looking right
			x++;
		}

		if (x < 5) { // searched in wrong direction
			x = -1;
			while (addToX(allXY, x, image)) // looking left
				x--;
			if (x > -5)
				return 0; // did not found any 3rd point
			return x + 1;
		}
		return x - 1;
	}

}

int openImage()
{
	cv::Mat image;
	image = cv::imread("C:/Users/$AG0100-N0MKJK66NU81/Desktop/ShapeDetector/Data/triangle2.jpg", cv::IMREAD_COLOR);

	int leftEdge;
	int rightEdge;
	int topEdge;
	int bottomEdge;

	std::vector<std::pair<int, int>> topPxls;
	std::vector<std::pair<int, int>> bottomPxls;
	std::vector<std::pair<int, int>> leftPxls;
	std::vector<std::pair<int, int>> rightPxls;

	std::vector<std::pair<int, int>> twoTrianglePoints;

	cv::Vec3b color;

	if (image.data) // Check for invalid input
	{
		cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE);// Create a window for display.
		imshow("Display window", image);
		cv::waitKey(0);

		leftEdge = findLeftEdge(image);
		rightEdge = findRightEdge(image);
		topEdge = findTopEdge(image);
		bottomEdge = findBottomEdge(image);

		if (leftEdge == -1 || rightEdge == -1 || topEdge == -1 || bottomEdge == -1 || leftEdge == rightEdge || topEdge == bottomEdge)
			return(-1);

		topPxls = getPixelsTop(image, topEdge);
		bottomPxls = getPixelsBottom(image, bottomEdge);
		leftPxls = getPixelsLeft(image, leftEdge);
		rightPxls = getPixelsRight(image, rightEdge);

		if (!topPxls.empty() && !bottomPxls.empty() && !leftPxls.empty() && !rightPxls.empty()) {
			twoTrianglePoints = findTwoPoints(topPxls, bottomPxls, leftPxls, rightPxls);
			if (twoTrianglePoints.size() < 2)
				return -1;

			searchThirdPoint(twoTrianglePoints);
			std::cout << "first point x = " << twoTrianglePoints[0].first << " y = " << twoTrianglePoints[0].second << std::endl;
			std::cout << "second point x = " << twoTrianglePoints[1].first << " y = " << twoTrianglePoints[1].second << std::endl;

			int x1 = twoTrianglePoints[0].first;
			int y1 = twoTrianglePoints[0].second;
			int x2 = twoTrianglePoints[1].first;
			int y2 = twoTrianglePoints[1].second;

			double coef = (y2 - y1) / (x2 - x1);
			double k = y1 - (coef * x1);
			
			std::cout << "k: " << k << std::endl;

			int compt = 0;
			int correct = 0;
			int y;

			if (twoTrianglePoints[0].first < twoTrianglePoints[1].first) {
				for (int x = twoTrianglePoints[0].first; x <= twoTrianglePoints[1].first; x++) {
					compt++;
					y = x * coef + k;
					color = image.at<cv::Vec3b>(cv::Point(x, y));
					if (color.val[0] == 255 && color.val[1] == 255 && color.val[2] == 255)
						correct++;
				}
			}
			else {
				for (int x = twoTrianglePoints[1].first; x <= twoTrianglePoints[0].first; x++) {
					compt++;
					y = x * coef + k;
					color = image.at<cv::Vec3b>(cv::Point(x, y));
					if (color.val[0] == 255 && color.val[1] == 255 && color.val[2] == 255)
						correct++;
				}
			}
			//if (correct < compt * 0.9)
//				return -1;				//		FAILED FIND LINE
			
			findThirdPoint(twoTrianglePoints, coef, k, image);

			color.val[0] = 0;
			color.val[1] = 0;
			color.val[2] = 0;
			for (int x = 0; x < image.size().width; x++) {
				y = x * coef + k;
				if (y >= 0 && y < image.size().height) {
					std::cout << "x: " << x << " y: " << y << std::endl;
					image.at<cv::Vec3b>(cv::Point(x, y)) = color;
				}
			}
		}
		else
			return -1;
	}
	imshow("Display window", image);
	cv::waitKey(0);
}

int main(int ac, char **av)
{
	openImage();
	/*
	if (ac < 2) {

		std::cout << "Usage: av[0] img1 [...]" << std::endl;
		return EXIT_FAILURE;
	}

	try
	{
		shapedetector::ShapeManager shapeManager(ac, av);
		shapeManager.ShowShapes();
	}
	catch (std::invalid_argument & e)
	{
		std::cout << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
	*/
}

