#include "userinterface.h"

namespace uslo
{
	extern char		domainToSearch[MAX_NETBIOS_DOMAIN_NAME_LENGTH];

	extern HWND		machineListViewControl;				// machines list view control handle.
	extern HWND		resultsListViewControl;				// Results list view control handle.

	extern HFONT	windowTextFont;						// Windows text font handle.

	bool			isWindowMaximized		= false;	// Speaks for itself.

	int				mainWindowXCoordinate	= 50;		// X coordinate of the main window on the screen.
	int				mainWindowYCoordinate	= 50;		// Y coordinate of the main window on the screen.

	int				mainWindowClientWidth	= 1000;		// Width of the main window client area.
	int				mainWindowClientHeight	= 600;		// Length of the main window client area.

	HWND			mainWindow				= NULL;		// Main window handle.

	HWND			usernameEditControl		= NULL;		// Username edit control handle.
	HWND			domainEditControl		= NULL;		// Domain edit control handle.

	HWND			searchButtonControl		= NULL;		// Search button control handle.

	namespace
	{
		const char MAIN_WINDOW_CLASS_NAME[] = "User Lookup Main Window Class";
		const char MAIN_WINDOW_TITLE[]		= "User lookup 3.0.0";
	}
}

void uslo::InitiateUserInterface()
{
	// Get the instance module handle.
	HINSTANCE instance = GetModuleHandle(NULL);
	if (!instance)
		throw stmm::WindowsException(GetLastError());

	// Create and register the main window class.
	WNDCLASSEX mainWindowClass;
	mainWindowClass.cbSize			= sizeof(WNDCLASSEX);
	mainWindowClass.style			= NULL;
	mainWindowClass.lpfnWndProc		= WindowProc;
	mainWindowClass.cbClsExtra		= 0;
	mainWindowClass.cbWndExtra		= 0;
	mainWindowClass.hInstance		= instance;
	mainWindowClass.hIcon			= NULL;
	mainWindowClass.hCursor			= NULL;
	mainWindowClass.hbrBackground	= HBRUSH(CTLCOLOR_DLG + 1);
	mainWindowClass.lpszMenuName	= NULL;
	mainWindowClass.lpszClassName	= MAIN_WINDOW_CLASS_NAME;
	mainWindowClass.hIconSm			= NULL;

	if (!RegisterClassEx(&mainWindowClass))
		throw stmm::WindowsException(GetLastError());

	// Calculate the size the main window should be to get to the right client sizes.
	RECT rect = { 0, 0, mainWindowClientWidth, mainWindowClientHeight };
	if (!AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false))
		throw stmm::WindowsException(GetLastError());

	int mainWindowWidth		= rect.right	- rect.left;
	int mainWindowHeight	= rect.bottom	- rect.top;

	DWORD windowStyles;
	if (isWindowMaximized)
		windowStyles = WS_VISIBLE | WS_OVERLAPPEDWINDOW | WS_MAXIMIZE;
	else
		windowStyles = WS_VISIBLE | WS_OVERLAPPEDWINDOW;

	// Main window.
	mainWindow = CreateWindowEx(NULL,
		MAIN_WINDOW_CLASS_NAME,
		MAIN_WINDOW_TITLE,
		windowStyles,
		mainWindowXCoordinate,
		mainWindowYCoordinate,
		mainWindowWidth,
		mainWindowHeight,
		NULL,
		NULL,
		instance,
		NULL);
	if (!mainWindow)
		throw stmm::WindowsException(GetLastError());

	InitiateFont();

	SendMessage(mainWindow, WM_SETFONT, (WPARAM)windowTextFont, NULL);

	// Username edit control.
	usernameEditControl = CreateWindowEx(NULL, "EDIT", NULL,
		WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL,
		RelativeToNormalPixels(11),
		RelativeToNormalPixels(50) + GetFontHeightInPixels(),
		RelativeToNormalPixels(75),
		RelativeToNormalPixels(23),
		mainWindow,
		(HMENU)USERNAME_EDIT_CONTROL_ID,
		instance,
		NULL);
	if (!usernameEditControl)
		throw stmm::WindowsException(GetLastError());
	SendMessage(usernameEditControl, WM_SETFONT, (WPARAM)windowTextFont, NULL);

	// Domain edit control.
	domainEditControl = CreateWindowEx(NULL, "EDIT", NULL,
		WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL,
		RelativeToNormalPixels(93),
		RelativeToNormalPixels(50) + GetFontHeightInPixels(),
		RelativeToNormalPixels(75),
		RelativeToNormalPixels(23),
		mainWindow,
		(HMENU)DOMAIN_EDIT_CONTROL_ID,
		instance,
		NULL);
	if (!domainEditControl)
		throw stmm::WindowsException(GetLastError());
	SendMessage(domainEditControl, WM_SETFONT, (WPARAM)windowTextFont, NULL);
	SendMessage(domainEditControl, WM_SETTEXT, NULL, (LPARAM)domainToSearch);

	// Search button control.
	searchButtonControl = CreateWindowEx(NULL, "BUTTON", NULL,
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_TEXT,
		RelativeToNormalPixels(11),
		RelativeToNormalPixels(11),
		RelativeToNormalPixels(75),
		RelativeToNormalPixels(23),
		mainWindow,
		(HMENU)SEARCH_BUTTON_CONTROL_ID,
		instance,
		NULL);
	if (!searchButtonControl)
		throw stmm::WindowsException(GetLastError());
	SendMessage(searchButtonControl, WM_SETFONT, (WPARAM)windowTextFont, NULL);
	SendMessage(searchButtonControl, WM_SETTEXT, NULL, (LPARAM)"Lookup user");

	InitiateMachineListView();
	InitiateResultsListView();

	PaintMainWindow();

	if (!SetFocus(usernameEditControl))
		throw stmm::WindowsException(GetLastError());
}

