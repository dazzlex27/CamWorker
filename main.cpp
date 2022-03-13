#include <thread>
#include <atomic>
#include "Common.h"
#include "FrameQueue.h"
#include "Frame.h"

void ReadFeed(FrameQueue& frameQueue, std::atomic<bool>& isFeedActive)
{
	std::cout << "starting feed thread..." << std::endl;

	cv::VideoCapture cap;
	cap.open(0);

	// set up specific video config if necessary
	//cap.set(cv::CAP_PROP_FRAME_WIDTH, 1280);
	//cap.set(cv::CAP_PROP_FRAME_HEIGHT, 720);
	//cap.set(cv::CAP_PROP_FPS, 25.0);

	// poll actual video params
	const double actualWidth = cap.get(cv::CAP_PROP_FRAME_WIDTH);
	const double actualHeight = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
	const double actualFps = cap.get(cv::CAP_PROP_FPS);
	std::cout << "Feed params: w=" << actualWidth << " h=" << actualHeight << " fps=" << actualFps << std::endl;

	while (isFeedActive)
	{
		if (!cap.isOpened())
		{
			isFeedActive = false;
			break;
		}

		cv::Mat image;
		cap.read(image);

		auto now = hrc::now();
		Frame frame(image, now);

		frameQueue.Push(frame);
	}

	std::cout << "feed thread finished" << std::endl;
}

int main()
{
	const char* windowName = "main";
	cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE);

	FrameQueue queue = FrameQueue(10);
	std::atomic<bool> isFeedActive = true; // wiil be set to false if either feed stops or exit is prompted

	std::thread th(ReadFeed, std::ref(queue), std::ref(isFeedActive));
	std::cout << "feed started" << std::endl;

	while (isFeedActive)
	{
		const Frame& frame = queue.Pop();
		cv::Mat imageData = frame.GetImageData();
		if (imageData.empty())
			continue;

		cv::imshow(windowName, imageData);
		int key = cv::waitKey(1);
		if (key != -1) // press any key to exit
			isFeedActive = false;
	}

	th.join();

	cv::destroyAllWindows();

	std::cout << "all done!" << std::endl;
}