#include "machinelistview.h"

namespace uslo
{
	extern int		mainWindowClientWidth;			// Width of the main window client area.
	extern int		mainWindowClientHeight;			// Length of the main window client area.

	extern HWND		mainWindow;						// Main window handle.
	
	extern HFONT	windowTextFont;					// Windows text font handle.

	HWND			machineListViewControl = NULL;	// Results list view control handle.

	namespace
	{
		std::vector<std::pair<std::string, std::string>> machines;

		std::mutex mutex;
	}
}

void uslo::InitiateMachineListView()
{
	// Get the instance module handle.
	HINSTANCE instance = GetModuleHandle(NULL);
	if (!instance)
		throw stmm::WindowsException(GetLastError());

	machineListViewControl = CreateWindowEx(NULL, WC_LISTVIEW, NULL,
		WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_OWNERDATA | LVS_REPORT | LVS_NOSORTHEADER,
		RelativeToNormalPixels(179),
		RelativeToNormalPixels(11),
		mainWindowClientWidth - RelativeToNormalPixels(190),
		mainWindowClientHeight - RelativeToNormalPixels(22),
		mainWindow,
		(HMENU)MACHINE_LIST_VIEW_CONTROL_ID,
		instance,
		NULL);
	if (!machineListViewControl)
		throw stmm::WindowsException(GetLastError());
	SendMessage(machineListViewControl, WM_SETFONT, (WPARAM)windowTextFont, NULL);
	ListView_SetExtendedListViewStyleEx(machineListViewControl, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);

	LVCOLUMN listViewColumn;
	listViewColumn.mask		= LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	listViewColumn.iSubItem = 0;
	listViewColumn.pszText	= "Hostname";
	listViewColumn.cx		= 175;
	if (ListView_InsertColumn(machineListViewControl, 0, &listViewColumn) == -1)
		throw stmm::WindowsException(stmm::WindowsException::ERROR_INSERTING_LIST_VIEW_COLUMN);

	listViewColumn.iSubItem = 1;
	listViewColumn.pszText	= "Status";
	listViewColumn.cx		= mainWindowClientWidth - 420;
	if (ListView_InsertColumn(machineListViewControl, 1, &listViewColumn) == -1)
		throw stmm::WindowsException(stmm::WindowsException::ERROR_INSERTING_LIST_VIEW_COLUMN);
}

void uslo::OnGetMachinesDisplayInfoT(NMLVDISPINFO* info)
{
	if (info->item.mask & LVIF_TEXT)
	{
		switch (info->item.iSubItem)
		{
		case 0:
		{
			std::lock_guard<std::mutex> lock(mutex);
			strcpy_s(info->item.pszText, info->item.cchTextMax - 1, machines.at(info->item.iItem).first.c_str());
		}
			info->item.pszText[info->item.cchTextMax - 1] = '\0';
			break;

		case 1:
		{
			std::lock_guard<std::mutex> lock(mutex);
			strcpy_s(info->item.pszText, info->item.cchTextMax - 1, machines.at(info->item.iItem).second.c_str());
		}
			info->item.pszText[info->item.cchTextMax - 1] = '\0';
			break;
		}
	}
}

int uslo::InsertMachineT(const char hostname[])
{
	int machineSize;

	{
		std::lock_guard<std::mutex> lock(mutex);

		machines.emplace_back(hostname, "");
		machineSize = machines.size();
	}

	PostMessage(machineListViewControl, LVM_SETITEMCOUNT, machineSize, LVSICF_NOINVALIDATEALL | LVSICF_NOSCROLL);

	return machineSize - 1;
}

void uslo::SetMachineStatusT(int machineId, const char statusMessage[], ...)
{
	va_list argumentsList;
	va_start(argumentsList, statusMessage);

	char statusMessageBuffer[256];
	vsprintf_s(statusMessageBuffer, sizeof(statusMessageBuffer), statusMessage, argumentsList);

	va_end(argumentsList);
	
	{
		std::lock_guard<std::mutex> lock(mutex);
		machines.at(machineId).second = statusMessageBuffer;
	}
	
	PostMessage(machineListViewControl, LVM_REDRAWITEMS, machineId, machineId);
}

void uslo::ClearMachineListViewT()
{
	{
		std::lock_guard<std::mutex> lock(mutex);
		machines.clear();
	}

	PostMessage(machineListViewControl, LVM_DELETEALLITEMS, NULL, NULL);
}