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

extern "C"
{
#include <unistd.h>
#include <X11/Xlib.h>
}

/*
 * first_keycode + keycode_count - 1 = maxKeyCode according to
 * http://tronche.com/gui/x/xlib/input/XGetKeyboardMapping.html
 */
static int keyCodeCount(int minKeyCodes, int maxKeyCodes) {
	return maxKeyCodes - minKeyCodes + 1;
}

/*
 * The number of elements in the KeySyms list.
 *
 * keycode_count * keysyms_per_keycode_return
 */
static int keySymListSize(int minKeyCodes, int maxKeyCodes, int keySymsPerKeyCode) {
	return keyCodeCount(minKeyCodes, maxKeyCodes) * keySymsPerKeyCode;
}

static int keySymIndex(char keyCode, int minKeyCodes, int keySymsPerKeyCode, int keySymNumber) {
	return (keyCode - minKeyCodes) * keySymsPerKeyCode + keySymNumber;
}

static int keySymIndex(char keyCode, int minKeyCodes, int keySymsPerKeyCode) {
	return keySymIndex(keyCode, minKeyCodes, keySymsPerKeyCode, 0);
}

static KeySym getKeySym(KeySym *keySymMap, int keySymIndex) {
	return keySymMap[keySymIndex];
}

int main(int argc, char **argv)
{
	Display *d = XOpenDisplay(0);
	if (d) {
		/* Converts a given key code to a keysym */
		char keyCode = 38; /* "a" -> <AC01> = 38; */

		int minKeyCodes;
		int maxKeyCodes;

		/* doc: http://tronche.com/gui/x/xlib/input/XDisplayKeycodes.html */
		XDisplayKeycodes(d, &minKeyCodes, &maxKeyCodes);

		std::cout << "XDisplayKeycodes: minKeyCodes " << minKeyCodes << " maxKeyCodes " << maxKeyCodes << " keyCodeCount " << keyCodeCount(minKeyCodes, maxKeyCodes) << std::endl;

		int keySymsPerKeyCode;
		KeySym *keySymMap = XGetKeyboardMapping(d, minKeyCodes, keyCodeCount(minKeyCodes, maxKeyCodes), &keySymsPerKeyCode);
		std::cout << "XGetKeyboardMapping: entries " << keySymListSize(minKeyCodes, maxKeyCodes, keySymsPerKeyCode) << " keySymsPerKeyCode " << keySymsPerKeyCode << std::endl;
		std::cout << "KeySym for keyCode: " << (int)keyCode
				<< " index " << keySymIndex(keyCode, minKeyCodes, keySymsPerKeyCode)
				<< " keySym: " << getKeySym(keySymMap, keySymIndex(keyCode, minKeyCodes, keySymsPerKeyCode))
				<< std::endl;
	}
	else {
		std::cerr << "Could not open display." << std::endl;
	}

	return EXIT_SUCCESS;
}
