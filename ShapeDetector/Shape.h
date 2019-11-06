#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>

namespace shapedetector
{
	class Shape
	{
		const std::string _path;
		cv::Mat _image;
	public:
		explicit Shape(const std::string &path);

		const cv::Mat& getImage() const;
	};
}
