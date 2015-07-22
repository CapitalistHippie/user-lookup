#ifndef _USER_LOOKUP_MAINLOOP_H_
#define _USER_LOOKUP_MAINLOOP_H_

#include <Windows.h>

#include "userinterface.h"
#include "machinelistview.h"
#include "resultslistview.h"
#include "search.h"
#include "terminate.h"

namespace uslo
{
	LRESULT CALLBACK WindowProc(_In_ HWND hwnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);

	int EnterMainLoop();
}

#endif // _USER_LOOKUP_MAINLOOP_H_