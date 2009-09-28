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
#include <algorithm>
#include <map>
#include <fstream>

using namespace std;

#include "config.h"
#include "Configuration.h"
#include "YAMLConfig.h"
#include "exceptions.h"

// yaml-cpp
#include "yaml-cpp/yaml.h"

YAMLConfig::YAMLConfig(string configFilePath)
: m_configFilePath(configFilePath)
{

}

YAMLConfig::~YAMLConfig()
{

}

string YAMLConfig::get(string name)
{
  std::transform(name.begin(), name.end(), name.begin(), ::tolower);
  map<string, string>::iterator it = m_values.find(name);
  if (it != m_values.end()) {
    return it->second;
  }
  else {
    throw UnknownOptionException(name);
  }
}

void YAMLConfig::init()
{
  ifstream fin(m_configFilePath.c_str());
  if (!fin) {
    cerr << "bad fin: "<< m_configFilePath << "\n";
    return;
  }
  YAML::Parser parser(fin);
  YAML::Node doc;
  parser.GetNextDocument(doc);

  for (YAML::Iterator it = doc.begin(); it != doc.end(); ++it) {
    string key;
    string value;
    it.first()  >> key;
    it.second() >> value;
    m_values[key] = value;
  }
}

void YAMLConfig::createDefault()
{
  // TODO
}
