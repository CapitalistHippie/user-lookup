#ifndef _USER_LOOKUP_MACHINES_H_
#define _USER_LOOKUP_MACHINES_H_

#include <functional>

#include <Windows.h>
#include <LM.h>

#include "machinelistview.h"
#include "resultslistview.h"
#include "threadpool.h"
#include "globalconstants.h"
#include "machine.h"
#include "windowsexception.h"

namespace uslo
{
	void SearchMachinesForUser();
}

#endif // _USER_LOOKUP_MACHINES_H_