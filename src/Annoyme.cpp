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

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <cstring>

using namespace std;

#include "exceptions.h"

#include "config/Configuration.h"
#include "config/BasicConfiguration.h"
#include "config/AnnoymeConfiguration.h"

// Input
#include "Event.h"
#include "InputEventReader.h"
#include "InputEventReaderFactory.h"

// Sound
#include "Sample.h"
#include "SoundOutput.h"
#include "SoundOutputFactory.h"
#include "SoundLoader.h"
#include "SoundLoaderFactory.h"

// Flow
#include "SoundOutputAdapter.h"
#include "InputEventHandler.h"
#include "HandlerSoundOutput.h"
#include "Dispatcher.h"

#include "Annoyme.h"

Annoyme::Annoyme()
: m_input(0)
, m_soundLoader(0)
, m_soundOutput(0)
, m_soundOutputAdapter(0)
, m_inputEventHandler(0)
, m_dispatcher(0)
{

}

Annoyme::~Annoyme()
{
  if (m_dispatcher != 0)          delete m_dispatcher;
  if (m_inputEventHandler != 0)   delete m_inputEventHandler;
  if (m_soundOutputAdapter != 0)  delete m_soundOutputAdapter;
  if (m_soundOutput != 0)         delete m_soundOutput;
  if (m_soundLoader != 0)         delete m_soundLoader;
  if (m_input != 0)               delete m_input;
}

void Annoyme::init() throw(AnnoymeException)
{
  AnnoymeConfiguration::getInstance();

  cout << "Creating key input reader.\n";
  m_input         = InputEventReaderFactory::getInstance()->getInputEventReader(
                         AnnoymeConfiguration::value("input.reader"));
  cout << "Creating sound file loader.\n";
  m_soundLoader   = SoundLoaderFactory::getInstance()->getSoundLoader(
                         AnnoymeConfiguration::value("sound.loader"));
  cout << "Creating sound output.\n";
  m_soundOutput   = SoundOutputFactory::getInstance()->getSoundOutput(
                         AnnoymeConfiguration::value("sound.output"),
                         AnnoymeConfiguration::value("sound.alsa.device"));

  cout << "Loading sound files.\n";
  m_soundLoader->loadFiles(AnnoymeConfiguration::value("sample_theme"));
  cout << "Opening sound output.\n";
  m_soundOutput->open();
  cout << "Opening event input.\n";
  m_input->open();

  m_soundOutputAdapter  = new SoundOutputAdapter(m_soundLoader, m_soundOutput);
  m_inputEventHandler   = new HandlerSoundOutput(m_soundOutputAdapter);
  m_dispatcher          = new Dispatcher(m_input, m_inputEventHandler);
	
  cout << "Initializing dispatcher.\n";
  m_dispatcher->init();
}


void Annoyme::run()
{
	cout << "Starting dispatcher.\n";
	m_dispatcher->run();
}

void Annoyme::close()
{
  if (m_dispatcher != 0)  m_dispatcher->close();
  if (m_input != 0)       m_input->close();
  if (m_soundOutput != 0) m_soundOutput->close();
}


