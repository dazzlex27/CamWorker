#pragma once

#include "Common.h"

namespace cw
{
	class Frame
	{
	private:
		cv::Mat _imageData;
		hrc::time_point _timestamp;

	public:
		Frame(const cv::Mat& imageData, const hrc::time_point& timestamp)
		{
			_imageData = imageData;
			_timestamp = timestamp;
		}

		cv::Mat GetImageData() const { return _imageData; }
		hrc::time_point GetTimestamp() const { return _timestamp; }
	};
}