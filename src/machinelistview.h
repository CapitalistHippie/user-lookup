#ifndef _USER_LOOKUP_MACHINES_LIST_VIEW_H_
#define _USER_LOOKUP_MACHINES_LIST_VIEW_H_

#include <mutex>
#include <string>
#include <vector>

#include <Windows.h>
#include <CommCtrl.h>

#include "font.h"
#include "globalconstants.h"
#include "windowsexception.h"

namespace uslo
{
	void InitiateMachineListView();
	void OnGetMachinesDisplayInfoT(NMLVDISPINFO* info);

	int InsertMachineT(const char hostname[]);
	void SetMachineStatusT(int machineId, const char statusMessage[], ...);

	void ClearMachineListViewT();
}

#endif // _USER_LOOKUP_MACHINES_LIST_VIEW_H_