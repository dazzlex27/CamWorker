#pragma once

#include "FrameProcessor.h"

namespace cw
{
	class WorkerOneFrameProcessor : public FrameProcessor
	{
	private:
		long long _frameCounter;

	public:
		WorkerOneFrameProcessor() : _frameCounter(0) {}

		virtual std::string GetName() const override { return "w1"; }

	private:
		virtual Frame ProcessNextFrame(const Frame& frame) override;
	};
}