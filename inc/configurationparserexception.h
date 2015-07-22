#ifndef _MM_CONFIGURATION_PARSER_CONFIGURATIONPARSER_EXCEPTION_H_
#define _MM_CONFIGURATION_PARSER_CONFIGURATIONPARSER_EXCEPTION_H_

#include <exception>

#include "mmconfigurationparser.h"

namespace mmcp
{
	extern enum ErrorCodes;

	class MMCP_DLL_IMEXPORT ConfigurationParserException : std::exception
	{
		const ErrorCodes _errorCode;

	public:

		const static char* const errorMessages[];

		ConfigurationParserException(const mmcp::ErrorCodes errorCode) throw() : _errorCode(errorCode) {}

		virtual const char* what() const throw() { return errorMessages[_errorCode]; }

		int _GetErrorCode() const throw() { return _errorCode; }
	};
}

#endif // _MM_CONFIGURATION_PARSER_CONFIGURATIONPARSER_EXCEPTION_H_