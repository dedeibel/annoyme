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

#include <cstdlib>
#include <cstring>
#include <string>
#include <map>
#include <vector>

using namespace std;

#include "exceptions.h"

#include "Configuration.h"
#include "BasicConfiguration.h"
#include "AggregateConfiguration.h"
#include "ConfigurationMap.h"
#include "SystemConfiguration.h"
#include "YAMLConfig.h"
#include "AnnoymeConfiguration.h"

/* Created by cmake */
#include "config.h"

#include <iostream>

AnnoymeConfiguration *AnnoymeConfiguration::m_annoymeConfiguration = 0;

AnnoymeConfiguration* AnnoymeConfiguration::getInstance()
{
	if (m_annoymeConfiguration == 0) {
		throw AnnoymeException(
				"Config not initialized, call iniWithBinaryPath before anything else.");
	}
	return m_annoymeConfiguration;
}

std::string AnnoymeConfiguration::value(const std::string &path)
{
	return AnnoymeConfiguration::getInstance()->get(path);
}

void AnnoymeConfiguration::initWithBinaryPath(const std::string &binary_path)
{
	m_annoymeConfiguration = new AnnoymeConfiguration();
	m_annoymeConfiguration->init(binary_path);
}

AnnoymeConfiguration::AnnoymeConfiguration() :
	m_buildConfig(new ConfigurationMap()), m_yamlConfig(new YAMLConfig()),
			m_configs(new AggregateConfiguration())
{
}

AnnoymeConfiguration::~AnnoymeConfiguration()
{
	delete m_configs;
	delete m_yamlConfig;
	delete m_buildConfig;
}

void AnnoymeConfiguration::init() throw (AnnoymeException)
{
	this->init("");
}

void AnnoymeConfiguration::init(const string& binary_path)
		throw (AnnoymeException)
{
	m_buildConfig->setNormalized("config_name", ANNOYME_CONFIG_NAME);
	m_buildConfig->setNormalized("prefix", ANNOYME_INSTALL_PATH);
	m_buildConfig->setNormalized("shared_path", ANNOYME_SHARED_PATH);
	m_buildConfig->setNormalized("resource_dir", ANNOYME_RESOURCE_DIRECTORY);
	m_buildConfig->setNormalized("samples_dir", ANNOYME_SAMPLE_DIRECTORY);
	m_buildConfig->setNormalized("shared_directory", ANNOYME_SHARED_DIRECTORY);

	/* Singleton, therefore mem not managed by the annoyme configuration class */
	Configuration *sys = SystemConfiguration::getInstance();
	string yamlPath = sys->getNormalized("system.home") + "/"
			+ ANNOYME_CONFIG_NAME;

	m_buildConfig->setNormalized("dynamic_prefix", getDynamicPrefix(
			sys->getNormalized("system.pwd"), binary_path));

	m_yamlConfig->setConfigFilePath(yamlPath);
	try {
		m_yamlConfig->init();
	}
	catch (AnnoymeException e) {
		m_configs->addConfig(m_buildConfig);
		m_configs->addConfig(sys);
		throw;
	}

	/* For installed version */
	m_buildConfig->setNormalized("resource_path", m_buildConfig->getNormalized(
			"shared_path") + "/" + m_buildConfig->getNormalized("resource_dir"));

	/* For dev version */
	m_buildConfig->setNormalized("dynamic_resource_path",
			m_buildConfig->getNormalized("dynamic_prefix") + "/"
					+ m_buildConfig->getNormalized("resource_dir"));

	/* For locally extracted tar */
	m_buildConfig->setNormalized("packaged_resource_path",
			m_buildConfig->getNormalized("dynamic_prefix") + "/"
					+ m_buildConfig->getNormalized("shared_directory") + "/"
					+ m_buildConfig->getNormalized("resource_dir"));

	m_configs->addConfig(m_buildConfig);
	m_configs->addConfig(sys);
	m_configs->addConfig(m_yamlConfig);
}

std::string AnnoymeConfiguration::getNormalized(const std::string &path)
		throw (UnknownOptionException)
{
	return m_configs->getNormalized(path);
}

std::string AnnoymeConfiguration::getDynamicPrefix(const std::string &pwd,
		const std::string &binary_path) const
{
	// TODO write properly and use unit testing ...

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

		const std::string searchString = "/./";
		const std::string replaceString = "/";
		std::string::size_type pos = 0;
		while ((pos = path.find(searchString, pos)) != string::npos) {
			path.replace(pos, searchString.size(), replaceString);
			pos++;
		}
		/* Path without the binary, basename so to speak */
		path = path.substr(0, path.find_last_of("/"));
		std::cout << "using PATH1: " << path << std::endl;
		/* One up */
		path = path.substr(0, path.find_last_of("/"));
		std::cout << "using PATH2: " << path << std::endl;
		return path;
	}
}
