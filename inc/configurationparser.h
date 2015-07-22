#ifndef _MM_CONFIGURATION_PARSER_CONFIGURATIONPARSER_H_
#define _MM_CONFIGURATION_PARSER_CONFIGURATIONPARSER_H_

#include <fstream>
#include <vector>
#include <string>
#include <memory>

#include "mmconfigurationparser.h"
#include "configurationparserexception.h"

namespace mmcp
{
	extern enum ReturnCodes;
	extern enum ErrorCodes;

#pragma warning(disable:4251)
	class MMCP_DLL_IMEXPORT ConfigurationParser
	{
		std::string											_filePath;				// Path and name of the configuration file loaded.

		int													_maxKeyLength;			// Maximum length of a key.
		int													_maxValueLength;		// Maximum length of a value.

		bool												_configurationLoaded;	// If true a configuration file is currently loaded.

		std::vector<std::string>							_stringKeys;			// Vector to save the keys in.
		std::vector<std::string>							_numericKeys;			// Vector to save the keys in.
		std::vector<std::string>							_stringValues;			// Vector to save the keys in.
		std::vector<std::pair<char, std::shared_ptr<void>>>	_numericValues;			// Vector to save the values in.

		/**
		* Private method that initiates all the class members to zero.
		*
		* Exception safety: No-throw guarantee.
		*
		* @noreturn
		*/
		void _ZeroMembers() throw();

		template <typename T>
		mmcp::ReturnCodes __GetValue(const char key[], T* valueBuffer, const T defaultValue, const bool addValue, const char type)
		{
			if (!_configurationLoaded)
				throw ConfigurationParserException(MMCP_NO_CONFIGURATION_LOADED);

			for (unsigned int i = 0; i < _numericKeys.size(); i++)
			{
				if (!_numericKeys.at(i).compare(key))
				{
					*valueBuffer = *static_cast<T*>(_numericValues.at(i).second.get());
					return MMCP_SUCCESS_VALUE_FOUND;
				}
			}

			if (defaultValue)
			{
				*valueBuffer = defaultValue;

				if (addValue)
				{
					std::vector<std::string>							keysBuffer = _numericKeys;
					std::vector<std::pair<char, std::shared_ptr<void>>>	valuesBuffer = _numericValues;

					keysBuffer.push_back(key);
					valuesBuffer.push_back(std::pair<char, std::shared_ptr<void>>(type, std::shared_ptr<void>(new T(defaultValue))));

					_numericKeys.swap(keysBuffer);
					_numericValues.swap(valuesBuffer);

					return MMCP_SUCCESS_VALUE_ADDED;
				}

				return MMCP_SUCCESS_DEFAULT_USED;
			}

			throw ConfigurationParserException(MMCP_VALUE_NOT_FOUND);
		}

		template <typename T>
		mmcp::ReturnCodes __SetValue(const char key[], const T value, const bool addValue, const char type)
		{
			if (!_configurationLoaded)
				throw ConfigurationParserException(MMCP_NO_CONFIGURATION_LOADED);

			for (unsigned int i = 0; i < _numericKeys.size(); i++)
			{
				if (!_numericKeys.at(i).compare(key))
				{
					std::vector<std::pair<char, std::shared_ptr<void>>> valuesBuffer = _numericValues;

					*static_cast<T*>(valuesBuffer.at(i).second.get()) = value;

					_numericValues.swap(valuesBuffer);

					return MMCP_SUCCESS_VALUE_SET;
				}
				else if (i == _numericKeys.size() - 1 && addValue)
				{
					std::vector<std::string>							keysBuffer = _numericKeys;
					std::vector<std::pair<char, std::shared_ptr<void>>>	valuesBuffer = _numericValues;

					keysBuffer.push_back(key);
					valuesBuffer.push_back(std::pair<char, std::shared_ptr<void>>(type, std::shared_ptr<void>(new T(value))));

					_numericKeys.swap(keysBuffer);
					_numericValues.swap(valuesBuffer);

					return MMCP_SUCCESS_VALUE_ADDED;
				}
			}

			throw ConfigurationParserException(MMCP_VALUE_NOT_FOUND);
		}

	public:

		const static int DEFAULT_MAX_KEY_LENGTH;
		const static int DEFAULT_MAX_VALUE_LENGTH;

		/**
		* Constructor that initiates all the class members to zero.
		*
		* The constructor calls the private method _ZeroMembers which
		* sets all the class members to zero.
		*
		* Exception safety: No-throw guarantee.
		*/
		ConfigurationParser() throw();

