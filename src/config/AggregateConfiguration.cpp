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
#include <vector>
#include <algorithm>

using namespace std;

#include "exceptions.h"

#include "config/Configuration.h"
#include "config/BasicConfiguration.h"
#include "config/AggregateConfiguration.h"

#include "STLHelpers.h"

std::string AggregateConfiguration::getNormalized(const std::string &path)
throw(UnknownOptionException)
{
  std::vector<Configuration*>::iterator entry = m_configs.begin();
  while (entry != m_configs.end()) {
    try {
      return (*entry)->getNormalized(path);
    }
    catch (UnknownOptionException ex) {
      // try next one
    }
    ++entry;
  }

  throw UnknownOptionException(path);
}

void AggregateConfiguration::clear()
{
  m_configs.clear();
}

void AggregateConfiguration::addConfig(Configuration *config)
{
  m_configs.push_back(config);
}

void AggregateConfiguration::removeConfig(Configuration *config)
{
  std::vector<Configuration*>::iterator entry;
  entry = std::find(m_configs.begin(), m_configs.end(), config);
  m_configs.erase(entry);
}
