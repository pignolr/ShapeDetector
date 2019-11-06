#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>
#include <list>
#include <exception>
#include <sstream>

std::list<cv::Mat> loadImageFromArguments(int ac, char **av)
{
	std::stringstream error;
	if (ac < 2) {
		error << "Usage: av[0] img1 [...]" << std::endl;
		throw std::invalid_argument(error.str());
	}
	std::list<cv::Mat> images;
	for (int i = 1; i < ac; ++i) {
		auto image = cv::imread(av[i], cv::IMREAD_COLOR);
		if (!image.data) {
			error << "Error: Could not open or find the image: " << av[i] << std::endl;
		}
		else {
			images.push_back(image);
		}
	}
	if (error.str().length())
		throw std::invalid_argument(error.str());
	return images;
}

void detectShapeImages(std::list<cv::Mat> images)
{
	cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE);
	for (auto& image : images) {
		cv::imshow("Display window", image);

		cv::waitKey(0);
	}
}

int main(int ac, char **av)
{
	try
	{
		auto images = loadImageFromArguments(ac, av);
		detectShapeImages(images);
	}
	catch (std::invalid_argument & e)
	{
		std::cout << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
