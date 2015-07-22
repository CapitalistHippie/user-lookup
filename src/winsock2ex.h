// Pass _WINSOCKAPI_ to the preprocessor definitions

#ifndef __MYWINSOCK2_H
#define __MYWINSOCK2_H
#pragma push_macro("_WINSOCKAPI_")
// We clear _WINSOCKAPI_ to avoid preprocessor warnings about
// multiple definitions of the _WINSOCKAPI_ macro, as winsock2.h will
// attempt to #define _WINSOCKAPI_ itself.
#undef _WINSOCKAPI_
#include <WinSock2.h>
#pragma pop_macro("_WINSOCKAPI_")
#endif // __MYWINSOCK2_H