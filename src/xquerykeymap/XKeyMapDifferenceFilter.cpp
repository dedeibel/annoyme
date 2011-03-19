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

extern "C"
{
#include <X11/Xlib.h>
#include <unistd.h>
}

#include <string>
#include <set>
#include <algorithm>
#include <vector>
#include <functional>

#include <iostream> // debug
#include "XKeyListener.h"
#include "XKeyMapDifferenceFilter.h"
#include "XKeyMapSeparatorImpl.h"
#include "XKeySymUtil.h"

/*
 * doc: http://tronche.com/gui/x/xlib/input/XGetKeyboardMapping.html
 */

namespace xutil
{

XKeyMapDifferenceFilter::XKeyMapDifferenceFilter(Display *d) :
	m_display(d), m_keyMapSeparator(new XKeyMapSeparatorImpl()), m_keySymMap(0)
{

}

XKeyMapDifferenceFilter::~XKeyMapDifferenceFilter()
{

}

void XKeyMapDifferenceFilter::addKeyListener(XKeyListener *listener)
{
	m_listeners.insert(listener);
}

bool XKeyMapDifferenceFilter::removeKeyListener(XKeyListener *listener)
{
	return m_listeners.erase(listener) != 0;;
}

KeySym XKeyMapDifferenceFilter::convert(unsigned char keyCode)
{
	return XKeySymUtil::getKeySym(m_keySymMap,
			XKeySymUtil::keySymIndex(keyCode, m_minKeyCodes, m_keySymsPerKeyCode));
}

void XKeyMapDifferenceFilter::onKeyMapChanged(const char *keyMap,
		const char *keyMapPrev)
{
	if (!isInitialized()) {
		init();
	}

	std::set<unsigned char> keysPrev;
	m_keyMapSeparator->getKeycodes(keyMapPrev, keysPrev);

	std::set<unsigned char> keys;
	m_keyMapSeparator->getKeycodes(keyMap, keys);

	/* Calculate what keys were pressed and released by comparing the currently
	 * active keys.
	 */
	std::vector<unsigned char> keysReleased;
	std::set_difference(keysPrev.begin(), keysPrev.end(), keys.begin(),
			keys.end(), std::inserter(keysReleased, keysReleased.begin()));

	std::vector<unsigned char> keysPressed;
	std::set_difference(keys.begin(), keys.end(), keysPrev.begin(),
			keysPrev.end(), std::inserter(keysPressed, keysPressed.begin()));

	std::set<KeySym> keysReleasedSet;
	std::insert_iterator<std::set<KeySym> > releasedInserter(keysReleasedSet,
			keysReleasedSet.begin());

	convertAll(keysReleased.begin(), keysReleased.end(), releasedInserter);

	std::set<KeySym> keysPressedSet;
	std::insert_iterator<std::set<KeySym> > pressedInserter(keysPressedSet,
			keysPressedSet.begin());

	convertAll(keysPressed.begin(), keysPressed.end(), pressedInserter);

	/* Notify the listeners */
	notifyKeysReleased(keysReleasedSet);
	notifyKeysPressed(keysPressedSet);
}

void XKeyMapDifferenceFilter::notifyKeysPressed(std::set<KeySym> keys)
{
	std::set<XKeyListener*>::iterator it = m_listeners.begin();
	while (it != m_listeners.end()) {
		(*it)->onKeysPressed(keys);
		++it;
	}
}

void XKeyMapDifferenceFilter::notifyKeysReleased(std::set<KeySym> keys)
{
	std::set<XKeyListener*>::iterator it = m_listeners.begin();
	while (it != m_listeners.end()) {
		(*it)->onKeysReleased(keys);
		++it;
	}
}

bool XKeyMapDifferenceFilter::isInitialized()
{
	return m_keySymMap != 0;
}

void XKeyMapDifferenceFilter::init()
{
	/* doc: http://tronche.com/gui/x/xlib/input/XDisplayKeycodes.html */
	XDisplayKeycodes(m_display, &m_minKeyCodes, &m_maxKeyCodes);
	m_keySymMap = XGetKeyboardMapping(m_display, m_minKeyCodes,
			XKeySymUtil::keyCodeCount(m_minKeyCodes, m_maxKeyCodes), &m_keySymsPerKeyCode);
}

}
