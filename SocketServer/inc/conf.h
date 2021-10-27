/*
 * conf.h
 *
 *  Created on: Jan 18, 2017
 *      Author: meditation
 */

#ifndef INC_CONF_H_
#define INC_CONF_H_

#define BUFSIZE 4096

#include <apr_general.h>
#include <apr_pools.h>
#include <apr_file_io.h>
#include <apr_strings.h>
#include <apr_network_io.h>
#include <apr_thread_proc.h>
#include <apr_portable.h>
#include <apr_poll.h>
#include <bio.h>
#include <ssl.h>
#include <err.h>
#include <iostream>

#define CRLF_STR	"\r\n"

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

namespace RLMServerMode
{
	enum serverMode { SSLMODE, NOSSLMODE};
}

namespace RLMSocketServer
{

	enum Status 		{
							ServerRunning 		= 0,
							QuitServer			= 1
						};

	enum SocketStatus	{
							SERV_RECV_REQUEST	= 0,
							SERV_SEND_RESPONSE	= 1,
						};

	enum ServerState	{
							VALID				= 0,
							INVALID				= 1
						};
	typedef int ServerStatus;

	struct threadHandler
	{
		apr_socket_t 	*ns;
		void 			(*fp)(char*);
		SSL				*ssl;
	};

	struct _ctx_t
	{
		apr_socket_t 		*sock;

		struct
		{
			const char		*filePath;
		} recv;

		struct
		{
			char			*send_buf;
			apr_off_t		offset;
		} send;
	};
	typedef struct _ctx_t		servCTX;

	struct _SSL_ctx_t : public _ctx_t
	{
		SSL				*sslSock;
	};
	typedef struct _SSL_ctx_t	SSLServCTX;
}
#endif
