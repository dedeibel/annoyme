/*
 * Copyright (c) 2009, Benjamin Peter <BenjaminPeter@arcor.de>
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
#include <cstring>
#include <cstdio>
#include <cerrno>
#include <algorithm>
#include <map>
#include <fstream>
#include <vector>

using namespace std;

#include "exceptions.h"
#include "util/FileUtil.h"

#include "config/Configuration.h"
#include "config/BasicConfiguration.h"
#include "config/SystemConfiguration.h"
#include "config/YAMLConfig.h"
#include "config.h"

// yaml-cpp
#include "yaml-cpp/yaml.h"

YAMLConfig::YAMLConfig(const string &resourcePath, const string &configFileName) :
	m_resourcePath(resourcePath), m_configFilePath(configFileName), m_fileUtil(
			new FileUtil())
{

}

YAMLConfig::YAMLConfig(const string &resourcePath,
		const string &configFileName, FileUtil* fileUtil) :
	m_resourcePath(resourcePath), m_configFilePath(configFileName), m_fileUtil(
			fileUtil)
{

}

YAMLConfig::~YAMLConfig()
{
	delete (m_fileUtil);
}

string YAMLConfig::getNormalized(const std::string &path)
		throw (UnknownOptionException)
{
	map<string, string>::iterator it = m_values.find(path);
	if (it != m_values.end()) {
		return it->second;
	}
	else {
		throw UnknownOptionException(path);
	}
}

void YAMLConfig::init() throw (FileNotFoundException, AnnoymeException)
{
	if (access(m_configFilePath.c_str(), R_OK)) {
		createDefault();
	}

	ifstream fin(m_configFilePath.c_str());
	if (!fin) {
		throw FileNotFoundException(m_configFilePath,
				"Application configuration file");
	}
	YAML::Parser parser(fin);
	YAML::Node doc;
	parser.GetNextDocument(doc);

	for (YAML::Iterator it = doc.begin(); it != doc.end(); ++it) {
		string key;
		string value;
		it.first() >> key;
		it.second() >> value;
		m_values[key] = value;
	}
}

void YAMLConfig::setConfigFilePath(const std::string &path)
{
	m_configFilePath = path;
}

std::string YAMLConfig::getConfigFilePath() const
{
	return m_configFilePath;
}

void YAMLConfig::setResourcePath(const std::string &path)
{
	m_resourcePath = path;
}

std::string YAMLConfig::getResourcePath() const
{
	return m_resourcePath;
}

void YAMLConfig::createDefault() throw (AnnoymeException)
{
	std::stringstream source;
	source << m_resourcePath << "/" << ANNOYME_DEFAULT_CONFIG_NAME;
	bool ret = m_fileUtil->copy(source.str(), m_configFilePath);
	if (!ret) {
		char *errno_message = strerror(errno);
		std::stringstream message;
		message << "Could not create configuration file " << "'"
				<< m_configFilePath << "'" << " from default file " << "'"
				<< source.str() << "', error: " << errno_message;
		throw AnnoymeException(message.str());
	}
}
