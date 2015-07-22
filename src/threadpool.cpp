#include "threadpool.h"

void stmm::ThreadPool::_WorkerThreadEntryPoint()
{
	while (true)
	{
		std::unique_lock<std::mutex> lock(_mutex);

		if (_terminate)
			break;

		if (!_jobQueue.empty())
		{
			auto functionBuffer = _jobQueue.front();
			_jobQueue.pop();

			if (_jobQueue.empty())
			{
				_overtimeWorker = std::this_thread::get_id();

				lock.unlock();

				functionBuffer();

				lock.lock();

				if (_jobQueue.empty() && _overtimeWorker == std::this_thread::get_id())
				{
					lock.unlock();

					_waitForJobsToFinish.notify_all();
				}
			}
			else
			{
				lock.unlock();

				functionBuffer();
			}
		}
		else
		{
			lock.unlock();

			std::unique_lock<std::mutex> conditionLock(_waitForNewJobMutex);
			_waitForNewJob.wait(conditionLock);
		}
	}
}

stmm::ThreadPool::ThreadPool(const unsigned int numberOfWorkerThreads)
{
	_Initialize(numberOfWorkerThreads);
}

stmm::ThreadPool::~ThreadPool()
{
	_TerminateWorkerThreads();
}

void stmm::ThreadPool::_Initialize(const unsigned int numberOfWorkerThreads)
{
	std::lock_guard<std::mutex> lock(_mutex);

	while (_threads.size() < numberOfWorkerThreads)
		_threads.emplace_back(&stmm::ThreadPool::_WorkerThreadEntryPoint, this);
}

void stmm::ThreadPool::_TerminateWorkerThreads()
{
	_mutex.lock();
	_terminate = true;
	_mutex.unlock();

	_waitForNewJob.notify_all();

	for (auto& element : _threads)
		if (element.joinable())
			element.join();

	_mutex.lock();
	_terminate = false;
	_threads.clear();
	_mutex.unlock();
}

void stmm::ThreadPool::_RemoveJobs()
{
	{
		std::lock_guard<std::mutex> lock(_mutex);

		std::queue<std::function<void()>> emptyJobQueue;
		std::swap(_jobQueue, emptyJobQueue);
	}

	_waitForJobsToFinish.notify_all();
}

void stmm::ThreadPool::_WaitForJobsToFinish()
{
	std::unique_lock<std::mutex> lock(_mutex);

	while (!_jobQueue.empty())
	{
		lock.unlock();

		std::unique_lock<std::mutex> conditionLock(_waitForJobsToFinishMutex);
		_waitForJobsToFinish.wait(conditionLock);

		lock.lock();
	}
}