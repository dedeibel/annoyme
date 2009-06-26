/*
 * Copyright (c) 2009, Benjamin Peter <BenjaminPeter@arcor.de>
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
#include <string>

using namespace std;

#include "Event.h"
#include "InputEventReader.h"
#include "InputEventHandler.h"
#include "Dispatcher.h"

Dispatcher::Dispatcher(InputEventReader *reader, InputEventHandler *handler)
: m_inputEventReader(reader)
, m_inputEventHandler(handler)
{
	
}

Dispatcher::~Dispatcher() {
	
}

void Dispatcher::init() {
}

void Dispatcher::run() {
	Event event;
	while (1)
	{
		// TODO create dynamic mapping table, event, (key) ,sound
		m_inputEventReader->getNextEvent(event);
		
		cout << "Key " << event.getType() << " '" << event.getSymbol() << "'";
		if (isprint(event.getValue().c_str()[0]))
		{
			cout << " '" << event.getValue() << "' ";
		}
		cout << endl;
		
		switch (event.getType())
		{
			case eventNormalKeyPressed:
				m_inputEventHandler->handleNormalKeyPressed();
				break;
			case eventNormalKeyReleased:
				m_inputEventHandler->handleNormalKeyReleased();
				break;
			case eventEnterKeyPressed:
				m_inputEventHandler->handleEnterPressed();
				break;
			case eventEnterKeyReleased:
				m_inputEventHandler->handleEnterReleased();
				break;
			case eventBackspaceKeyPressed:
				m_inputEventHandler->handleBackspacePressed();
				break;
			case eventBackspaceKeyReleased:
				m_inputEventHandler->handleBackspaceReleased();
				break;
			default:
				cerr << "Event '"<< event.getType() << "' currently not supported.\n";
				break;
		}
	}	
}

void Dispatcher::close() {
	
}
