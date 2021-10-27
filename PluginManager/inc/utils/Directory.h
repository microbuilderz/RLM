#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <apr-1.0/apr.h>
#include <apr-1.0/apr_file_info.h>
#include <string>
#ifdef PF_PLATFORM_LINUX
#include <dirent.h>
#include <sys/stat.h>
#endif

class Path;
  
namespace Directory
{
	bool							exists(const std::string & path);
	std::string						getCWD();
	void							setCWD(const std::string & path);
	void							copyTree(const std::string & source, const std::string & destination);
	void							removeTree(const std::string & path);
	void							create(const std::string & path);
	struct Entry
#ifndef PF_PLATFORM_LINUX
					: public apr_finfo_t
#endif
	{
		enum Type { FILE, DIRECTORY, LINK };

		Type						type;
		std::string					path;
	};

	class Iterator
	{
	public:

										Iterator(const Path & path);
										Iterator(const std::string & path);
										~Iterator();

		void							reset();
		Entry							*next(Entry & e);

	private:
										Iterator();
										Iterator(const Iterator &);

		void							init(const std::string & path);

		std::string						path_;
#ifdef PF_PLATFORM_LINUX
		DIR 							*handle_;
#else
		apr_dir_t 						*handle_;
		apr_pool_t 						*pool_;
#endif
	};
}

#endif


