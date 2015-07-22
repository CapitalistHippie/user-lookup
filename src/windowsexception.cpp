#include "windowsexception.h"

const char* const stmm::WindowsException::errorMessages[] = {
	"Insufficient space available to set the text in the control.",	// ERROR_INSUFFICIENT_SPACE_AVAILABLE
	"Error when inserting a new list view column.",					// ERROR_INSERTING_LIST_VIEW_COLUMN
	"No display device context available.",							// ERROR_NO_DISPLAY_DEVICE_CONTEXT_AVAILABLE
	"Error selecting object into device context",					// ERROR_SELECTING_OBJECT_INTO_DEVICE_CONTEXT
	"Error setting the background mid mode",						// ERROR_SETTING_BK_MODE
	"Error setting list view column width." };						// ERROR_SETTING_LIST_VIEW_COLUMN_WIDTH

const char* stmm::WindowsException::what() const throw()
{
	if (_errorCode > 20000)
		return errorMessages[_errorCode - 20001];
	return "Unknown windows exception.";
}

char* stmm::WindowsException::what(char* errorMessageBuffer, const int bufferSize) const throw()
{
	if (_errorCode > 20000)
		strcpy_s(errorMessageBuffer, bufferSize, errorMessages[_errorCode - 20001]);

	else
	{
		DWORD errorCode = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, _errorCode, 0, errorMessageBuffer, bufferSize, NULL);
		if (!errorCode)
			sprintf_s(errorMessageBuffer, bufferSize, "Failed to retrieve error message with error: %i.", errorCode);
		else
		{
			// Get rid of \r\n
			*strchr(errorMessageBuffer, '\r') = '\0';
		}
	}
	
	return errorMessageBuffer;
}