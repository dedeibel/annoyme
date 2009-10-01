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

#include "Configuration.h"
#include "BasicConfiguration.h"
#include "AnnoymeConfiguration.h"

#include "SystemConfiguration.h"
#include "YAMLConfig.h"

/* Created by cmake */
#include "config.h" 

using namespace std;

#include "exceptions.h"


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

AnnoymeConfiguration::AnnoymeConfiguration()
: m_yamlConfig(0)
{

}

AnnoymeConfiguration::~AnnoymeConfiguration()
{
  if (m_yamlConfig != 0) {
    delete m_yamlConfig;
  }
}

void AnnoymeConfiguration::init()
{
  Configuration *sys = SystemConfiguration::getInstance();

  m_values["base_directory"]        = ANNOYME_INSTALL_DIRECTORY;
  m_values["config_name"]           = ANNOYME_CONFIG_NAME;

  string yamlPath = sys->getNormalized("system.home")
                  + sys->getNormalized("system.dir_separator")
                  + ANNOYME_CONFIG_NAME;

  m_yamlConfig = new YAMLConfig(yamlPath);
  m_yamlConfig->init();

  m_values["sample_directory_base"] = m_values["base_directory"]
                                      + sys->getNormalized("system.dir_separator")
                                      + "pcm";

  m_values["sample_directory"] = m_values["sample_directory_base"]
                                 + sys->getNormalized("system.dir_separator")
                                 + m_yamlConfig->getNormalized("sample_theme");
}

const std::string AnnoymeConfiguration::getNormalized(const std::string &path)
{
  map<string, string>::iterator value = m_values.find(path);
  if (value != m_values.end()) {
    return value->second;
  }

  try {
    return SystemConfiguration::getInstance()->getNormalized(path);
  }
  catch (const UnknownOptionException &ex) {
    // Ignore
  }

  if (m_yamlConfig != 0) {
    try {
      return m_yamlConfig->getNormalized(path);
    }
    catch (const UnknownOptionException &ex) {
      // Ignore
    }
  }

  throw UnknownOptionException(path);
}

