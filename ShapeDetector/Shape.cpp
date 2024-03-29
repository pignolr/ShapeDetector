#include <set>
#include <algorithm>
#include <opencv2/imgproc.hpp>
#include "Shape.h"

namespace shapedetector
{
	Shape::Shape(const std::string& path)
		: _path(path),
		_image(cv::imread(path, cv::IMREAD_COLOR)),
		_edge(_image.clone()),
		_edgePoint(),
		_gradient(_edge.rows, std::vector<double>(_edge.cols, 0)),
		_direction(_edge.rows, std::vector<double>(_edge.cols, 0))
	{
		if (!_image.data) {
			std::stringstream ss;
			ss << "Error: Could not open or find the image: "  << _path;
			throw std::invalid_argument(ss.str());
		}
	
		edgeDetection();
	}

	void Shape::edgeGrayscale() noexcept
	{
		uint8_t* pixelPtr = (uint8_t*)_edge.data;
		int cn = _edge.channels();
		cv::Scalar_<uint8_t> bgrPixel;

		for (int y = 0; y < _edge.rows; ++y)
			for (int x = 0; x < _edge.cols; ++x) {
				bgrPixel.val[0] = pixelPtr[y * _edge.cols * cn + x * cn + 0]; // B
				bgrPixel.val[1] = pixelPtr[y * _edge.cols * cn + x * cn + 1]; // G
				bgrPixel.val[2] = pixelPtr[y * _edge.cols * cn + x * cn + 2]; // R

				uint8_t grayscale = .2126 * bgrPixel.val[2] + .7152 * bgrPixel.val[1] + .0722 * bgrPixel.val[0];

				pixelPtr[y * _edge.cols * cn + x * cn + 0] = grayscale; // B
				pixelPtr[y * _edge.cols * cn + x * cn + 1] = grayscale; // G
				pixelPtr[y * _edge.cols * cn + x * cn + 2] = grayscale; // R
			}

	}

	void Shape::edgeBlur() noexcept
	{
		std::vector<std::vector<double>> kernel = {
			{1. / 256., 4. / 256., 6. / 256., 4. / 256., 1. / 256.},
			{4. / 256., 16. / 256., 24. / 256., 16. / 256., 4. / 256.},
			{6. / 256., 24. / 256., 36. / 256., 24. / 256., 6. / 256.},
			{4. / 256., 16. / 256., 24. / 256., 16. / 256., 4. / 256.},
			{1. / 256., 4. / 256., 6. / 256., 4. / 256., 1. / 256.}
		};
	
		size_t offset = kernel.size() / 2;
		auto copy = _edge.clone();
		uint8_t* pixelCopyPtr = (uint8_t*)copy.data;
		uint8_t* pixelPtr = (uint8_t*)_edge.data;
		int cn = _edge.channels();
		cv::Scalar_<uint8_t> bgrPixel;
		for (int y = 0; y < _edge.rows; ++y)
			for (int x = 0; x < _edge.cols; ++x) {
				double acc = 0;
				for (int b = 0; b < kernel.size(); ++b)
					for (int a = 0; a < kernel[b].size(); ++a) {
						int xn = x + a - offset;
						if (xn > _edge.cols - 1)
							xn = _edge.cols - 1;
						if (xn < 0)
							xn = 0;
						int yn = y + b - offset;
						if(yn > _edge.rows - 1)
							yn = _edge.rows - 1;
						if (yn < 0)
							yn = 0;
						acc += static_cast<double>(pixelCopyPtr[yn * _edge.cols * cn + xn * cn]) * kernel[a][b];
					}
				pixelPtr[y * _edge.cols * cn + x * cn + 0] = static_cast<int>(acc); // B
				pixelPtr[y * _edge.cols * cn + x * cn + 1] = static_cast<int>(acc); // G
				pixelPtr[y * _edge.cols * cn + x * cn + 2] = static_cast<int>(acc); // R
			}
	}

	void Shape::edgeGradient() noexcept
	{
		uint8_t* pixelPtr = (uint8_t*)_edge.data;
		int cn = _edge.channels();
		for (int y = 0; y < _gradient.size(); ++y)
			for (int x = 0; x < _gradient[y].size(); ++x) {
				if (0 < x && x < _gradient[y].size() - 1 && 0 < y && y < _gradient.size() - 1) {
					auto val1 = static_cast<double>(pixelPtr[y * _edge.cols * cn + (x + 1) * cn]);
					auto val2 = static_cast<double>(pixelPtr[y * _edge.cols * cn + (x - 1) * cn]);
					double magx = val1 - val2;
					val1 = static_cast<double>(pixelPtr[(y + 1) * _edge.cols * cn + x * cn]);
					val2 = static_cast<double>(pixelPtr[(y - 1) * _edge.cols * cn + x * cn]);
					double magy = val1 - val2;
					_gradient[y][x] = sqrt(magx * magx + magy * magy);
					_direction[y][x] = atan2(magy, magx);
				}
			}
	}

