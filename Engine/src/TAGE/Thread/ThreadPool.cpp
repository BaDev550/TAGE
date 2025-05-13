#include "tagepch.h"
#include "ThreadPool.h"

namespace TAGE::THREAD {
	ThreadPool::ThreadPool(size_t threadCount)
	{
		for (size_t i = 0; i < threadCount; ++i)
		{
			_Workers.emplace_back([this]() {
				while (true)
				{
					std::function<void()> task;

					{
						std::unique_lock<std::mutex> lock(_QueueMutex);
						_Condition.wait(lock, [this] { return _Stop || !_Tasks.empty(); });

						if (_Stop && _Tasks.empty())
							return;

						task = std::move(_Tasks.front());
						_Tasks.pop();
					}

					task();
				}
				});
		}
	}

	ThreadPool::~ThreadPool()
	{
		{
			std::unique_lock<std::mutex> lock(_QueueMutex);
			_Stop = true;
		}
		_Condition.notify_all();

		for (std::thread& worker : _Workers)
		{
			if (worker.joinable())
				worker.join();
		}
	}

}