#pragma once

#include <OnixS/FIXEngine.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <cstdarg>
#include <cstdlib>
#include <csignal>
#include <pthread.h>
#include <queue>
#include <unistd.h>
#include <exception>

using namespace std;
using namespace OnixS::FIX;
using namespace OnixS::FIX::FIX44;

const ProtocolVersion::Enum fixProtocolVersion = ProtocolVersion::FIX_44;

#ifdef DEFINECOLORS
	#define FBLACK      "\e[1;30m"
	#define FRED        "\e[1;31m"
	#define FGREEN      "\e[1;32m"
	#define FYELLOW     "\e[1;33m"
	#define FBLUE       "\e[1;34m"
	#define FPURPLE     "\e[1;35m"
	#define FCYAN       "\e[1;36m"
	#define D_FGREEN    "\e[1;6m"
	#define FWHITE      "\e[1;7m"

	#define NONE        "\e[1;0m"

	#define BBLACK      "\e[1;40m"
	#define BRED        "\e[1;41m"
	#define BGREEN      "\e[1;42m"
	#define BYELLOW     "\e[1;43m"
	#define BBLUE       "\e[1;44m"
	#define BPURPLE     "\e[1;45m"
	#define BCYAN       "\e[1;46m"
	#define BWHITE      "\e[1;47m"

	#define BOLD		"\e[1;1m"

#else
	#define FBLACK      ""
	#define FRED        ""
	#define FGREEN      ""
	#define FYELLOW     ""
	#define FBLUE       ""
	#define FPURPLE     ""
	#define D_FGREEN    ""
	#define FWHITE      ""
	#define FCYAN       ""
	#define NONE        ""

	#define BBLACK      ""
	#define BRED        ""
	#define BGREEN      ""
	#define BYELLOW     ""
	#define BBLUE       ""
	#define BPURPLE     ""
	#define BCYAN       ""
	#define BWHITE      ""

	#define BOLD		""
#endif



namespace RLMEngine
{
	typedef int 		Port;
	const Port			DEFAULT_SSL_PORT	= 2442;						///< Default Engine SSL port
	const Port			DEFAULT_PORT		= 4500;						///< Default Engine port
	enum				RLMMode { Acceptor, Initiator, Both };			///< Engine modes
	enum				NetworkConnectionType { NO_SSL, SSL };			///< Engine connection types
}

namespace RLMEngine
{
	/**
	 * Side Info structure needed by OnixS initialization
	 */
	struct SideInfo
	{
        SideInfo(RLMEngine::Port port,std::string side)
        {
            SidePort = port;
            SideName = side;
        }
		RLMEngine::Port     SidePort;							///< Side Port
		std::string         SideName;							///< Side Name
	};
	/**
	 * listener structure used by \ref RLMlistenerFunctor
	 */
	struct RLMListenerStruct
	{
		Session					*sn;										///< OnixS Session
		Message					*msg;										///< OnixS Message
		ISessionListener		*listener;									///< OnixS SessionListener
	};

	/**
		 * listener structure used by \ref RLMlistenerFunctor
		 * onInit Function pointer
	*/
	struct RLMlistenerFunctor
	{
		int (*onInit)(RLMListenerStruct *);						///< onInit function pointer .
		int (*onInboundApplicationMsg)(RLMListenerStruct *);	///< onInboundApplicationMsg function pointer .
		int (*onInboundSessionMsg)(RLMListenerStruct *);		///< onInboundSessionMsg function pointer .
		int (*onStateChangePtr)(RLMListenerStruct *);			///< onStateChangePtr function pointer .
	};
}

/**
 * Wrapper of the c snprintf function with variadics
*/
inline int xsnprintf (char* buffer, size_t size, const char* format, ...)
{
    va_list list;
    va_start (list, format);

    return snprintf (buffer, size, format, list);
}

#ifdef _ENABLE_OPTIM__
#ifndef STR2INT__
#define STR2INT__
typedef int str2int(const char *p);

/**
 * Very fast string to int converter
*/
static int naive_strtoint(const char *p) {
    int x = 0;
    while (*p >= '0' && *p <= '9') {
        x = (x*10) + (*p - '0');
        ++p;
    }
    return x;
}

str2int naive_strtoint;
#endif
#endif


