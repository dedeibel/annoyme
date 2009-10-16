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
  #include <ao/ao.h>
}

using namespace std;

#include "exceptions.h"
#include "Sample.h"

#include "AOutput.h"


AOutput::AOutput(const std::string &)
{

}

AOutput::~AOutput()
{

}

void AOutput::playSound(const Sample *sample)
{
  std::cout << "Playing sound " << sample->getName() << std::endl;
  ao_play(m_device, sample->getData(), sample->getSize());
}

void AOutput::open()
{
  ao_initialize();
	
  /* -- Setup for default driver -- */
  int default_driver = ao_default_driver_id();
  
  ao_sample_format format;
  format.bits = 16;
  format.channels = 1;
  //format.rate = 11025;
  format.rate = 22050;
  format.byte_format = AO_FMT_LITTLE;
  
  /* -- Open driver -- */
  m_device = ao_open_live(default_driver, &format, NULL /* no options */);
  if (m_device == 0) {
  	throw SoundOutputException("AOutput: Error opening device.");
  }
}

void AOutput::close()
{
  ao_shutdown();
}

