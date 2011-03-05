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

#ifndef XKEYMAPUTIL_H_
#define XKEYMAPUTIL_H_

/*
 * #include <sstream>
 * #include "xquerykeymap/XKeyMapMonitor.h"
 */

namespace xutil
{

/*
 * Prints out the keymap into a string of binary digits, separated at each byte.
 *
 * Example with key 22 active:
 *
 * 00000000 00000000 01000000 00000000 00000000 00000000 00000000 00000000
 * 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
 * 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
 * 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
 *
 */
std::string printKeymap(const char *keys);

/*
 * Sets the keycode, addressed by "key" to 1 or 0.
 *
 * Key 22 for example sets the bit 7 in the 3rd byte of the keymap.
 *
 * Example with key 22 active:
 *
 * 00000000 00000000 01000000 00000000 00000000 00000000 00000000 00000000
 * 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
 * 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
 * 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
 */
inline void setKeycode(char *keymap, unsigned char key, bool value)
{
	unsigned const char byte = static_cast<unsigned char> (key / (char) 8u);
	const char bitmask = (char) 1 << (key - (byte * 8));
	if (value) {
		keymap[byte] |= bitmask;
	}
	else {
		keymap[byte] &= (char) 0xFFu ^ bitmask;
	}
}

}

#endif /* XKEYMAPUTIL_H_ */
