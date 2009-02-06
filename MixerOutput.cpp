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
#include <iostream>
#include <sstream>
#include <cstring>

extern "C" {
  #include <pthread.h>
  #include <limits.h>
}

using namespace std;

#include "exceptions.h"
#include "Sample.h"

#include "MixerOutput.h"

MixerOutput::MixerOutput(const std::string &param)
: m_formatBits(16)
, m_bufferSize(0)
, m_buffer(0)
{
  pthread_mutex_init(&m_bufferMutex, NULL);

  std::istringstream is(param);
  is >> m_bufferSize;
  is >> m_formatBits;

  if (m_bufferSize == 0) {
    std::cerr << "Buffer size must specified.\n";
    return;
  }

  if (m_formatBits == 0) {
    std::cerr << "The frame bit size must be specified.\n";
    return;
  }
  
  if (m_bufferSize % m_formatBits != 0) {
    std::cerr << "Buffer size must be a multiple of formatBits.\n";
  }
}

MixerOutput::~MixerOutput()
{
  pthread_mutex_lock(&m_bufferMutex);
  pthread_mutex_unlock(&m_bufferMutex);
  pthread_mutex_destroy(&m_bufferMutex);
}

void MixerOutput::open()
{
  pthread_mutex_lock(&m_bufferMutex);
  m_bufferUnderruns = 0;
  m_bufferOverflows = 0;
  m_storedBytes     = 0;
  m_pointer         = 0;
  m_buffer    = new byte[m_bufferSize];
  pthread_mutex_unlock(&m_bufferMutex);
}

void MixerOutput::close()
{
  pthread_mutex_lock(&m_bufferMutex);
  delete[] m_buffer;
  m_buffer = 0;
  pthread_mutex_unlock(&m_bufferMutex);
}

unsigned int MixerOutput::getBufferUnderruns()
{
  return m_bufferUnderruns;
}

unsigned int MixerOutput::getBufferOverflows()
{
  return m_bufferOverflows;
}

unsigned int MixerOutput::getBufferSize()
{
  return m_bufferSize;
}

unsigned int MixerOutput::getStoredBytes()
{
  return m_storedBytes;
}
