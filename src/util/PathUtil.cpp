#include <string>
#include <cstring>
using namespace std;

#include <cerrno>
#include "exceptions.h"

#include "util/PathUtil.h"

extern "C"
{
#include <limits.h>
}
#include <cstdlib>

PathUtil::PathUtil()
{

}

PathUtil::~PathUtil()
{

}

std::string PathUtil::getDynamicPrefix(const std::string &pwd,
		const std::string &binary_path) const
{
	// If binary path is empty or not containing a directory, return pwd
	if (binary_path.empty() || binary_path.find("/") == binary_path.npos) {
		return pwd;
	}
	else {
		std::string path;
		if (binary_path.substr(0, 1).compare("/") == 0) {
			path = binary_path;
		}
		else {
			/*
			 * Build the complete path pwd + binary path, then go one up
			 */
			path = pwd + "/" + binary_path;
		}

		path = normalizePath(path);
		/* Path without the binary, basename so to speak */
		path = path.substr(0, path.find_last_of("/"));
		/* One up */
		path = path.substr(0, path.find_last_of("/"));
		return path;
	}
}

std::string PathUtil::normalizePath(const std::string &path) const
		throw (AnnoyErrnoException)
{
	char resolved[PATH_MAX];
	if (realpath(path.c_str(), resolved) == 0) {
		throw AnnoyErrnoException("Could not normalize path", path, errno);
	}
	return std::string(resolved);
}
