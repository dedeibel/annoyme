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
#include <cstring>

extern "C" {
  #include <alsa/asoundlib.h>
}

using namespace std;

#include "exceptions.h"
#include "Sample.h"

#include "AlsaOutput.h"
#include "MixerOutput.h"
#include "MixedAlsaOutput.h"

MixedAlsaOutput::MixedAlsaOutput(const std::string &device)
: m_soundOutput(new AlsaOutput(device))
{
  
}

MixedAlsaOutput::~MixedAlsaOutput()
{

}

void MixedAlsaOutput::playSound(const Sample *sample)
{
  std::cout << "Playing sound " << sample->getName() << std::endl;
  m_mixer->add(reinterpret_cast<byte*>(sample->getData()), sample->getSize());
  std::cout << "STATS: Size: "<< m_mixer->getStoredBytes() <<
   " Overflows: " << m_mixer->getBufferOverflows() <<
   " Underruns: " << m_mixer->getBufferUnderruns() <<
   endl;
}

void MixedAlsaOutput::open()
{
  m_mixer = new MixerOutput("5242880 16");
  m_mixer->open();

  m_soundOutput->open();

  startThread();
}

void MixedAlsaOutput::close()
{
  m_soundOutput->close();
  stopThread();
}

void MixedAlsaOutput::startThread() throw(AnnoymeException)
{
  int rc;
  rc = pthread_create(&m_thread, NULL, runObject, reinterpret_cast<void*>(this));

  if (rc != 0) {
    throw SoundOutputException("AOutput: An error occured while starting the AOutput thread.");
  }
}

void MixedAlsaOutput::stopThread() throw(AnnoymeException)
{
  pthread_exit(NULL);
}

void MixedAlsaOutput::run()
{
  byte buffer[40000];
  unsigned int bytes_fetched;
  int ret;
  Sample s;
  while (1) {
    /* 2 * samplesize, enough to keep the card buffer filled */
    bytes_fetched = m_mixer->fetch(buffer, 16384<<2);
    s.setSize(bytes_fetched);
    s.setData(reinterpret_cast<char*>(buffer));
    m_soundOutput->playSound(&s);

    /* busy loop, should be improved like everything */
  }
}

void* MixedAlsaOutput::runObject(void *object)
{
  MixedAlsaOutput* out = reinterpret_cast<MixedAlsaOutput*>(object);
  out->run();
  return 0;
}
