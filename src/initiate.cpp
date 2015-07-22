#include "initiate.h"

namespace uslo
{
	stmm::ThreadPool machineWorkers;
}

void uslo::Initiate()
{
	WSADATA wsaData;
	int wsaStartupResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (wsaStartupResult)
		throw stmm::WindowsException(wsaStartupResult);

	InitiateUserInterface();

	machineWorkers._Initialize(5);
}