/*
 * Copyright (c) 2008, Benjamin Peter <BenjaminPeter@arcor.de>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the author nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY Benjamin Peter ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL Benjamin Peter BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <string>
#include <fstream>

using namespace std;

#include <cerrno>
#include "exceptions.h"
#include "FileUtil.h"

extern "C"
{
#include <sys/types.h>
#include <sys/stat.h>
}

bool FileUtil::isDirectory(const string &path) throw (AnnoyErrnoException)
{
	struct stat buf;
	int retval = stat(path.c_str(), &buf);
	if (retval == -1) {
		throw AnnoyErrnoException("Could not stat directory", path, errno);
	}

	if (S_ISDIR(buf.st_mode)) {
		return true;
	}
	else {
		return false;
	}
}

bool FileUtil::copy(const string &src, const string &dst)
{
	std::ifstream ifs(src.c_str()); // I'll never get why they excpect a cstring
	if (!ifs) {
		return false;
	}

	std::ofstream ofs(dst.c_str());
	if (!ofs) {
		return false;
	}

	// Copy the file
	const unsigned int bufflen = 4068;
	std::streamsize read;
	char buffer[bufflen];

	while ((read = ifs.readsome(buffer, bufflen)) > 0) {
		ofs.write(buffer, read);
	}

	// check for failures
	if (ifs.rdbuf()->in_avail() != 0 || !ofs) {
		unlink(dst.c_str());
		return false;
	}

	return true;
}
