#include "mainloop.h"

namespace uslo
{
	extern bool	isWindowMaximized;		// Speaks for itself.

	extern int	mainWindowXCoordinate;	// X coordinate of the main window on the screen.
	extern int	mainWindowYCoordinate;	// Y coordinate of the main window on the screen.

	extern int	mainWindowClientWidth;	// Width of the main window client area.
	extern int	mainWindowClientHeight;	// Length of the main window client area.

	extern char	userToFind[MAX_USERNAME_LENGTH];
	extern char	domainToSearch[MAX_NETBIOS_DOMAIN_NAME_LENGTH];

	extern HWND mainWindow;
}

int uslo::EnterMainLoop()
{
	MSG message;
	while (GetMessage(&message, NULL, NULL, NULL))
	{
		if (!IsDialogMessage(mainWindow, &message))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
	}

	return (int)message.wParam;
}

LRESULT CALLBACK uslo::WindowProc(_In_ HWND hwnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		TerminateRunningProcesses();
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	
	case WM_PAINT:
		PaintMainWindow();
		break;

	case WM_MOVE:
	{
		mainWindowXCoordinate = (int)LOWORD(lParam);
		mainWindowYCoordinate = (int)HIWORD(lParam);
	}
		break;

	case WM_SIZE:
	{
		if (wParam == SIZE_MAXIMIZED)
			OnMainWindowResize(LOWORD(lParam), HIWORD(lParam), true);
		else
			OnMainWindowResize(LOWORD(lParam), HIWORD(lParam), false);
	}
		break;

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case SEARCH_BUTTON_CONTROL_ID:
			if (HIWORD(wParam) == BN_CLICKED)
				SearchUser();
			break;
		case USERNAME_EDIT_CONTROL_ID:
			if (HIWORD(wParam) == EN_CHANGE)
				SendMessage((HWND)lParam, WM_GETTEXT, sizeof(userToFind), (LPARAM)userToFind);
			break;
		case DOMAIN_EDIT_CONTROL_ID:
			if (HIWORD(wParam) == EN_CHANGE)
				SendMessage((HWND)lParam, WM_GETTEXT, sizeof(domainToSearch), (LPARAM)domainToSearch);
			break;
		}
	}
		break;

	case WM_NOTIFY:
	{
		switch (((LPNMHDR)lParam)->idFrom)
		{
		case MACHINE_LIST_VIEW_CONTROL_ID:
		{
			if (((LPNMHDR)lParam)->code == LVN_GETDISPINFO)
				OnGetMachinesDisplayInfoT((NMLVDISPINFO*)lParam);
		}
			break;
		case RESULTS_LIST_VIEW_CONTROL_ID:
		{
			if (((LPNMHDR)lParam)->code == LVN_GETDISPINFO)
				OnGetResultsDisplayInfoT((NMLVDISPINFO*)lParam);
		}
			break;
		}
		
	}
		break;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}