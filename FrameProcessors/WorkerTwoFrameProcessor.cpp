#include "WorkerTwoFrameProcessor.h"

using namespace cw;
using namespace std::chrono;

// this worker will try to process frames as fast as possible, but only at least one second apart
// since processing and queueing incoming frames are done on separate threads with a mutex-sync,
// there will never be a collision between processing and incoming frame queueing.
// however, if there is a strict need to drop any incoming frame whilst processing is taking place,
// then a simple atomic bool check will suffice to tell if we need to drop the incoming frame or not.
// for the sake of simplicity, I decided not to implement the frame dropping behavior,
// but it could be done very easily, albeit a little kludgy given the multithreaded architecture.

Frame WorkerTwoFrameProcessor::ProcessNextFrame(const Frame& frame)
{
	const auto& frameTimestamp = frame.GetTimestamp();
	auto timeSinceLastUpdate = duration_cast<milliseconds>(frameTimestamp - _lastProcessTimestamp).count();
	if (timeSinceLastUpdate < 1000) // process frames no closer than 1 sec apart
		return Frame(cv::Mat(), hrc::now()); // empty

	_lastProcessTimestamp = frameTimestamp;

	const cv::Mat& originalImage = frame.GetImageData();
	if (originalImage.empty())
		return Frame(cv::Mat(), hrc::now()); // empty

	const bool needToFlipImage = CheckIfNeedToFlipImage(originalImage);

	cv::Mat finalImage;
	// mirror image if mean is in upper half of the intensity range
	if (needToFlipImage)
		cv::flip(originalImage, finalImage, 1); // more than zero is horizontal flip
	else
		finalImage = originalImage;

	return Frame(finalImage, frame.GetTimestamp());
}

// will return true if mean pixel intensity of the left half of the image lies in the top half of the intensity range
bool cw::WorkerTwoFrameProcessor::CheckIfNeedToFlipImage(const cv::Mat& image)
{
	// could be done with masking instead of roi
	cv::Rect roi(0, 0, image.cols / 2, image.rows);
	const cv::Mat& leftHalfImage = image(roi);

	// using a simple 1D-hist on a single-channel 8bpp image. Could do a 2D-hist on HSV image
	cv::Mat grayscaleLeftHalfImage;
	cv::cvtColor(leftHalfImage, grayscaleLeftHalfImage, cv::COLOR_BGR2GRAY);

	const int histSize = 256;
	const int channels[] = { 0 };
	const float range[] = { 0, 256 }; // [ .. )
	const float* histRange[] = { range };
	cv::Mat hist;
	cv::calcHist(&grayscaleLeftHalfImage, 1, channels, cv::Mat(), hist, 1, &histSize, histRange);

	// hist is a collection of values split into bins by intensity
	float accumulatedValue = 0;
	for (int i = 0; i < hist.rows; i++)
		accumulatedValue += hist.at<float>(i) * i;

	const int totalPixelCount = grayscaleLeftHalfImage.cols * grayscaleLeftHalfImage.rows;
	const float meanPixelIntensityValue = accumulatedValue / std::max(totalPixelCount, 1);
	const bool needToFlipImage = meanPixelIntensityValue > range[1] / 2;

	return needToFlipImage;
}