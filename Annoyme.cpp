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

using namespace std;

#include "Configuration.h"
#include "BasicConfiguration.h"
#include "AnnoymeConfiguration.h"
#include "YAMLConfig.h"

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
{

}

Annoyme::~Annoyme()
{
  delete m_dispatcher;
  delete m_inputEventHandler;
  delete m_soundOutputAdapter;
  delete m_soundOutput;
  delete m_soundLoader;
  delete m_input;
  delete m_config;
}

void Annoyme::init()
{
  cout << "Creating key input reader.\n";
  m_input         = InputEventReaderFactory::getInstance()->getInputEventReader(
                         AnnoymeConfiguration::value("input.reader"));
  cout << "Creating sound file loader.\n";
  m_soundLoader   = SoundLoaderFactory::getInstance()->getSoundLoader(
                         AnnoymeConfiguration::value("sound.loader"),
                         AnnoymeConfiguration::value("sample_directory"));
  cout << "Creating sound output.\n";
  m_soundOutput   = SoundOutputFactory::getInstance()->getSoundOutput(
                         AnnoymeConfiguration::value("sound.output"),
                         AnnoymeConfiguration::value("sound.alsa.device"));

  cout << "Loading sound files.\n";
  m_soundLoader->loadFiles();
  cout << "Opening sound output.\n";
  m_soundOutput->open();
  cout << "Opening event input.\n";
  m_input->open();

  m_soundOutputAdapter = new SoundOutputAdapter(m_soundLoader, m_soundOutput);
  m_inputEventHandler = new HandlerSoundOutput(m_soundOutputAdapter);
  m_dispatcher = new Dispatcher(m_input, m_inputEventHandler);
	
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
  m_dispatcher->close();
  m_input->close();
  m_soundOutput->close();
}


