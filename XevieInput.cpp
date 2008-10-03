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

#include <string>

// OS specific headers
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xproto.h>
#include <X11/X.h>
#include <X11/extensions/Xevie.h>
#include <X11/Xutil.h>

using namespace std;

#include "exceptions.h"
#include "Event.h"
#include "XevieInput.h"

/*
 * @param delay Delay in milliseconds
 */
XevieInput::XevieInput(int delay)
: m_delay(delay * 1000)
{

}

XevieInput::~XevieInput()
{

}

/*
 * Initializes Xevie and starts capturing the X key events
 */
void XevieInput::open()
{
	m_display = XOpenDisplay(NULL);
  if (!XevieQueryVersion(m_display, &m_xevieVersionMinor, &m_xevieVersionMajor))
  {
    throw XevieExtensionNotInstalledException();
  }

  if (!XevieStart(m_display))
  {
    throw XevieExtensionNotInstalledException();
  }

  XevieSelectInput(m_display, KeyPressMask | KeyReleaseMask);
}

/*
 * Stops listening for X events
 */
void XevieInput::close()
{
  XevieEnd(m_display);
}


bool XevieInput::getNextEvent(Event &event)
{
	XNextEvent(m_display, &m_event);
  XevieSendEvent(m_display, &m_event, XEVIE_UNMODIFIED);

  m_xcme = reinterpret_cast<XClientMessageEvent *>(&m_event);
  fillEventFromXEvent(event, m_event);
}

void XevieInput::fillEventFromXEvent(Event &event, XEvent &xevent)
{
  switch (xevent.type)
  {
    case KeyPress:
      fillEventFromXKeyEvent(event, xevent);
      event.setType(keyPressed);
    break;
    case KeyRelease:
      fillEventFromXKeyEvent(event, xevent);
      event.setType(keyReleased);
    break;
    case ButtonPress:
    case ButtonRelease:
    case MotionNotify:
    case ClientMessage:
    default:
      throw UnknownEventReceived();
      break;
  }
}

void XevieInput::fillEventFromXKeyEvent(Event &event, XEvent &xevent)
{
  XKeyEvent      *keyEvent;
  KeySym          key;
  XComposeStatus  compose;
  int             charCount;
  char            buffer[20];
  int             bufferSize = 19;

  keyEvent = reinterpret_cast<XKeyEvent*>(&xevent);
  charCount = XLookupString(keyEvent, buffer, bufferSize, &key, &compose);
  buffer[charCount] = '\0';

  event.setValue(buffer);
  event.setSymbol(key);
}

