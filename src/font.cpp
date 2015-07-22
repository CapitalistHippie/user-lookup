#include "font.h"

namespace uslo
{
	extern HWND		mainWindow;				// Main window handle.

	HFONT			windowTextFont	= NULL;	// Windows text font handle.

	namespace
	{
		float	relativePixelSize;
		int		fontHeightInPixels;
	}
}

void uslo::InitiateFont()
{
	// Piece of code to retrieve the default windows text font.
	NONCLIENTMETRICS nonClientMetrics;
	nonClientMetrics.cbSize = sizeof(nonClientMetrics);
#if (WINVER >= 0x0600)
	OSVERSIONINFO osVersionInfo;
	osVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	GetVersionEx(&osVersionInfo);
	if (osVersionInfo.dwMajorVersion < 6)
		nonClientMetrics.cbSize -= sizeof(int);
#endif

	if (!SystemParametersInfo(SPI_GETNONCLIENTMETRICS, nonClientMetrics.cbSize, &nonClientMetrics, NULL))
		throw stmm::WindowsException(GetLastError());

	windowTextFont = CreateFontIndirect(&nonClientMetrics.lfMessageFont);
	if (!windowTextFont)
		throw stmm::WindowsException(GetLastError());

	HDC displayDevice = GetDC(mainWindow);
	SelectObject(displayDevice, windowTextFont);

	TEXTMETRIC textMetrics;
	GetTextMetrics(displayDevice, &textMetrics);

	fontHeightInPixels = textMetrics.tmHeight;

	// Get a relative pixel size in pixels.
	relativePixelSize = 1.0f / 96.0f * (float)GetDeviceCaps(displayDevice, LOGPIXELSX);
}

int uslo::RelativeToNormalPixels(int relativePixels)
{
	return (int)floor(relativePixels * relativePixelSize + 0.5);
}

int uslo::GetFontHeightInPixels()
{
	return fontHeightInPixels;
}