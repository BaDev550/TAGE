#pragma once

#include <vector>
#include <queue>
#include <functional>
#include <future>
#include <mutex>
#include <condition_variable>
#include <thread>

namespace TAGE::THREAD {

	class ThreadPool
	{
	public:
		ThreadPool(size_t threadCount = std::thread::hardware_concurrency());
		~ThreadPool();

		template<class F, class... Args>
		auto Enqueue(F&& f, Args&&... args)
			-> std::future<typename std::invoke_result_t<F, Args...>>;

		size_t GetThreadCount() const { return _Workers.size(); }

	private:
		std::vector<std::thread> _Workers;
		std::queue<std::function<void()>> _Tasks;

		std::mutex _QueueMutex;
		std::condition_variable _Condition;
		bool _Stop = false;
	};

	template<class F, class... Args>
	auto ThreadPool::Enqueue(F&& f, Args&&... args)
		-> std::future<typename std::invoke_result_t<F, Args...>>
	{
		using return_type = typename std::invoke_result_t<F, Args...>;

		auto task = std::make_shared<std::packaged_task<return_type()>>(
			std::bind(std::forward<F>(f), std::forward<Args>(args)...)
		);

		std::future<return_type> result = task->get_future();
		{
			std::unique_lock<std::mutex> lock(_QueueMutex);
			_Tasks.emplace([task]() { (*task)(); });
		}
		_Condition.notify_one();
		return result;
	}

}
