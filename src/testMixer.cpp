
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
#include <string>
#include <iostream>

using namespace std;

#include "Event.h"
#include "Sample.h"
#include "StaticConfiguration.h"
#include "InputEventReader.h"
#include "InputEventReaderFactory.h"
#include "SoundOutput.h"
#include "SoundOutputFactory.h"
#include "SoundLoader.h"
#include "SoundLoaderFactory.h"
#include "Annoyme.h"

#include "MixerOutput.h"

int main(int argc, char **argv)
{
  MixerOutput mo = MixerOutput("128 16");
  mo.open();
  cout << " FFFFUL " << (0xFFFFul) << endl;
  cout << "~FFFFUL " << (~0xFFFFul) << endl;
  cout << "testoutput\n";
  unsigned char buff[128] = {0};
  unsigned char buff2[128] = {0};
  *(uint32_t*)buff = 0x00030001u; // haha, don't forget the endianess! :D
  cout << "buff: " << *(uint16_t*)buff << endl;
  cout << "buff: " << *((uint16_t*)buff + 1) << endl;
  mo.add(buff, 4);
  mo.add(buff, 4);
  mo.fetch(buff2, 2);
  cout << "buff2: " << *(uint16_t*)buff2 << endl;
  cout << "b eq 2: " << (*(uint16_t*)buff2 == (uint16_t)2) << endl;
  mo.fetch(buff2, 2);
  cout << "buff2: " << *(uint16_t*)buff2 << endl;
  cout << "b eq 6: " << (*(uint16_t*)buff2 == (uint16_t)6) << endl;

  cout << "\nGetting it all.\n";
  // wrap around test

  mo.fetch(buff2, 128); // empty buff2
  mo.add(buff2, 128);   // fill the buffer once with zeros
  cout << "buff: " << *(uint16_t*)buff << endl;
  cout << "buff: " << *((uint16_t*)buff + 1) << endl;
  mo.add(buff, 4);      // add some values
  mo.add(buff, 4);
  mo.fetch(buff2, 2); // should be the same in here as above
  cout << "buff2: " << *(uint16_t*)buff2 << endl;
  cout << "b eq 2: " << (*(uint16_t*)buff2 == (uint16_t)2) << endl;
  mo.fetch(buff2, 2);
  cout << "buff2: " << *(uint16_t*)buff2 << endl;
  cout << "b eq 6: " << (*(uint16_t*)buff2 == (uint16_t)6) << endl;

  cout << "\nMix 11 and 11 to 121\n";

  *(uint32_t*)buff = 0x00010001u; // haha, don't forget the endianess! :D
  cout << "buff: " << *(uint16_t*)buff << endl;
  cout << "buff: " << *((uint16_t*)buff + 1) << endl;
  mo.add(buff, 4);
  mo.fetch(buff2, 2);
  cout << "buff2: " << *(uint16_t*)buff2 << endl;
  cout << "b eq 1: " << (*(uint16_t*)buff2 == (uint16_t)1) << endl;
  mo.add(buff, 4);
  mo.fetch(buff2, 2);
  cout << "buff2: " << *(uint16_t*)buff2 << endl;
  cout << "b eq 2: " << (*(uint16_t*)buff2 == (uint16_t)2) << endl;
  mo.fetch(buff2, 2);
  cout << "buff2: " << *(uint16_t*)buff2 << endl;
  cout << "b eq 1: " << (*(uint16_t*)buff2 == (uint16_t)1) << endl;

  return EXIT_SUCCESS;
}

/*
 *
 *  make && g++ -Wall -I. -ggdb -DWITH_ALSA -lX11 -lXevie -lpthread -lasound testMixer.cpp Annoyme.o Event.o Sample.o AlsaOutput.o XevieInput.o InputEventReaderFactory.o StaticConfiguration.o SoundOutputFactory.o SimpleWaveFileLoader.o SoundLoaderFactory.o MixerOutput.o -o testMixer
 *  */
