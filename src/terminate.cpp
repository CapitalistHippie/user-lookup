#include "terminate.h"

namespace uslo
{
	extern stmm::ThreadPool machineWorkers;
}

void uslo::TerminateRunningProcesses()
{
	TerminateSearchUser();

	machineWorkers._TerminateWorkerThreads();
}