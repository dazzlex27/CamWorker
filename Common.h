#pragma once

#include <chrono>
#include <opencv2/opencv.hpp> // probably not the best to include OpenCV in a common header, but will do here

namespace cw
{
	typedef unsigned int uint;
	typedef std::chrono::high_resolution_clock hrc;
}