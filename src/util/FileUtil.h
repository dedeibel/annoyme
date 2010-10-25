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

#ifndef FILEUTIL_H
#define FILEUTIL_H

class AnnoyErrnoException;
class IllegalArgumentException;

class FileUtil
{
public:
	bool copy(const std::string &src, const std::string &dst) const;
	std::string findFile(const std::string &filename, const std::vector<
			std::string> paths) const throw (FileNotFoundException);
	std::string findFile(const std::string &filename, const std::vector<
			std::string> &paths) const throw (FileNotFoundException);
	void loadFile(const std::string &filename, const vector<string> paths,
			char **data, unsigned int *size) const throw (FileNotFoundException);
	void loadFile(const std::string &path, char** data, unsigned int* size) const
			throw (FileNotFoundException);
	void
	listFiles(const std::string &dirname, std::vector<std::string> &files) const
			throw (IllegalArgumentException);

	/**
	 * Returns true if the directory exists and is a readable directory. If there is a problem accessing the directory, invalid path, not existing, too long, ... an exception is thrown.
	 */
	bool isAccessableDirectory(const std::string &path) const
			throw (AnnoyErrnoException);

	/**
	 * Returns true if the file exists and is readable. If there is a problem accessing the directory, invalid path, not existing, too long, ... an exception is thrown.
	 */
	bool isAccessableFile(const std::string &path) const
			throw (AnnoyErrnoException);

	/**
	 * Returns true if the directory exists and is a directory.
	 */
	bool isDirectory(const std::string &path) const;

	/**
	 * Returns true if the file exists and is a file.
	 */
	bool isFile(const std::string &path) const;

	bool isReadable(const std::string &path) const;
};

#endif // FILEUTIL_H
