#ifndef _USER_LOOKUP_SEARCH_H_
#define _USER_LOOKUP_SEARCH_H_

#include <thread>
#include <mutex>

#include <Windows.h>

#include "machines.h"
#include "windowsexception.h"
#include "threadpool.h"

namespace uslo
{
	void SearchUser();
	void TerminateSearchUser();
}

#endif // _USER_LOOKUP_SEARCH_H_