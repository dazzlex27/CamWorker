#pragma once

#include "FrameProcessor.h"

namespace cw
{
	class WorkerTwoFrameProcessor : public FrameProcessor
	{
	private:
		hrc::time_point _lastProcessTimestamp;

	public:
		virtual std::string GetName() const override { return "w2"; }

	private:
		virtual Frame ProcessNextFrame(const Frame& frame) override;
	};
}