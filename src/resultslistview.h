#ifndef _USER_LOOKUP_RESULTS_LIST_VIEW_H_
#define _USER_LOOKUP_RESULTS_LIST_VIEW_H_

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
	void InitiateResultsListView();
	void OnGetResultsDisplayInfoT(NMLVDISPINFO* info);

	int InsertResultT(const char hostname[]);

	void ClearResultsListViewT();
}

#endif // _USER_LOOKUP_RESULTS_LIST_VIEW_H_