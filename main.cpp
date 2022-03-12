#include <opencv2/opencv.hpp>

int main()
{
	cv::VideoCapture cap;
	cap.open(0);

	while (true)
	{
		cv::Mat frame;
		cap.read(frame);
		cv::imshow("out", frame);
		int key = cv::waitKey(1);
		if (key == 8) // backspace
			return 0;
	}
}