	void Shape::edgeRemoveNonMaxGradient() noexcept
	{
		for (int y = 1; y < _gradient.size() - 1; ++y)
			for (int x = 1; x < _gradient[y].size() - 1; ++x) {
				double angle = _direction[y][x] >= 0 ? _direction[y][x] : _direction[y][x] + M_PIl;
					double rangle = round(angle / (M_PIl / 4));
					double mag = _gradient[y][x];
					if ((rangle == 0 || rangle == 4) && (_gradient[y][x + 1] > mag || _gradient[y][x + 1] > mag)
						|| (rangle == 1 && (_gradient[y - 1][x - 1] > mag || _gradient[y + 1][x + 1] > mag))
						|| (rangle == 2 && (_gradient[y - 1][x] > mag || _gradient[y + 1][x] > mag))
						|| (rangle == 3 && (_gradient[y - 1][x + 1] > mag || _gradient[y + 1][x - 1] > mag)))
						_gradient[y][x] = 0;
			}
	}

	void Shape::edgeFilter(int low, int high) noexcept
	{
		int cn = _edge.channels();
		cv::Scalar_<uint8_t> bgrPixel;
		uint8_t* pixelPtr = (uint8_t*)_edge.data;
		std::set<std::pair<int, int>> iter;
		for (int y = 0; y < _edge.rows; ++y)
			for (int x = 0; x < _edge.cols; ++x) {
				if (_gradient[y][x] > high) {
					pixelPtr[y * _edge.cols * cn + x * cn + 0] = 255;
					pixelPtr[y * _edge.cols * cn + x * cn + 1] = 255;
					pixelPtr[y * _edge.cols * cn + x * cn + 2] = 255;
					iter.insert(std::pair<int, int>(x, y));
					_edgePoint.insert(std::pair<int, int>(x, y));
				}
				else {
					pixelPtr[y * _edge.cols * cn + x * cn + 0] = 0;
					pixelPtr[y * _edge.cols * cn + x * cn + 1] = 0;
					pixelPtr[y * _edge.cols * cn + x * cn + 2] = 0;
				}
			}
		std::vector<std::pair<int, int>> neighbors =
		{ {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1} };
		while (iter.size()) {
			std::set<std::pair<int, int>> tmp;
			for (auto& pos : iter) {
				int x = pos.first;
				int y = pos.second;
				for (auto& neighbor : neighbors) {
					int a = neighbor.first;
					int b = neighbor.second;
					if (!(y + b < 0 || y + b > _gradient.size() || x + a < 0 || x + a > _gradient[y + b].size())
						&& _gradient[y + b][x + a] > low
						&& pixelPtr[(y + b) * _edge.cols * cn + (x + a) * cn + 0] == 0)
						tmp.insert(std::make_pair<int, int>(x + a, y + b));
				}
			}

			for (auto& pos : tmp) {
				int x = pos.first;
				int y = pos.second;
				pixelPtr[y * _edge.cols * cn + x * cn + 0] = 255;
				pixelPtr[y * _edge.cols * cn + x * cn + 1] = 255;
				pixelPtr[y * _edge.cols * cn + x * cn + 2] = 255;
				_edgePoint.insert(std::pair<int, int>(x, y));
			}
			iter = tmp;
		}
	}

	void Shape::edgeDetection() noexcept
	{
		edgeGrayscale();
		edgeGradient();
		edgeRemoveNonMaxGradient();
		edgeFilter(20, 25);
	}

	void Shape::drawTriangle(int ax, int ay, int bx, int by, int cx, int cy)
	{
		cv::line(_image, cv::Point(ax, ay), cv::Point(bx, by), cv::Scalar(255, 0, 255, 255), 2);
		cv::line(_image, cv::Point(ax, ay), cv::Point(cx, cy), cv::Scalar(255, 0, 255, 255), 2);
		cv::line(_image, cv::Point(bx, by), cv::Point(cx, cy), cv::Scalar(255, 0, 255, 255), 2);
	}

	void Shape::drawCircle(int x, int y, int r)
	{
		cv::circle(_image, cv::Point(x, y), r, cv::Scalar(0, 0, 255, 255), 2);
	}

	void Shape::drawEllipse(int x, int y, int a, int b)
	{
		cv::ellipse(_image, cv::Point(x, y), cv::Size(a, b), 0., 0., 360., cv::Scalar(0, 255, 0, 255), 2);
	}

	const cv::Mat& Shape::getImage() const
	{
		return _image;
	}

	const cv::Mat& Shape::getEdge() const
	{
		return _edge;
	}

	const std::set<std::pair<int, int>>& Shape::getEdgePoint() const
	{
		return _edgePoint;
	}

	const std::string& Shape::getPath() const
	{ return _path; }
}
