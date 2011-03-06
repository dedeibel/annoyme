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

#ifndef XKEYMAPDIFFERENCEFILTER_H_
#define XKEYMAPDIFFERENCEFILTER_H_

#include "XKeyMapListener.h"

namespace xutil
{

class XKeyListener;
class XKeyMapSeparator;

class XKeyMapDifferenceFilter: public xutil::XKeyMapListener
{
public:
	XKeyMapDifferenceFilter(Display *d);
	virtual ~XKeyMapDifferenceFilter();
	void addKeyListener(XKeyListener *listener);
	bool removeKeyListener(XKeyListener *listener);
	virtual void onKeyMapChanged(const char *keyMap, const char *keyMapPrev);

private:
	/* methods */
	void notifyKeysPressed(std::set<KeySym> keys);
	void notifyKeysReleased(std::set<KeySym> keys);

private:
	/* fields */
	Display *m_display;
	XKeyMapSeparator *m_keyMapSeparator;
	std::set<XKeyListener *> m_listeners;
};

}

#endif /* XKEYMAPDIFFERENCEFILTER_H_ */
