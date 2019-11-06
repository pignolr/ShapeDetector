#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>
#include <vector>

namespace shapedetector
{
	class Shape
	{
		const std::string _path;
		cv::Mat _image;
		cv::Mat _edge;
		std::vector<std::vector<double>> _gradient;
		std::vector<std::vector<double>> _direction;

		void edgeDetection() noexcept;
		void edgeGrayscale() noexcept;
		void edgeBlur() noexcept;
		void edgeGradient() noexcept;
		void edgeRemoveNonMaxGradient() noexcept;
		void edgeFilter(int low, int high) noexcept;
	public:
		explicit Shape(const std::string &path);

		const cv::Mat& getImage() const;
		const cv::Mat& getEdge() const;
	};
}
