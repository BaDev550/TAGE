#pragma once

#include <thread>
#include <queue>

namespace TAGE::THREAD {
	class ThreadPool
	{
	public:
		ThreadPool(size_t threadCount = std::thread::hardware_concurrency());
		~ThreadPool();

		template<class F>
		void Enqueue(F&& task);

	private:
		std::vector<std::thread> _Workers;
		std::queue<std::function<void()>> _Tasks;

		std::mutex _QueueMutex;
		std::condition_variable _Condition;
		bool _Stop = false;
	};
	template<class F>
	inline void ThreadPool::Enqueue(F&& task)
	{
		{
			std::unique_lock<std::mutex> lock(_QueueMutex);
			_Tasks.emplace(std::forward<F>(task));
		}
		_Condition.notify_one();
	}
}