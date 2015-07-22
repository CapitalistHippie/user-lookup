#include "machine.h"

stmm::Machine::Machine(const char hostname[]) : _hostname(hostname)
{
	_addrInfo		= NULL;

	_gotIpAddress	= false;
}

void stmm::Machine::_GetAddrInfo()
{
	if (_addrInfo)
		return;

	addrinfo addrInfo;
	ZeroMemory(&addrInfo, sizeof(addrInfo));
	addrInfo.ai_family = AF_INET;
	addrInfo.ai_socktype = SOCK_STREAM;
	addrInfo.ai_protocol = IPPROTO_TCP;

	if (getaddrinfo(_hostname.c_str(), NULL, &addrInfo, &_addrInfo))
		throw stmm::WindowsException(WSAGetLastError());
}

const char* stmm::Machine::_GetIPAddress()
{
	if (!_gotIpAddress)
	{
		// Prerequisites
		_GetAddrInfo();

		if (getnameinfo(_addrInfo->ai_addr, sizeof(sockaddr), _ipAddress, sizeof(_ipAddress), NULL, NULL, NI_NUMERICHOST))
			throw stmm::WindowsException(WSAGetLastError());

		_gotIpAddress = true;
	}

	return _ipAddress;
}

void stmm::Machine::_GetLoggedInUsers(LoggedInUsersProc LoggedInUsersProc)
{
	LPWKSTA_USER_INFO_0 userInfoBuffer = NULL;
	DWORD EntriesRead = 0;
	DWORD TotalEntries = 0;

	wchar_t hostnameBuffer[MAX_HOSTNAME_LENGTH + 1];
	mbstowcs_s(NULL, hostnameBuffer, _hostname.c_str(), MAX_HOSTNAME_LENGTH);

	NET_API_STATUS status = NetWkstaUserEnum(hostnameBuffer, 0, (LPBYTE*)&userInfoBuffer, MAX_PREFERRED_LENGTH, &EntriesRead, &TotalEntries, NULL);
	if (status == NERR_Success || status == ERROR_MORE_DATA)
	{
		if (LPWKSTA_USER_INFO_0 tempUserInfoBuffer = userInfoBuffer)
		{
			for (unsigned int i = 0; i < EntriesRead; i++)
			{
				char usernameBuffer[MAX_USERNAME_LENGTH + 1];
				wcstombs_s(NULL, usernameBuffer, tempUserInfoBuffer->wkui0_username, MAX_USERNAME_LENGTH);

				LoggedInUsersProc(this, usernameBuffer);

				tempUserInfoBuffer++;
			}

			// BUG
			// NetApiBufferFree returns ERROR_INVALID_PARAMETER about 50%
			// of the time. figure out why. Using delete for now. Fixes
			// the error but might still have a memory leak.
			//delete userInfoBuffer;

			DWORD errorCode = NetApiBufferFree(userInfoBuffer);
			//if (errorCode != NERR_Success)
			//	throw stmm::WindowsException(errorCode);
		}
	}
	else
		throw stmm::WindowsException(status);
}

bool stmm::Machine::_IsOnline(int timeOut)
{
	// Prerequisites
	_GetAddrInfo();

	HANDLE icmpHandle	= IcmpCreateFile();

	char pingData[]		= "data";
	int replyBufferSize	= sizeof(ICMP_ECHO_REPLY) + sizeof(pingData);
	void* replyBuffer	= malloc(replyBufferSize);

	DWORD result = IcmpSendEcho(icmpHandle, ((sockaddr_in*)_addrInfo->ai_addr)->sin_addr.S_un.S_addr, pingData, sizeof(pingData), NULL, replyBuffer, replyBufferSize, timeOut);
	if (result)
	{
		PICMP_ECHO_REPLY echoReply = (PICMP_ECHO_REPLY)replyBuffer;

		if (echoReply->Status == IP_SUCCESS)
			return true;
		else if (echoReply->Status == IP_REQ_TIMED_OUT)
			return false;
		else
			throw stmm::WindowsException(echoReply->Status);
	}
	else
	{
		int error = GetLastError();
		if (error == IP_REQ_TIMED_OUT)
			return false;
		else
			throw stmm::WindowsException(error);
	}
}