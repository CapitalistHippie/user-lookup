#ifndef _STMM_THREAD_POOL_H_
#define _STMM_THREAD_POOL_H_

#include <vector>
#include <queue>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace stmm
{
	class ThreadPool
	{
		std::vector<std::thread>			_threads;

		std::mutex							_mutex;

		std::thread::id						_overtimeWorker;

		bool								_terminate = false;
		std::queue<std::function<void()>>	_jobQueue;

		std::condition_variable				_waitForNewJob;
		std::mutex							_waitForNewJobMutex;

		std::condition_variable				_waitForJobsToFinish;
		std::mutex							_waitForJobsToFinishMutex;

		void								_WorkerThreadEntryPoint();

	public:
		ThreadPool(){};
		ThreadPool(const unsigned int numberOfWorkerThreads);
		~ThreadPool();

		void _Initialize(const unsigned int numberOfWorkerThreads);
		void _TerminateWorkerThreads();
		
		template <typename FunctionType>
		void _AddJob(const FunctionType function)
		{
			{
				std::lock_guard<std::mutex> lock(_mutex);
				
				_jobQueue.emplace(function);
			}
			_waitForNewJob.notify_one();
		}

		void _RemoveJobs();
		void _WaitForJobsToFinish();
	};
}

#endif	// _STMM_THREAD_POOL_H_