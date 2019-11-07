#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
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
			{
				std::cout << "x = " << x << " | y = " << y << std::endl;
				return x;
			}
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

			if (blue == 0 && green == 0 && red == 0) {
				std::cout << "x = " << x << " | y = " << y << std::endl;
				return x;
			}
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

			if (blue == 0 && green == 0 && red == 0) {
				std::cout << "x = " << x << " | y = " << y << std::endl;
				return y;
			}
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

			if (blue == 0 && green == 0 && red == 0) {
				std::cout << "x = " << x << " | y = " << y << std::endl;
				return y;
			}
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
	for (const auto& p : pxls)
		std::cout << p.first << ", " << p.second << std::endl;

	return pxls;
}

bool checkCommon(std::vector<std::pair<int, int>> first, std::vector<std::pair<int, int>> second)
{
	for (const auto& p : first)
		for (const auto& q : second)
			if (p.first == q.first && p.second == q.second)
				return true;
	return false;
}

int openImage()
{
	cv::Mat image;
	image = cv::imread("C:/Users/$AG0100-N0MKJK66NU81/Desktop/ShapeDetector/Data/Circle.jpg", cv::IMREAD_COLOR);

	int leftEdge;
	int rightEdge;
	int topEdge;
	int bottomEdge;

	std::vector<std::pair<int, int>> topPxls;
	std::vector<std::pair<int, int>> bottomPxls;
	std::vector<std::pair<int, int>> leftPxls;
	std::vector<std::pair<int, int>> rightPxls;

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

		if (!topPxls.empty() && !bottomPxls.empty() && !leftPxls.empty() && !rightPxls.empty())
			if (checkCommon(topPxls, rightPxls) == true || checkCommon(rightPxls, bottomPxls) == true || checkCommon(bottomPxls, leftPxls) == true || checkCommon(leftPxls, topPxls) == true)
			{

			}
		else
			return -1;
	}
}

int main(int ac, char **av)
{
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
}

