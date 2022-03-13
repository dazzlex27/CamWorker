#include "FrameQueue.h"

FrameQueue::FrameQueue(uint capacity)
	: _capacity(capacity)
{
}

void FrameQueue::Push(const Frame& frame)
{
	std::unique_lock<std::mutex> lock(_mutex);

	if (_innerQueue.size() == _capacity)
	{
		_innerQueue.pop();
		std::cout << "frame queue capacity exceeded" << std::endl;
	}

	_innerQueue.emplace(frame/*.clone()*/); // will copy data
}

Frame FrameQueue::Pop()
{
	std::unique_lock<std::mutex> lock(_mutex);

	if (_innerQueue.empty())
		return Frame(cv::Mat(), hrc::now()); // empty

	Frame frame = _innerQueue.front();
	_innerQueue.pop();

	return frame;
}