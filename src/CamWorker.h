#pragma once

#include "FrameProcessors/FrameProcessor.h"

namespace cw
{
	class CamWorker
	{
	public:
		void Run();

	private:
		std::vector<std::shared_ptr<cw::FrameProcessor>> CreateWorkers();
	};
}