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
#include "xquerykeymap/XKeyMapSeparator.h"
#include "xquerykeymap/XKeyMapSeparatorImpl.h"

class XKeyMapSeparatorImplTest: public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(XKeyMapSeparatorImplTest);
		CPPUNIT_TEST(testEmpty);
		CPPUNIT_TEST(testSimple);
		CPPUNIT_TEST(testMultiple);
	CPPUNIT_TEST_SUITE_END();
public:
	void testEmpty()
	{
		xutil::XKeyMapSeparatorImpl separator;
		std::set<unsigned char> keys;
		char keymap[xutil::KEYMAP_SIZE_BYTES];
		memset(keymap, 0, xutil::KEYMAP_SIZE_BYTES);
		separator.getKeycodes(keymap, keys);

		CPPUNIT_ASSERT(keys.empty());
	}

	void testSimple()
	{
		xutil::XKeyMapSeparatorImpl separator;
		std::set<unsigned char> keys;
		char keymap[xutil::KEYMAP_SIZE_BYTES];
		memset(keymap, 0, xutil::KEYMAP_SIZE_BYTES);

		xutil::setKeycode(keymap, 22, true);
		separator.getKeycodes(keymap, keys);
		CPPUNIT_ASSERT(keys.find(22) != keys.end());
	}

	void testMultiple()
	{
		xutil::XKeyMapSeparatorImpl separator;
		std::set<unsigned char> keys;
		char keymap[xutil::KEYMAP_SIZE_BYTES];
		memset(keymap, 0, xutil::KEYMAP_SIZE_BYTES);

		xutil::setKeycode(keymap, 20, true);
		xutil::setKeycode(keymap, 0, true);
		xutil::setKeycode(keymap, 240, true);

		separator.getKeycodes(keymap, keys);

		CPPUNIT_ASSERT(keys.find(0) != keys.end());
		CPPUNIT_ASSERT(keys.find(20) != keys.end());
		CPPUNIT_ASSERT(keys.find(240) != keys.end());
	}
};
CPPUNIT_TEST_SUITE_REGISTRATION(XKeyMapSeparatorImplTest);
