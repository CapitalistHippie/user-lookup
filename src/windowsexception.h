#ifndef _STMM_WINDOWSEXCEPTION_H_
#define _STMM_WINDOWSEXCEPTION_H_

#include <exception>

#include <Windows.h>
#include <stdio.h>

namespace stmm
{
	const class WindowsException : public std::exception
	{
		const static char* const errorMessages[];

		const DWORD _errorCode;

	public:
		const static int ERROR_INSUFFICIENT_SPACE_AVAILABLE = 20001;
		const static int ERROR_INSERTING_LIST_VIEW_COLUMN = 20002;
		const static int ERROR_NO_DISPLAY_DEVICE_CONTEXT_AVAILABLE = 20003;
		const static int ERROR_SELECTING_OBJECT_INTO_DEVICE_CONTEXT = 20004;
		const static int ERROR_SETTING_BK_MODE = 20005;
		const static int ERROR_SETTING_LIST_VIEW_COLUMN_WIDTH = 20006;

		WindowsException(const DWORD errorCode) throw() : _errorCode(errorCode) {}

		virtual const char* what() const throw();
		virtual char* what(char* errorMessageBuffer, const int bufferSize) const throw();

		DWORD _GetErrorCode() const throw() { return _errorCode; }
	};
}

#endif // _STMM_WINDOWSEXCEPTION_H_