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
#include <sstream>
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

template <class T>
bool from_string(T& t, 
                 const std::string& s, 
                                  std::ios_base& (*f)(std::ios_base&))
{
    std::istringstream iss(s);
    return !(iss >> f >> t).fail();
}

AnnoymeConfiguration *AnnoymeConfiguration::m_annoymeConfiguration = 0;

AnnoymeConfiguration* AnnoymeConfiguration::getInstance()
{
  if (m_annoymeConfiguration == 0) {
    m_annoymeConfiguration = new AnnoymeConfiguration();
    m_annoymeConfiguration->init();
  }
  return m_annoymeConfiguration;
}

const std::string AnnoymeConfiguration::value(const std::string &path) {
  return AnnoymeConfiguration::getInstance()->get(path);
}

const int AnnoymeConfiguration::intValue(const std::string &path) throw (InvalidValueException) {
  int i;
  if (from_string<int>(i, AnnoymeConfiguration::getInstance()->get(path), std::dec)) {
    return i;
  }
  throw InvalidValueException(path + " value: "+ AnnoymeConfiguration::getInstance()->get(path));
}

AnnoymeConfiguration::AnnoymeConfiguration()
: m_buildConfig(new ConfigurationMap())
, m_yamlConfig(new YAMLConfig())
, m_configs(new AggregateConfiguration())
{
}

AnnoymeConfiguration::~AnnoymeConfiguration()
{
  delete m_configs;
  delete m_yamlConfig;
  delete m_buildConfig;
}

void AnnoymeConfiguration::init() throw(AnnoymeException)
{

  m_buildConfig->setNormalized("base_directory",        ANNOYME_INSTALL_DIRECTORY);
  m_buildConfig->setNormalized("resource_directory",    ANNOYME_RESOURCE_DIRECTORY);
  m_buildConfig->setNormalized("sample_base_directory", ANNOYME_SAMPLE_DIRECTORY);
  m_buildConfig->setNormalized("config_name",           ANNOYME_CONFIG_NAME);

  Configuration *sys = SystemConfiguration::getInstance();
  string yamlPath = sys->getNormalized("system.home")
                  + sys->getNormalized("system.dir_separator")
                  + ANNOYME_CONFIG_NAME;

  m_yamlConfig->setConfigFilePath(yamlPath);
  try {
    m_yamlConfig->init();
  }
  catch (AnnoymeException e) {
    m_configs->addConfig(m_buildConfig);
    m_configs->addConfig(sys);
    throw;
  }

  m_buildConfig->setNormalized("sample_directory",
                                 m_buildConfig->getNormalized("sample_base_directory")
                                 + sys->getNormalized("system.dir_separator")
                                 + m_yamlConfig->getNormalized("sample_theme"));

  m_configs->addConfig(m_buildConfig);
  m_configs->addConfig(sys);
  m_configs->addConfig(m_yamlConfig);
}

const std::string AnnoymeConfiguration::getNormalized(const std::string &path)
throw(UnknownOptionException)
{
  return m_configs->getNormalized(path);
}

