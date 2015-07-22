#include "resultslistview.h"

namespace uslo
{
	extern int		mainWindowClientWidth;			// Width of the main window client area.
	extern int		mainWindowClientHeight;			// Length of the main window client area.

	extern HWND		mainWindow;						// Main window handle.
	
	extern HFONT	windowTextFont;					// Windows text font handle.

	HWND			resultsListViewControl = NULL;	// Results list view control handle.

	namespace
	{
		std::vector<std::string> results;

		std::mutex mutex;
	}
}

void uslo::InitiateResultsListView()
{
	// Get the instance module handle.
	HINSTANCE instance = GetModuleHandle(NULL);
	if (!instance)
		throw stmm::WindowsException(GetLastError());

	resultsListViewControl = CreateWindowEx(
		NULL,
		WC_LISTVIEW,
		NULL,
		WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_OWNERDATA | LVS_REPORT | LVS_NOSORTHEADER,
		RelativeToNormalPixels(11),
		RelativeToNormalPixels(84) + GetFontHeightInPixels(),
		RelativeToNormalPixels(157),
		mainWindowClientHeight - RelativeToNormalPixels(95) - GetFontHeightInPixels(),
		mainWindow,
		(HMENU)RESULTS_LIST_VIEW_CONTROL_ID,
		instance,
		NULL);
	if (!resultsListViewControl)
		throw stmm::WindowsException(GetLastError());
	SendMessage(resultsListViewControl, WM_SETFONT, (WPARAM)windowTextFont, NULL);
	ListView_SetExtendedListViewStyleEx(resultsListViewControl, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);

	LVCOLUMN listViewColumn;
	listViewColumn.mask		= LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	listViewColumn.iSubItem = 0;
	listViewColumn.pszText	= "Results";
	listViewColumn.cx		= RelativeToNormalPixels(157);
	if (ListView_InsertColumn(resultsListViewControl, 0, &listViewColumn) == -1)
		throw stmm::WindowsException(stmm::WindowsException::ERROR_INSERTING_LIST_VIEW_COLUMN);
}

void uslo::OnGetResultsDisplayInfoT(NMLVDISPINFO* info)
{
	if (info->item.mask & LVIF_TEXT)
	{
		switch (info->item.iSubItem)
		{
		case 0:
		{
			std::lock_guard<std::mutex> lock(mutex);
			strcpy_s(info->item.pszText, info->item.cchTextMax - 1, results.at(info->item.iItem).c_str());
		}
			info->item.pszText[info->item.cchTextMax - 1] = '\0';
			break;
		}
	}
}

int uslo::InsertResultT(const char hostname[])
{
	int resultsSize;

	{
		std::lock_guard<std::mutex> lock(mutex);

		results.emplace_back(hostname);
		resultsSize = results.size();
	}

	PostMessage(resultsListViewControl, LVM_SETITEMCOUNT, resultsSize, LVSICF_NOINVALIDATEALL | LVSICF_NOSCROLL);

	return resultsSize - 1;
}

void uslo::ClearResultsListViewT()
{
	{
		std::lock_guard<std::mutex> lock(mutex);
		results.clear();
	}

	PostMessage(resultsListViewControl, LVM_DELETEALLITEMS, NULL, NULL);
}