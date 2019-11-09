#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>
#include <vector>
#include <set>
# define M_PIl          3.141592653589793238462643383279502884L /* pi */

namespace shapedetector
{
	class Shape
	{
		const std::string _path;
		cv::Mat _image;
		cv::Mat _edge;
		std::set<std::pair<int, int>> _edgePoint;
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
		void drawTriangle(int ax, int ay, int bx, int by, int cx, int cy);
		void drawCircle(int x, int y, int r);
		void drawEllipse(int x, int y, int a, int b);

		const cv::Mat& getImage() const;
		const cv::Mat& getEdge() const;
		const std::set<std::pair<int, int>>& getEdgePoint() const;
		const std::string& getPath() const;
	};
}
