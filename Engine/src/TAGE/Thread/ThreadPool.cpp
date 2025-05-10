#include "tagepch.h"
#include "ThreadPool.h"

namespace TAGE::THREAD {
	ThreadPool::ThreadPool(size_t threadCount)
	{
		for (size_t i = 0; i < threadCount; ++i) {
			_Workers.emplace_back([this] {
				while (true) {
					std::function<void()> task;

					{
						std::unique_lock<std::mutex> lock(this->_QueueMutex);
						this->_Condition.wait(lock, [this] {
							return this->_Stop || !this->_Tasks.empty();
							});

						if (this->_Stop && this->_Tasks.empty())
							return;

						task = std::move(this->_Tasks.front());
						this->_Tasks.pop();
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
			worker.join();
	}
}