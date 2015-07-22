#include "machines.h"

namespace uslo
{
	extern HWND	resultsListViewControl;	// Results list view control handle.

	extern stmm::ThreadPool machineWorkers;

	char		userToFind[MAX_USERNAME_LENGTH]					= "";
	char		domainToSearch[MAX_NETBIOS_DOMAIN_NAME_LENGTH]	= "mrc";

	namespace
	{
		void _stdcall LoggedInUsersProc(stmm::Machine* machine, const char username[])
		{
			if (!_strcmpi(userToFind, username))
				InsertResultT(machine->_GetHostname());
		}

		void SearchMachineThreadEntryPoint(stmm::Machine machine)
		{
			int machineId;

			try
			{
				machineId = InsertMachineT(machine._GetHostname());
				SetMachineStatusT(machineId, "Checking if machine is online.");

				if (machine._IsOnline())
				{
					SetMachineStatusT(machineId, "Getting logged in users.");
					machine._GetLoggedInUsers(LoggedInUsersProc);
					SetMachineStatusT(machineId, "Done looking.");
				}
				else
					SetMachineStatusT(machineId, "Machine is offline.");
			}
			catch (stmm::WindowsException& exception)
			{
				char exceptionMessage[128];

				SetMachineStatusT(machineId, "Lookup failed with error %i: %s", exception._GetErrorCode(), exception.what(exceptionMessage, sizeof(exceptionMessage)));
			}
			catch (std::exception& exception)
			{
				SetMachineStatusT(machineId, "Lookup failed with error: %s", exception.what());
			}
		}
	}
}

void uslo::SearchMachinesForUser()
{
	ClearMachineListViewT();
	ClearResultsListViewT();

	wchar_t domainBuffer[MAX_NETBIOS_DOMAIN_NAME_LENGTH + 1];
	mbstowcs_s(NULL, domainBuffer, domainToSearch, MAX_NETBIOS_DOMAIN_NAME_LENGTH);

	LPSERVER_INFO_100 serverInfoBuffer = NULL;
	DWORD entriesRead = 0;
	DWORD totalEntries = 0;

	int status = NetServerEnum(NULL, 100, (LPBYTE*)&serverInfoBuffer, MAX_PREFERRED_LENGTH, &entriesRead, &totalEntries, SV_TYPE_ALL, domainBuffer, NULL);
	if (status == NERR_Success || status == ERROR_MORE_DATA)
	{
		if (LPSERVER_INFO_100 tempServerInfoBuffer = serverInfoBuffer)
		{
			for (unsigned int i = 0; i < entriesRead; i++)
			{
				char hostnameBuffer[MAX_HOSTNAME_LENGTH + 1];
				wcstombs_s(NULL, hostnameBuffer, tempServerInfoBuffer->sv100_name, MAX_HOSTNAME_LENGTH);

				machineWorkers._AddJob(std::bind(SearchMachineThreadEntryPoint, stmm::Machine(hostnameBuffer)));

				tempServerInfoBuffer++;
			}

			// BUG
			// NetApiBufferFree returns ERROR_INVALID_PARAMETER about 50%
			// of the time. figure out why. Using delete for now. Fixes
			// the error but might still have a memory leak.
			//delete serverInfoBuffer;

			DWORD errorCode = NetApiBufferFree(serverInfoBuffer);
			//if (errorCode != NERR_Success)
			//	throw stmm::WindowsException(errorCode);

			machineWorkers._WaitForJobsToFinish();
		}
	}
	else
		throw stmm::WindowsException(status);
}