#include "WorkerOneFrameProcessor.h"

using namespace cw;

// this worker will process every even frame and rotate every fifth frame, displaying all frames in grayscale
Frame WorkerOneFrameProcessor::ProcessNextFrame(const Frame& frame)
{
	_frameCounter++;

	if (_frameCounter % 2 != 0) // only every second frame is processed
		return Frame(cv::Mat(), hrc::now()); // empty

	cv::Mat imageToProcess;
	cv::cvtColor(frame.GetImageData(), imageToProcess, cv::COLOR_BGR2GRAY);

	if (_frameCounter % 5 == 0) // every fifth frame gets rotated 90deg clockwise 
		cv::rotate(imageToProcess, imageToProcess, cv::ROTATE_90_CLOCKWISE);

	return Frame(imageToProcess, frame.GetTimestamp());
}