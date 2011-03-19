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
#include <set>
#include <vector>

extern "C"
{
#include <X11/Xlib.h>
}

#include "XKeyListenerBuilder.h"

#include "IllegalStateException.h"

#include "XKeyListener.h"
#include "XKeyMapMonitor.h"
#include "XKeyMapDifferenceFilter.h"
#include "XKeyListenerPrinter.h"
#include "XKeyMapListenerPrinter.h"

namespace xutil
{

XKeyListenerBuilder::XKeyListenerBuilder()
 : m_display(0) {
}

void XKeyListenerBuilder::connect()
{
	connect(std::string());
}

void XKeyListenerBuilder::connect(const std::string &displayName)
{
	if (m_display != 0) {
		throw IllegalStateException("Already connected, display already open.");
	}

	const char *displayNameChar = 0;
	if (!displayName.empty()) {
		displayNameChar = displayName.c_str();
	}
	m_display = XOpenDisplay(displayNameChar);
}

void XKeyListenerBuilder::disconnect()
{
	if (m_display != 0) {
		XFree(m_display);
		m_display = 0;
	}
}

std::string XKeyListenerBuilder::getDisplayName()
{
	if (!m_display) {
		throw IllegalStateException(
				"Not connected yet, no is display set. Use connect first.");
	}

	return std::string(XDisplayString(m_display));
}

XKeyMapMonitor * XKeyListenerBuilder::getKeyMapMonitor()
{
	if (!m_display) {
		throw IllegalStateException(
				"Not connected yet, no is display set. Use connect first.");
	}
	return new XKeyMapMonitor(m_display);
}

XKeyListenerPrinter * XKeyListenerBuilder::getKeyListenerPrinter()
{
	return new XKeyListenerPrinter();
}

XKeyMapListenerPrinter * XKeyListenerBuilder::getKeyMapListenerPrinter()
{
	return new XKeyMapListenerPrinter();
}

XKeyMapDifferenceFilter * XKeyListenerBuilder::getKeyMapDifferenceFilter()
{
	if (!m_display) {
		throw IllegalStateException(
				"Not connected yet, no is display set. Use connect first.");
	}
	return new XKeyMapDifferenceFilter(m_display);
}
}
