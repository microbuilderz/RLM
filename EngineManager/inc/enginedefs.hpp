#include <string>
#include <iostream>
#include <apr-1.0/apr_general.h>
#include <apr-1.0/apr_thread_proc.h>

#include <PEngineManager.h>
#include <pluginFramework/PluginManager.h>
#include <utils/Path.h>
#include <multithread/NoSSLServer.h>
#include <multithread/SSLServer.h>
#include <multiplex/NoSSLServer.h>
#include <multiplex/SSLServer.h>


#define SSLPASS 				"th3matr1x"
#define SSLKEYPATH 				"./RLMkey.pem"
#define SSLCERTPATH 			"./RLMcert.pem"
#define PORT					8081

apr_pool_t						*memPool;
apr_thread_t 					*server_t;
apr_thread_t 					*scan_t;
char							*pluginPath;

extern RLMEngine::RLMEngineManager *RLMengine;

RLMEngine::PEngineManager PE = RLMEngine::PEngineManager::getInstance();

void							createScanThread(char*);

static void						piped_maintenance(int reason, void *data, apr_wait_t status)
{
	switch (reason)
	{
		case APR_OC_REASON_DEATH:
			printf("Child process is dead\n");
		break;
		case APR_OC_REASON_LOST:
			printf("Child process is lost\n");
		break;
		case APR_OC_REASON_RESTART:
			printf("Child process is restarting\n");
		break;
		case APR_OC_REASON_UNREGISTER:
			printf("Child process is unregistering\n");
		break;
		default:
			printf("default piped_maintenance\n");
		break;
	}
}

int								InitiateProcesses()
{
	apr_proc_t				newproc;
	const char				*programargs[2];
	apr_procattr_t			*procattr;

	apr_status_t ret = apr_procattr_create( &procattr, memPool );
	if (ret!=APR_SUCCESS)
		printf("%sProcess attribute could not be created%s\n",FRED,NONE);

	apr_procattr_dir_set(procattr,"../../SocketServerAPR/Debug/");

	programargs[0]			= "SocketServerAPR.exe";
	programargs[1]			= NULL;

	apr_procattr_detach_set(procattr, 0);
	apr_procattr_cmdtype_set(procattr, APR_PROGRAM_ENV);

	ret = apr_proc_create( &newproc, programargs[0], (const char * const *)programargs,(const char * const *)NULL, procattr, memPool );
	if (ret!=APR_SUCCESS)
		printf("%sProcess not created%s\n",FRED,NONE);
	else
	{
		printf("Process %s created\n",programargs[0]);
		apr_proc_other_child_register(&newproc,piped_maintenance,NULL,NULL,memPool);

		int exitcode;
		apr_exit_why_e status;
		apr_status_t rv = apr_proc_wait_all_procs(&newproc, &exitcode, &status, APR_WAIT, memPool);
		if (APR_STATUS_IS_CHILD_DONE(rv))
			if (apr_proc_other_child_alert(&newproc, APR_OC_REASON_DEATH, status)!= APR_SUCCESS)
				printf("apr_proc_other_child_alert not ok\n");
	}

	return 0;
}

int								_ReadHandlerPtr (void* arg)
{
    char* msg=(char*)arg;
    printf("_ReadHandlerPtr message=%s\n",msg);
    if (msg!=NULL)
        RLMEngine::PEngineManager::getInstance().saveServerMessage(msg);
    createScanThread(pluginPath);
	return 0;
}

int								_pluginFinishLoadCallback(void* arg)
{
    char* msg=(char*)arg;
	printf("%supdating functors %s\n",FYELLOW,NONE);
    int ret=-1;
    if (msg!=NULL)
        ret=RLMEngine::PEngineManager::getInstance().LoadPluginByInfoName(msg);
    printf("%sloaded plugin %d%s\n",FRED,ret,NONE);

    if (ret<0)
	    RLMEngine::PEngineManager::getInstance().setUPLatest();
	return 0;
}

static void *APR_THREAD_FUNC  	startServer(apr_thread_t *thd, void* data)
{
	printf("%sstarting SSL server server %s\n",FYELLOW,NONE);
	RLMSocketServer::MultiPlex::SSLServer *Server=new RLMSocketServer::MultiPlex::SSLServer((char*)SSLPASS,(char*)SSLKEYPATH,(char*)SSLCERTPATH,PORT);
	Server->SetReadHandler( _ReadHandlerPtr);
	Server->RunSocketServer();
	return NULL;
}

void initPluginInstance(RLMEngine::Port rlmPort)
{
	RLMEngine::PEngineManager::getInstance().setInstance(RLMEngine::NO_SSL,RLMEngine::Acceptor,rlmPort);
}

static void	*APR_THREAD_FUNC	startPluginScanner(apr_thread_t *thd, void* data)
{
	char* path=(char*)data;

	printf("%s starting plugin scanner %s\n",FYELLOW,NONE);

	RLMFrameWork::PluginManager &pm = RLMFrameWork::PluginManager::getInstance();
	RLMEngine::PEngineManager::getInstance().setFinishLoadCallback(_pluginFinishLoadCallback);
	pm.loadAll(Path::makeAbsolute(path));
	RLMEngine::PEngineManager::getInstance().scan();

	return NULL;
}

void 							createSocketServerThread()
{
	apr_thread_create(&server_t, NULL, startServer, NULL, memPool);
}

void							createScanThread(char* path)
{
	apr_thread_create(&scan_t, NULL, startPluginScanner, path, memPool);
}
