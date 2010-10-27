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

#ifndef MIXEROUTPUT_H
#define MIXEROUTPUT_H

#include "SoundOutput.h"

#include <iostream>
#include <stdint.h>

typedef unsigned char byte;

class MixerOutput
{
public:
  MixerOutput(const std::string &device);
  ~MixerOutput();
  inline void playSound(const Sample *sound);
  inline void getSound(byte *buffer, unsigned int size);
  void open();
  void close();

  unsigned int getBufferUnderruns();
  unsigned int getBufferOverflows();
  unsigned int getBufferSize();
  unsigned int getStoredBytes();

public: // TODO PRIVATE
  inline unsigned int add(byte *buffer, unsigned int size);
  inline unsigned int fetch(byte *buffer, unsigned int size);
  inline void mix(byte *dst, byte *src, unsigned int size);
  inline void clean(unsigned int size);

  unsigned int m_formatBits;

  unsigned int m_bufferUnderruns;
  unsigned int m_bufferOverflows;
  unsigned int m_storedBytes;
  unsigned int m_pointer;
  unsigned int m_bufferSize;
  byte *m_buffer;

  pthread_mutex_t m_bufferMutex;
};

inline void MixerOutput::playSound(const Sample *sample)
{
  std::cout << "Playing sound " << sample->getName() << std::endl;
  pthread_mutex_lock(&m_bufferMutex);
  this->add(reinterpret_cast<byte*>(sample->getData()), sample->getSize());
  pthread_mutex_unlock(&m_bufferMutex);
}

inline void MixerOutput::getSound(byte *buffer, unsigned int size)
{
  pthread_mutex_lock(&m_bufferMutex);
  this->fetch(buffer, size);
  pthread_mutex_unlock(&m_bufferMutex);
}

inline unsigned int MixerOutput::add(byte *buffer, unsigned int size)
{
  if (m_buffer == 0) {
    throw AnnoymeException("MixerOutput has not been opened yet.");
  }

  if (size == 0) {
    return size;
  }

  pthread_mutex_lock(&m_bufferMutex);

  if (size > m_bufferSize) {
    m_bufferOverflows++;
    size = m_bufferSize;
  }

  if (size > m_storedBytes) {
    this->clean(size);
  }

  // If it is bigger than the space left, wrap around
  if (m_pointer + size > m_bufferSize) {
    unsigned int fit = m_bufferSize - m_pointer;
    mix(m_buffer + m_pointer, buffer,       fit);
    mix(m_buffer            , buffer + fit, size - fit);
  }
  else {
    mix(m_buffer + m_pointer, buffer, size);
  }
  if (size > m_storedBytes) m_storedBytes = size;

  pthread_mutex_unlock(&m_bufferMutex);

  return size;
}

inline unsigned int MixerOutput::fetch(byte *buffer, unsigned int size)
{
  if (m_buffer == 0) {
    throw AnnoymeException("MixerOutput has not been opened yet.");
  }

  pthread_mutex_lock(&m_bufferMutex);

  if (size > m_bufferSize) {
    m_bufferUnderruns++;
  }

  if (size > m_storedBytes) {
    memset(buffer + m_storedBytes, 0, size - m_storedBytes);
    size = m_storedBytes;
  }

  // If the requested amount is bigger than the space left, wrap around
  if (m_pointer + size >= m_bufferSize) {
    unsigned int fit = m_bufferSize - m_pointer;
    memcpy(buffer,        m_buffer + m_pointer, fit);
    memcpy(buffer + fit,  m_buffer,             size - fit);
    m_pointer = size - fit;
  }
  else {
    memcpy(buffer, m_buffer + m_pointer, size);
    m_pointer += size;
  }

  m_storedBytes -= size;

  pthread_mutex_unlock(&m_bufferMutex);

  return size;
}

inline void MixerOutput::mix(byte *dst, byte *src, unsigned int size)
{
  int s1 = 0;
  int s2 = 0;
  byte *dstt = dst;
  byte *srct = src;
  unsigned int formatBytes = m_formatBits >> 3; // div by 8
  for (unsigned int i = 0; i < size; i += formatBytes)
  {
    if (m_formatBits == 16) {
      s1 = (int)*(int16_t*)dstt;
      s2 = (int)*(int16_t*)srct;
      s1 += s2;
      *(int16_t*)dstt = (int16_t)s1;
    }
    dstt += formatBytes;
    srct += formatBytes;
  }
}

inline void MixerOutput::clean(unsigned int size)
{
  unsigned int from   = m_pointer + m_storedBytes;
  unsigned int amount = size - m_storedBytes;
  from %= m_bufferSize;
  if (from + amount > m_bufferSize) {
    unsigned int fit = m_bufferSize - from;
    memset(m_buffer + from, 0, fit);
    memset(m_buffer, 0, amount - fit);
  }
  else {
    memset(m_buffer + from, 0, amount);
  }
}

#endif // MIXEROUTPUT_H
