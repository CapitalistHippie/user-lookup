#ifndef _USER_LOOKUP_FONT_H_
#define _USER_LOOKUP_FONT_H_

// Not DPI aware. Might do that later: http://msdn.microsoft.com/en-us/library/dd464659(VS.85).aspx

#include <math.h>

#include <Windows.h>

#include "windowsexception.h"

namespace uslo
{
	void InitiateFont();

	int RelativeToNormalPixels(int relativePixels);
	int GetFontHeightInPixels();
}

#endif // _USER_LOOKUP_FONT_H_