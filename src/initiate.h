#ifndef _USER_LOOKUP_SETUP_H_
#define _USER_LOOKUP_SETUP_H_

#include <Windows.h>
#include "WinSock2ex.h"

#include "threadpool.h"
#include "userinterface.h"
#include "search.h"
#include "windowsexception.h"

namespace uslo
{
	void Initiate();
}

#endif // _USER_LOOKUP_SETUP_H_