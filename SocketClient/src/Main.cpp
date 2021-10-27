#include "ClientHandler.h"

#define PORT				8081
#define TIMEOUT_IN_SEC		30

int main(int argc, const char *argv[])
{
	::apr_initialize();
	RLMClient::ClientHandler * handler;
#ifdef CLIENTSSLMODE
	handler = new RLMClient::ClientHandler((char*)ENGINEHOSTNAME,PORT,TIMEOUT_IN_SEC,RLMServerMode::SSLMODE);
#endif
#ifdef CLIENTAPRMODE
	handler = new RLMClient::ClientHandler((char*)ENGINEHOSTNAME,PORT,TIMEOUT_IN_SEC,RLMServerMode::SSLMODE);
#endif
	if (handler->Connect()!=APR_SUCCESS)
		printf("Client connect error\n");
	if (handler->do_client_task() != APR_SUCCESS)
		 printf("Error doing client tasks\n");
	delete (handler);
	::apr_terminate();
}

