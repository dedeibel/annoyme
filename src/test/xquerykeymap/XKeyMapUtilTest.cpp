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

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include <set>
#include <string>
#include <cstring>

#include "xquerykeymap/XKeyMapConstants.h"
#include "xquerykeymap/XKeyMapUtil.h"

class XKeyMapUtilTest: public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(XKeyMapUtilTest);
		CPPUNIT_TEST(testSetKeycode);
	CPPUNIT_TEST_SUITE_END();
public:
	void testSetKeycode()
	{
		char keymap[xutil::KEYMAP_SIZE_BYTES];
		memset(keymap, 0, xutil::KEYMAP_SIZE_BYTES);

		/* set key 22 pressed */
		xutil::setKeycode(keymap, 22, true);
		CPPUNIT_ASSERT(keymap[2] == 0x40);

		/* reset to zero */
		xutil::setKeycode(keymap, 22, false);
		char nullkeymap[xutil::KEYMAP_SIZE_BYTES] = { 0 };
		CPPUNIT_ASSERT(memcmp(nullkeymap, keymap, xutil::KEYMAP_SIZE_BYTES) == 0);

		/* set zero */
		xutil::setKeycode(keymap, 0, true);
		CPPUNIT_ASSERT(keymap[0] == 0x01);

		/* clean again */
		memset(keymap, 0, xutil::KEYMAP_SIZE_BYTES);

		/* set two keys */
		xutil::setKeycode(keymap, 22, true);
		xutil::setKeycode(keymap, 33, true);
		CPPUNIT_ASSERT(keymap[2] == 0x40);
		CPPUNIT_ASSERT(keymap[4] == 0x02);
	}
};
CPPUNIT_TEST_SUITE_REGISTRATION(XKeyMapUtilTest);
