/*
 * Copyright (c) 2011, Benjamin Peter <BenjaminPeter@arcor.de>
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

#include <iostream>
#include <cstdlib>
#include <cstring>

extern "C" {
  #include <unistd.h>
  #include <X11/Xlib.h>
}

/**
 *
 * Small demo programm to test the XQueryKeymap function
 * for the use with annoyme. Seems to work very well,
 * screw Xevie!
 *
 */ 

std::string toBin(char c) {
  std::string result;
  for (int i = (sizeof(char) * 8) - 1; i >= 0; --i) {
    if (i == 3) {
      result += ' ';
    }
    result += (c & ((char)1 << i)) ? '1' : '0';
  }
  return result;
}

std::string printKeymap(char *keys) {
  std::string result;
  for (int i = 0; i < 32; ++i) {
    result += ' ' + toBin(keys[i]);
  }
  return result;
}

int main(int argc, char **argv)
{
  Display *d = XOpenDisplay(0);
  if (d) {
    char keys[32] = { 0 };
    char keys_prev[32] = { 0 };
    while (true) {
      XQueryKeymap(d, keys);

      if (memcmp(keys, keys_prev, 32) != 0) {
//      std::cout << printKeymap(keys) << std::endl;
        for (int i = 0; i < 32; ++i) {
            std::cout << keys[i];
        }
        std::cout << std::flush;
      }

      memcpy(keys_prev, keys, 32);
      usleep(50);
    }
  }
  else {
    std::cerr << "Could not open display" << std::endl;
  }
  
  return EXIT_SUCCESS;
}
