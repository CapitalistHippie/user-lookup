#ifndef _STMM_MACHINE_H_
#define _STMM_MACHINE_H_

#ifdef FORCE_UNICODE
#undef FORCE_UNICODE
#endif

#pragma comment (lib, "Netapi32.lib")
#pragma comment (lib, "Iphlpapi.lib")
#pragma comment (lib, "Ws2_32.lib")

#include <string>

#include <Windows.h>
#include <LM.h>

#include "WinSock2ex.h"
#include <WS2tcpip.h>
#include <IPHlpApi.h>
#include <IcmpAPI.h>

#include "windowsexception.h"

namespace stmm
{
	class Machine
	{
		const static int	MAX_IP_ADDRESS_LENGTH	= 16;
		const static int	MAX_USERNAME_LENGTH		= UNLEN;
		const static int	MAX_HOSTNAME_LENGTH		= CNLEN;

		const std::string	_hostname;

		addrinfo*			_addrInfo;

		bool				_gotIpAddress;
		char				_ipAddress[MAX_IP_ADDRESS_LENGTH];
		
		void				_GetAddrInfo();

	public:
		typedef void(_stdcall* LoggedInUsersProc)(Machine*, const char*);

		Machine(const char hostname[]) throw();
		
		const char* _GetHostname() const throw() { return _hostname.c_str(); };
		const char* _GetIPAddress();
		void		_GetLoggedInUsers(LoggedInUsersProc LoggedInUsersProc);

		bool _IsOnline(int timeOut = 50);
	};
}

#endif // _STMM_MACHINE_H_