void uslo::PaintMainWindow()
{
	PAINTSTRUCT paintInfo;
	HDC displayDevice = BeginPaint(mainWindow, &paintInfo);
	if (displayDevice)
	{
		SelectObject(displayDevice, windowTextFont);
		SetBkMode(displayDevice, TRANSPARENT);

		TextOut(displayDevice, RelativeToNormalPixels(11), RelativeToNormalPixels(45), "Username:", 9);
		TextOut(displayDevice, RelativeToNormalPixels(93), RelativeToNormalPixels(45), "Domain:", 7);

		EndPaint(mainWindow, &paintInfo);
	}
}

void uslo::OnMainWindowResize(int newClientWidth, int newClientHeight, bool maximized)
{
	isWindowMaximized		= maximized;

	mainWindowClientWidth	= newClientWidth;	// New width of the client area.
	mainWindowClientHeight	= newClientHeight;	// New height of the client area.

	// Username edit control.
	MoveWindow(usernameEditControl,
		RelativeToNormalPixels(11),
		RelativeToNormalPixels(50) + GetFontHeightInPixels(),
		RelativeToNormalPixels(75),
		RelativeToNormalPixels(23),
		true);

	// Domain edit control.
	MoveWindow(domainEditControl,
		RelativeToNormalPixels(93),
		RelativeToNormalPixels(50) + GetFontHeightInPixels(),
		RelativeToNormalPixels(75),
		RelativeToNormalPixels(23),
		true);
	
	// Search button control.
	MoveWindow(searchButtonControl,
		RelativeToNormalPixels(11),
		RelativeToNormalPixels(11),
		RelativeToNormalPixels(75),
		RelativeToNormalPixels(23),
		true);

	// Results list view control.
	MoveWindow(resultsListViewControl,
		RelativeToNormalPixels(11),
		RelativeToNormalPixels(84) + GetFontHeightInPixels(),
		RelativeToNormalPixels(157),
		mainWindowClientHeight - RelativeToNormalPixels(95) - GetFontHeightInPixels(),
		true);

	MoveWindow(machineListViewControl,
		RelativeToNormalPixels(179),
		RelativeToNormalPixels(11),
		mainWindowClientWidth - RelativeToNormalPixels(190),
		mainWindowClientHeight - RelativeToNormalPixels(22),
		true);

	ListView_SetColumnWidth(machineListViewControl, 1, mainWindowClientWidth - 420);//yolo
}