#define SSLPASS 						"th3matr1x"
#define SSLKEYPATH 						"./RLMkey.pem"
#define SSLCERTPATH 					"./RLMcert.pem"
#define PORT							8081

#if MULTITHREADAPR
    #include "multithread/NoSSLServer.h"
#endif
#if MULTITHREADSSL
    #include "multithread/SSLServer.h"
#endif
#if MULTIPLEXAPR
    #include "multiplex/NoSSLServer.h"
#endif
#if MULTIPLEXSSL
    #include "multiplex/SSLServer.h"
#endif

int main()
{
#if MULTITHREADSSL
    RLMSocketServer::MultiThread::SSLServer *Server = new RLMSocketServer::MultiThread::SSLServer((char*)SSLPASS,(char*)SSLKEYPATH,(char*)SSLCERTPATH,PORT);
    Server->RunSocketServer();
#endif
#if MULTITHREADAPR
	RLMSocketServer::MultiThread::NoSSLServer *Server=new RLMSocketServer::MultiThread::NoSSLServer(PORT);
    Server->RunSocketServer();
#endif
#if MULTIPLEXAPR
    RLMSocketServer::MultiPlex::NoSSLServer *Server=new RLMSocketServer::MultiPlex::NoSSLServer(PORT);
    Server->RunSocketServer();
#endif
#if MULTIPLEXSSL
    RLMSocketServer::MultiPlex::SSLServer *Server=new RLMSocketServer::MultiPlex::SSLServer((char*)SSLPASS,(char*)SSLKEYPATH,(char*)SSLCERTPATH,PORT);
    Server->RunSocketServer();
#endif

	return 0;
}
