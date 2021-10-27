#include <enginedefs.hpp>

using namespace std;

RLMEngine::RLMEngineManager *RLMengine = NULL;

char*   path    = NULL;
int     RLM_PORT= -1;

void    checkArgs(int argc, char** argv)
{
    if (argc<1)
    {
        cout <<FRED<<"Not enough or invalid arguments, please try again"<<NONE<<endl;
        cout <<FYELLOW<< "Usage : " << endl << "-rl : RLM port \n -p : Plugin path" <<NONE<< endl;
        exit(0);
    }
    for (int i = 1; i < argc; i++)
    {
        if (i + 1 != argc)
        {
            if (argv[i] == "--help")
            {
                cout <<FYELLOW<< "Usage : " << endl << "-rl : RLM port \n -p : Plugin path" <<NONE<< endl;
            }
            else if (strcmp(argv[i],"-rl")==0)
            {
                RLM_PORT = atoi(argv[i + 1]);
                cout<<RLM_PORT<<endl;
                i++;
            }
            else if (strcmp(argv[i],"-p")==0)
            {
                path = argv[i + 1];
                cout<<path<<endl;
                i++;
            }
            else
            {
                cout <<FRED<<"Not enough or invalid arguments, please try again"<<NONE<<endl;
                cout <<FYELLOW<< "Usage : " << endl << "-rl : RLM port \n -p : Plugin path" <<NONE<< endl;
                exit(0);
            }

        }

    }
}


int					main(int argc, char * argv[])
{
	int ret=system("clear");
	if (ret<0)
		printf("%ssystem lib not accessible%s",FRED,NONE);

    checkArgs(argc,argv);

    if (RLM_PORT<0 || path==NULL)
    {
        cout <<FRED<<"Not enough or invalid arguments, please try again"<<NONE<<endl;
        cout <<FYELLOW<< "Usage : " << endl << "-lp : LP port \n -rl : RLM port \n -p : Plugin path" <<NONE<< endl;
        exit(0);
    }

	printf("%s%s%sInit RLM RLM PluginManager%s\n",BOLD,BBLUE,FWHITE,NONE);
	::apr_initialize();

	apr_pool_create(&memPool, NULL);
	pluginPath = apr_psprintf(memPool,"%s",path);

	initPluginInstance(RLM_PORT);

	createSocketServerThread();
	createScanThread(pluginPath);

	apr_status_t status;
	apr_thread_join(&status,server_t);

	::apr_terminate();
	printf("%s%s%sClosed PluginManager%s\n",BOLD,BBLUE,FWHITE,NONE);
	return 0;
}

