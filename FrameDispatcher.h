#pragma once

#include <list>
#include <functional>
#include "FrameQueue.h"
#include "FrameProcessors/FrameProcessor.h"

namespace cw
{
	class FrameDispatcher
	{
	private:
		cv::VideoCapture _capture;
		FrameQueue _queue;
		std::atomic<bool> _isFeedActive;
		std::vector<std::shared_ptr<FrameProcessor>> _subscribers;
		std::thread _thRead;
		std::thread _thNotify;

	public:
		FrameDispatcher(cv::VideoCapture& capture, const uint queueCapacity = 25);
		~FrameDispatcher();

		bool IsActive() const {	return _isFeedActive; }

		void Start();
		void Stop();

		void AddSubscriber(const std::shared_ptr<FrameProcessor>& processor);
		void RemoveAllSubscribers();

	private:
		void RunFeedReading();
		void RunSubscriberNotification();
	};
}