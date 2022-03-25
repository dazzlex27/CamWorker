#pragma once

#include "FrameProcessor.h"

namespace cw
{
	class WorkerTwoFrameProcessor : public FrameProcessor
	{
	private:
		hrc::time_point _lastProcessTimestamp;
		std::atomic<bool> _processingInProgress;

	public:
		WorkerTwoFrameProcessor(CircularQueue<std::pair<std::string, Frame>>& processedFrameQueue)
			: _processingInProgress(false), _lastProcessTimestamp(hrc::duration(0)), FrameProcessor(processedFrameQueue)
		{}

		virtual void PushFrame(const Frame& frame) override;

		virtual std::string GetName() const override { return "w2"; }

	private:
		virtual Frame ProcessNextFrame(const Frame& frame) override;

		bool CheckIfNeedToFlipImage(const cv::Mat& image);
	};
}