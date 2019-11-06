#include "Shape.h"

namespace shapedetector
{
	Shape::Shape(const std::string& path)
		: _path(path), _image(cv::imread(path, cv::IMREAD_COLOR))
	{
		if (!_image.data) {
			std::stringstream ss;
			ss << "Error: Could not open or find the image: "  << _path;
			throw std::invalid_argument(ss.str());
		}
	}

	const cv::Mat &Shape::getImage() const
	{
		return _image;
	}
}
