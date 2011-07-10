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

#include <string>
#include <cerrno>
#include <cstring>
#include <set>

extern "C"
{
	#include <X11/Xlib.h>
	#include "X11/keysym.h"
}


using namespace std;

#include "exceptions.h"
#include "Event.h"

#include "XKeyMapInput.h"

#include <iostream>
#include <cstdlib>
#include <vector>
#include <queue>

#include "xquerykeymap/XKeyMapMonitor.h"
#include "xquerykeymap/XKeyMapDifferenceFilter.h"
#include "xquerykeymap/XKeyListenerBuilder.h"
#include "xquerykeymap/XKeyBufferingListener.h"

XKeyMapInput::XKeyMapInput() :
	running(false), builder(0), monitor(0), filter(0), listener(0)
{
}

XKeyMapInput::~XKeyMapInput()
{
}

void XKeyMapInput::open()
{
	if (builder != 0) {
		throw new AnnoymeException(
				"Tried to open the input device twice. Use close first.");
	}
	builder = new xutil::XKeyListenerBuilder();
	builder->connect();
	monitor = builder->getKeyMapMonitor();
	filter = builder->getKeyMapDifferenceFilter();
	monitor->addKeyMapListener(filter);
	listener = new xutil::XKeyBufferingListener();
	filter->addKeyListener(listener);

	monitor->start();
}

void XKeyMapInput::close()
{
	monitor->stop();
	delete listener;
	listener = 0;
	delete filter;
	filter = 0;
	delete monitor;
	monitor = 0;
	delete builder;
	builder = 0;
}

bool XKeyMapInput::getNextEvent(Event &event)
{
	std::set<KeySym> *keys;

	while (1) {
		keys = listener->pop();
		if (keys == 0 || keys->empty()) {
			usleep(10000);
		}
		else {
			return this->fillEvent(event, keys);
		}
	}
}

bool XKeyMapInput::fillEvent(Event &event, std::set<KeySym> *keys) {
	KeySym first = *(keys->begin());
	event.setSymbol((unsigned int) first);
	// TODO implement type determination
	event.setType(eventNormalKeyPressed);
	event.setValue(XKeysymToString(first));
	return true;
}
