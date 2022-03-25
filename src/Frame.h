#pragma once

#include <chrono>
#include <opencv2/core/mat.hpp>

namespace cw
{
	typedef std::chrono::high_resolution_clock hrc;

	class Frame
	{
	private:
		cv::Mat _imageData;
		hrc::time_point _timestamp;

	public:
		Frame()
		{
			_imageData = cv::Mat();
			_timestamp = hrc::now();
		}

		Frame(const cv::Mat& imageData, const hrc::time_point& timestamp)
		{
			_imageData = imageData;
			_timestamp = timestamp;
		}

		cv::Mat GetImageData() const { return _imageData; }
		hrc::time_point GetTimestamp() const { return _timestamp; }
	};
}