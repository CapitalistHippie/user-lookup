#include "configuration.h"

namespace uslo
{
	extern bool			isWindowMaximized;		// Speaks for itself.

	extern int			mainWindowXCoordinate;	// X coordinate of the main window on the screen.
	extern int			mainWindowYCoordinate;	// Y coordinate of the main window on the screen.

	extern int			mainWindowClientWidth;	// Width of the main window client area.
	extern int			mainWindowClientHeight;	// Length of the main window client area.

	extern char			userToFind[MAX_USERNAME_LENGTH];
	extern char			domainToSearch[MAX_NETBIOS_DOMAIN_NAME_LENGTH];

	namespace
	{
		const static char CONFIGURATION_FILE_PATH[] = "config.txt";

		//static mmcp::ConfigurationParser configurationParser;
	}
}

void uslo::LoadConfiguration()
{
	/*configurationParser._LoadConfiguration(CONFIGURATION_FILE_PATH);

	configurationParser._GetValue("mainWindowXCoordinate", &mainWindowXCoordinate, DEFAULT_MAIN_WINDOW_X_COORDINATE);
	configurationParser._GetValue("mainWindowYCoordinate", &mainWindowYCoordinate, DEFAULT_MAIN_WINDOW_Y_COORDINATE);

	configurationParser._GetValue("usernameEditControlText", usernameEditControlText, sizeof(usernameEditControlText), DEFAULT_EDIT_CONTROL_TEXT);
	configurationParser._GetValue("domainEditControlText", domainEditControlText, sizeof(domainEditControlText), DEFAULT_EDIT_CONTROL_TEXT);
	configurationParser._GetValue("queryTimeoutEditControlText", queryTimeoutEditControlText, sizeof(queryTimeoutEditControlText), DEFAULT_QUERY_TIMEOUT_EDIT_CONTROL_TEXT);
	configurationParser._GetValue("hostnameFilterEditControlText", hostnameFilterEditControlText, sizeof(hostnameFilterEditControlText), DEFAULT_EDIT_CONTROL_TEXT);
*/}

void uslo::SaveConfiguration()
{
	/*configurationParser._SetValue("mainWindowXCoordinate", mainWindowXCoordinate);
	configurationParser._SetValue("mainWindowYCoordinate", mainWindowYCoordinate);

	configurationParser._SetValue("usernameEditControlText", usernameEditControlText);
	configurationParser._SetValue("domainEditControlText", domainEditControlText);
	configurationParser._SetValue("queryTimeoutEditControlText", queryTimeoutEditControlText);
	configurationParser._SetValue("hostnameFilterEditControlText", hostnameFilterEditControlText);

	if (configurationParser._IsConfigurationLoaded())
		configurationParser._SaveConfiguration();
*/}