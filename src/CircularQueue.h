#pragma once

#include <iostream>
#include <queue>
#include <mutex>
#include <atomic>
#include <condition_variable>

namespace cw
{
	template <typename T>
	class CircularQueue
	{
	private:
		std::queue<T> _queue;
		std::mutex _mutex;
		std::condition_variable _cond;
		std::atomic<bool> _disposed;
		unsigned int _capacity;

	public:
		CircularQueue(unsigned int capacity)
			: _capacity(capacity), _disposed(false)
		{}

		~CircularQueue()
		{
			StopQueue();
		}

		void StopQueue()
		{
			std::unique_lock<std::mutex> lock(_mutex);
			_disposed = true;
			_cond.notify_all();
		}

		void Push(const T& element)
		{
			std::unique_lock<std::mutex> lock(_mutex);

			if (_queue.size() == _capacity)
			{
				_queue.pop();
				std::cout << "queue capacity exceeded" << std::endl;
			}

			_queue.emplace(element);

			_cond.notify_all();
		}

		// this will return the next element in queue or wait until an element appears in the queue
		T Pop()
		{
			std::unique_lock<std::mutex> lock(_mutex);

			while (_queue.empty() && !_disposed)
				_cond.wait(lock);

			if (_disposed)
				return T();

			T element = _queue.front();
			_queue.pop();

			return element;
		}

		// this is non-blocking and will return default value of T and assign false to "success" if queue is empty
		T TryPop(bool& success)
		{
			success = false;

			std::unique_lock<std::mutex> lock(_mutex);

			if (_queue.empty())
				return T(); // returns emp

			T element = _queue.front();
			_queue.pop();

			success = true;

			_cond.notify_one();

			return element;
		}

		unsigned int GetCapacity() const { return _capacity; }
	};
}