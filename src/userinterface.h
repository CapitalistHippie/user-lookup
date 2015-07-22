#ifndef _USER_LOOKUP_INTERFACE_H_
#define _USER_LOOKUP_INTERFACE_H_

// Windows 6.0 control style
#pragma comment(linker, "\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#pragma comment (lib, "ComCtl32.lib")

#include <Windows.h>
#include <CommCtrl.h>

#include "font.h"
#include "globalconstants.h"
#include "mainloop.h"
#include "machinelistview.h"
#include "resultslistview.h"
#include "windowsexception.h"

namespace uslo
{
	void InitiateUserInterface();

	void PaintMainWindow();
	void OnMainWindowResize(int newClientWidth, int newClientHeight, bool maximized);
}

#endif // _USER_LOOKUP_SEARCH_H_