		/**
		* Constructor that loads a configuration file into memory.
		*
		* The constructor calls the private method _ZeroMembers which
		* sets all the class members to zero. After that it calls the method
		* _LoadConfiguration with the provided variables. You can check
		* _IsConfigurationLoaded() to see if the configuration was loaded
		* successfully.
		*
		* Exception safety: Strong exception safety.
		*
		* @param	filePath		Path and name of the configuration file to
		*							be loaded.
		* @param	create			If true, create the file if it wasn't
		*							found.
		* @param	maxKeyLength	Maximum length of a key.
		* @param	maxValueLength	Maximum length of a value.
		*/
		ConfigurationParser(const char filePath[], const bool create = true, const int maxKeyLength = DEFAULT_MAX_KEY_LENGTH, const int maxValueLength = DEFAULT_MAX_VALUE_LENGTH);

		operator bool() const throw() { return _IsConfigurationLoaded(); }

		/**
		* Public method that loads a configuration into memory.
		*
		* Exception safety: Strong exception safety.
		*
		* @param	filePath		Path and name of the configuration file to
		*							be loaded.
		* @param	create			If true, create the file if it wasn't
		*							found.
		* @param	maxKeyLength	Maximum length of a key.
		* @param	maxValueLength	Maximum length of a value.
		*
		* @noreturn
		*/
		void _LoadConfiguration(const char filePath[], const bool create = true, const int maxKeyLength = DEFAULT_MAX_KEY_LENGTH, const int maxValueLength = DEFAULT_MAX_VALUE_LENGTH);

		/**
		* Public method that saves the loaded configuration to a file.
		*
		* The method opens and truncates the file given. If the file doens't
		* exist it is created.
		*
		* Exception safety: Basic exception safety.
		* If an exception happens during writing you can end up with half a
		* written file.
		*
		* @param	filePath		Path and name of the configuration file to
		*							be loaded. If NULL, it will use the file it
		*							was loaded from.
		*
		* @noreturn
		*/
		void _SaveConfiguration(const char filePath[] = NULL);

		/**
		* Gets a value.
		*
		* Public method that gets the value behind the given key. If the file
		* wasn't found it will use the defaultValue instead. If addValue is
		* true it will add the key and value pair to the configuration. Note
		* defaultValue needs to be not NULL for that to work.
		*
		* Exception safety: Strong exception safety.
		*
		* @param	key				The key the value is behind.
		* @param	valueBuffer		The buffer the value is to be written to.
		* @param	bufferSize		Size of valueBuffer.
		* @param	defaultValue	The value to use if the key and value pair
		*							don't exist.
		* @param	addValue		If true, creates the key value pair if it
		*							doesn't exist using defaultValue as value.
		*
		* @return	One of the mmcp::ReturnCodes values.
		*/
		mmcp::ReturnCodes _GetValue(const char key[], char valueBuffer[], const int bufferSize, const char defaultValue[] = NULL, const bool addValue = true);

		mmcp::ReturnCodes _GetValue(const char key[], char* valueBuffer, const char defaultValue, const bool addValue = true)								{ return __GetValue(key, valueBuffer, defaultValue, addValue, '1'); }
		mmcp::ReturnCodes _GetValue(const char key[], signed char* valueBuffer, const signed char defaultValue, const bool addValue = true)					{ return __GetValue(key, valueBuffer, defaultValue, addValue, '2'); }
		mmcp::ReturnCodes _GetValue(const char key[], unsigned char* valueBuffer, const unsigned char defaultValue, const bool addValue = true)				{ return __GetValue(key, valueBuffer, defaultValue, addValue, '3'); }
		mmcp::ReturnCodes _GetValue(const char key[], bool* valueBuffer, const bool defaultValue, const bool addValue = true)								{ return __GetValue(key, valueBuffer, defaultValue, addValue, '4'); }
		mmcp::ReturnCodes _GetValue(const char key[], short* valueBuffer, const short defaultValue, const bool addValue = true)								{ return __GetValue(key, valueBuffer, defaultValue, addValue, '5'); }
		mmcp::ReturnCodes _GetValue(const char key[], unsigned short* valueBuffer, const unsigned short defaultValue, const bool addValue = true)			{ return __GetValue(key, valueBuffer, defaultValue, addValue, '6'); }
		mmcp::ReturnCodes _GetValue(const char key[], int* valueBuffer, const int defaultValue, const bool addValue = true)									{ return __GetValue(key, valueBuffer, defaultValue, addValue, '7'); }
		mmcp::ReturnCodes _GetValue(const char key[], unsigned int* valueBuffer, const unsigned int defaultValue, const bool addValue = true)				{ return __GetValue(key, valueBuffer, defaultValue, addValue, '8'); }
		mmcp::ReturnCodes _GetValue(const char key[], long* valueBuffer, const long defaultValue, const bool addValue = true)								{ return __GetValue(key, valueBuffer, defaultValue, addValue, '9'); }
		mmcp::ReturnCodes _GetValue(const char key[], unsigned long* valueBuffer, const unsigned long defaultValue, const bool addValue = true)				{ return __GetValue(key, valueBuffer, defaultValue, addValue, 'a'); }
		mmcp::ReturnCodes _GetValue(const char key[], long long* valueBuffer, const long long defaultValue, const bool addValue = true)						{ return __GetValue(key, valueBuffer, defaultValue, addValue, 'b'); }
		mmcp::ReturnCodes _GetValue(const char key[], unsigned long long* valueBuffer, const unsigned long long defaultValue, const bool addValue = true)	{ return __GetValue(key, valueBuffer, defaultValue, addValue, 'c'); }
		mmcp::ReturnCodes _GetValue(const char key[], float* valueBuffer, const float defaultValue, const bool addValue = true)								{ return __GetValue(key, valueBuffer, defaultValue, addValue, 'd'); }
		mmcp::ReturnCodes _GetValue(const char key[], double* valueBuffer, const double defaultValue, const bool addValue = true)							{ return __GetValue(key, valueBuffer, defaultValue, addValue, 'e'); }
		mmcp::ReturnCodes _GetValue(const char key[], long double* valueBuffer, const long double defaultValue, const bool addValue = true)					{ return __GetValue(key, valueBuffer, defaultValue, addValue, 'f'); }

