#include <RLMconf.hpp>

typedef std::vector<int>			SuppressedSignals;
SuppressedSignals					suppressedSignals;
sigset_t							signalSet;

void errorAbort (int status, const char* reason)
{
	printf ("Aborted due to status %d: %s\n", status, reason ? reason : "no error message");
	exit (1);
}

void* signalWaiter (void* arg)
{
	int signalNumber;

	while (1)
	{
		sigwait (&signalSet, &signalNumber);
		printf ("Signal %d received and suppressed.\n", signalNumber);
	}
	return NULL;
}

static void sig_pipeclosed(int sig)
{
    //ap_start_shutdown();
}

void manageLinuxSignals()
{
	suppressedSignals.push_back (SIGPIPE);
	pthread_t signalThreadId;
	int status;

	sigemptyset (&signalSet);

	for (SuppressedSignals::const_iterator i = suppressedSignals.begin(), e = suppressedSignals.end();
			i != e; ++i)
	{
		sigaddset (&signalSet, *i);
	}

	status = pthread_sigmask (SIG_BLOCK, &signalSet, NULL);
	if (status != 0)
		errorAbort (status, "Set signal mask");

	status = pthread_create (&signalThreadId, NULL, signalWaiter, NULL);
	if (status != 0)
		errorAbort (status, "Create signalWaiter");

    /*apr_pool_t		    *memPool;
    apr_pool_create(&memPool, NULL);
	apr_sigfunc_t * 	apr_signal (SIGPIPE, SIG_DFL);*/
}
