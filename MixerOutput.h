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

typedef unsigned char byte;

class MixerOutput : public virtual SoundOutput
{
public:
  MixerOutput(const std::string &device);
  virtual ~MixerOutput();
  virtual void playSound(const Sample *sound);
  void getSound(byte *buffer, unsigned int size);
  virtual void open();
  virtual void close();

  unsigned int getBufferUnderruns();
  unsigned int getBufferOverflows();
  unsigned int getBufferSize();
  unsigned int getStoredBytes();

public: // TODO PRIVATE
  unsigned int add(byte *buffer, unsigned int size);
  unsigned int fetch(byte *buffer, unsigned int size);
  void mix(byte *dst, byte *src, unsigned int size);
  void clean(unsigned int size);

  unsigned int m_formatBits;

  unsigned int m_bufferUnderruns;
  unsigned int m_bufferOverflows;
  unsigned int m_storedBytes;
  unsigned int m_pointer;
  unsigned int m_bufferSize;
  byte *m_buffer;

  pthread_mutex_t m_bufferMutex;
};

#endif // MIXEROUTPUT_H
