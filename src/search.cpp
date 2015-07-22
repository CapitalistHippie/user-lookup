#include "search.h"

namespace uslo
{
	extern HWND	searchButtonControl;	// Search button control handle.

	extern stmm::ThreadPool machineWorkers;

	namespace
	{
		std::thread searchUserThread;

		std::mutex mutex;

		void SearchUserThreadEntryPoint()
		{
			try
			{
				SearchMachinesForUser();
			}
			catch (stmm::WindowsException& exception)
			{
				char errorMessage[128];
				char exceptionMessage[128];
				sprintf_s(errorMessage, sizeof(errorMessage), "Lookup failed with error %i: %s", exception._GetErrorCode(), exception.what(exceptionMessage, sizeof(exceptionMessage)));

				MessageBox(NULL, errorMessage, "Error", MB_OK | MB_ICONERROR);
			}
			catch (std::exception& exception)
			{
				char errorMessage[128];
				sprintf_s(errorMessage, sizeof(errorMessage), "Lookup failed with error: %s", exception.what());

				MessageBox(NULL, errorMessage, "Error", MB_OK | MB_ICONERROR);
			}

			if (mutex.try_lock())
			{
				EnableWindow(searchButtonControl, true);
				mutex.unlock();
			}
		}
	}
}

void uslo::SearchUser()
{
	EnableWindow(searchButtonControl, false);

	if (searchUserThread.joinable())
		searchUserThread.detach();

	searchUserThread = std::thread(&SearchUserThreadEntryPoint);
}

void uslo::TerminateSearchUser()
{
	machineWorkers._RemoveJobs();

	if (searchUserThread.joinable())
	{
		std::lock_guard<std::mutex> lock(mutex);
		searchUserThread.join();
	}
}