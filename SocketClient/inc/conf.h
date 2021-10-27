#ifndef INC_CONF_H_
#define INC_CONF_H_

#include <apr_general.h>
#include <apr_pools.h>
#include <apr_file_io.h>
#include <apr_strings.h>
#include <apr_network_io.h>
#include <apr_thread_proc.h>
#include <apr_portable.h>

#include <bio.h>
#include <ssl.h>
#include <err.h>

#ifdef DEFINECOLORS
	#define FBLACK      "\e[1;30m"
	#define FRED        "\e[1;31m"
	#define FGREEN      "\e[1;32m"
	#define FYELLOW     "\e[1;33m"
	#define FBLUE       "\e[1;34m"
	#define FPURPLE     "\e[1;35m"
	#define D_FGREEN    "\e[1;6m"
	#define FWHITE      "\e[1;7m"
	#define FCYAN       "\e[1;36m"
	#define NONE        "\e[1;0m"
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
#endif

namespace RLMServerMode
{
	enum serverMode { SSLMODE, NOSSLMODE};
}

namespace RLMClient
{
	class ClientHandler;

	struct threadHandler
	{
		apr_socket_t 					*ns;
		void 							(*fp)(char*);
		SSL								*ssl;
		ClientHandler					*ch;
		RLMServerMode::serverMode		serverMode;
	};
}

#endif
