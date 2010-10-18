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

using namespace std;

#include <string>
#include <vector>
#include <cstring>

#include "exceptions.h"

#include "ResourceLoader.h"
#include "FileUtil.h"

#include "config/Configuration.h"
#include "config/BasicConfiguration.h"
#include "config/AnnoymeConfiguration.h"

ResourceLoader::ResourceLoader(FileUtil* fileUtil)
: m_fileUtil(fileUtil)
{
}

ResourceLoader::ResourceLoader()
: m_fileUtil(new FileUtil())
{

}

ResourceLoader::~ResourceLoader()
{
	delete (m_fileUtil);
}

void ResourceLoader::init()
{
	// TODO enforce init being called

	/* Try the absolute defined path fist, defined by build process */
	if (m_fileUtil->isDirectory(AnnoymeConfiguration::value("resource_path"))) {
		m_path = AnnoymeConfiguration::value("resource_path");
	}
	/* Try the relative app prefix path from binary position afterwards */
	else if (m_fileUtil->isDirectory(AnnoymeConfiguration::value(
			"dynamic_resource_path"))) {
		m_path = AnnoymeConfiguration::value("dynamic_resource_path");
	}
	else {
		throw AnnoymeException(
				"Could not determine resource path. Please check your compile parameters and installation paths.");
	}
}

// TODO document methods and about "resource paths" relative to resource dir
bool ResourceLoader::isResourceDirectory(const string &dir)
{
	return m_fileUtil->isDirectory(m_path + AnnoymeConfiguration::value("system.dir_separator") + dir);
}

void ResourceLoader::listResources(const string &dir,
		vector<string> &resources)
{
	m_fileUtil->listFiles(m_path + AnnoymeConfiguration::value("system.dir_separator") + dir, resources);
	vector<string>::iterator it = resources.begin();
	while (it != resources.end()) {
		it->erase(0, m_path.size() + 1);
		++it;
	}
}

string ResourceLoader::getPath(const string &resource)
{
	return m_path + AnnoymeConfiguration::value("system.dir_separator") + resource;
}

void ResourceLoader::getContent(const string &resource, char **data,
		unsigned int *size) throw (FileNotFoundException)
{
	m_fileUtil->loadFile(m_path + AnnoymeConfiguration::value("system.dir_separator") + resource, data, size);
}

