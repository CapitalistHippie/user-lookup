#ifndef _USER_LOOKUP_GLOBALCONSTANTS_H_
#define _USER_LOOKUP_GLOBALCONSTANTS_H_

#include <Windows.h>
#include <LMCons.h>

namespace uslo
{
	const static int	USERNAME_EDIT_CONTROL_ID		= 1000;
	const static int	DOMAIN_EDIT_CONTROL_ID			= 1001;

	const static int	SEARCH_BUTTON_CONTROL_ID		= 1002;

	const static int	RESULTS_LIST_VIEW_CONTROL_ID	= 1003;

	const static int	MACHINE_LIST_VIEW_CONTROL_ID	= 1004;

	const static int	MAX_USERNAME_LENGTH				= UNLEN;
	const static int	MAX_NETBIOS_DOMAIN_NAME_LENGTH	= NETBIOS_NAME_LEN;
	const static int	MAX_HOSTNAME_LENGTH				= CNLEN;

	const static int	WM_INSERT_MACHINE				= (WM_USER + 0x0001);
	const static int	WM_INSERT_RESULT				= (WM_USER + 0x0002);
}

#endif // _USER_LOOKUP_GLOBALCONSTANTS_H_