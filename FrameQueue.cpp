#include "FrameQueue.h"

using namespace cw;

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

	_innerQueue.emplace(frame);
}

// since the queue is concurrent, this will imitatate TryDequeue, where it's better to try dequing,
// than checking if there is anything in the queue
Frame FrameQueue::TryPop(bool& success)
{
	success = false;

	std::unique_lock<std::mutex> lock(_mutex);

	if (_innerQueue.empty())
		return Frame(cv::Mat(), hrc::now()); // returns emp

	Frame frame = _innerQueue.front();
	_innerQueue.pop();

	success = true;

	return frame;
}