		/**
		* Sets a value.
		*
		* Public method that sets a value. Optionally also creates it if it
		* doesn't exist yet.
		*
		* Exception safety: Strong exception safety.
		*
		* @param	key				The key the value is behind.
		* @param	value			The new value.
		* @param	addValue		If true, creates the key value pair if it
		*							doesn't exist.
		*
		* @return	One of the mmcp::ReturnCodes values.
		*/
		mmcp::ReturnCodes _SetValue(const char key[], const char value[], const bool addValue = true);

		mmcp::ReturnCodes _SetValue(const char key[], const char value, const bool addValue = true)					{ return __SetValue(key, value, addValue, '1'); }
		mmcp::ReturnCodes _SetValue(const char key[], const signed char value, const bool addValue = true)			{ return __SetValue(key, value, addValue, '2'); }
		mmcp::ReturnCodes _SetValue(const char key[], const unsigned char value, const bool addValue = true)		{ return __SetValue(key, value, addValue, '3'); }
		mmcp::ReturnCodes _SetValue(const char key[], const bool value, const bool addValue = true)					{ return __SetValue(key, value, addValue, '4'); }
		mmcp::ReturnCodes _SetValue(const char key[], const short value, const bool addValue = true)				{ return __SetValue(key, value, addValue, '5'); }
		mmcp::ReturnCodes _SetValue(const char key[], const unsigned short value, const bool addValue = true)		{ return __SetValue(key, value, addValue, '6'); }
		mmcp::ReturnCodes _SetValue(const char key[], const int value, const bool addValue = true)					{ return __SetValue(key, value, addValue, '7'); }
		mmcp::ReturnCodes _SetValue(const char key[], const unsigned int value, const bool addValue = true)			{ return __SetValue(key, value, addValue, '8'); }
		mmcp::ReturnCodes _SetValue(const char key[], const long value, const bool addValue = true)					{ return __SetValue(key, value, addValue, '9'); }
		mmcp::ReturnCodes _SetValue(const char key[], const unsigned long value, const bool addValue = true)		{ return __SetValue(key, value, addValue, 'a'); }
		mmcp::ReturnCodes _SetValue(const char key[], const long long value, const bool addValue = true)			{ return __SetValue(key, value, addValue, 'b'); }
		mmcp::ReturnCodes _SetValue(const char key[], const unsigned long long value, const bool addValue = true)	{ return __SetValue(key, value, addValue, 'c'); }
		mmcp::ReturnCodes _SetValue(const char key[], const float value, const bool addValue = true)				{ return __SetValue(key, value, addValue, 'd'); }
		mmcp::ReturnCodes _SetValue(const char key[], const double value, const bool addValue = true)				{ return __SetValue(key, value, addValue, 'e'); }
		mmcp::ReturnCodes _SetValue(const char key[], const long double value, const bool addValue = true)			{ return __SetValue(key, value, addValue, 'f'); }

		/**
		* Closes down the current loaded configuration.
		*
		* Public method that closes down the current loaded
		* configuration and releases all the used resources,
		* also setting them to zero.
		*
		* Exception safety: No-throw guarantee.
		*
		* @noreturn
		*/
		void _CloseConfiguration() throw();

		const char*	_GetfilePath() const throw()			{ return _filePath.c_str(); }
		int			_GetMaxKeyLength() const throw()		{ return _maxKeyLength; }
		int			_GetMaxValueLength() const throw()		{ return _maxValueLength; }
		bool		_IsConfigurationLoaded() const throw()	{ return _configurationLoaded; }
	};
#pragma warning(default:4251)
}

#endif // _MM_CONFIGURATION_PARSER_CONFIGURATIONPARSER_H_