#pragma once

#include <queue>
#include <mutex>
#include "Common.h"
#include "Frame.h"

namespace cw
{
	class FrameQueue
	{
	private:
		std::queue<Frame> _innerQueue;
		std::mutex _mutex;
		uint _capacity;

	public:
		FrameQueue(uint capacity);

		void Push(const Frame& frame);
		Frame TryPop(bool& success);

		uint GetCapacity() const { return _capacity; }
	};
}