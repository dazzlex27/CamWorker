#pragma once

#include "FrameProcessors/FrameProcessor.h"

namespace cw
{
	class CamWorker
	{
	private:
		std::thread _thNofify;
		std::vector<std::shared_ptr<FrameProcessor>> _workers;
		std::atomic<bool> _runLoop;
		cw::CircularQueue<Frame> _rawFrameQueue;
		cw::CircularQueue<std::pair<std::string, Frame>> _procesedFrameQueue;

	public:
		CamWorker();

		void Run();

	private:
		std::vector<std::shared_ptr<FrameProcessor>> CreateWorkers();
		void RunWorkerNotification();
		void Stop();
	};
}