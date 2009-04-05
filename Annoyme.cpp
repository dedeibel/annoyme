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
#include <iostream>

using namespace std;

#include "Event.h"
#include "Sample.h"
#include "StaticConfiguration.h"
#include "InputEventReader.h"
#include "InputEventReaderFactory.h"
#include "SoundOutput.h"
#include "SoundOutputFactory.h"
#include "SoundLoader.h"
#include "SoundLoaderFactory.h"

#include "Annoyme.h"

Annoyme::Annoyme()
{

}

Annoyme::~Annoyme()
{
  delete m_soundOutput;
  delete m_soundLoader;
  delete m_input;
  delete m_config;
}

void Annoyme::play(enum Sample::SampleType type)
{
  const Sample *sample;
  m_soundLoader->getSample(type, &sample);
  m_soundOutput->playSound(sample);
}

void Annoyme::handleNormalKeyPressed()
{
  play(Sample::normalKeyPressed);
}


void Annoyme::handleNormalKeyReleased()
{
  play(Sample::normalKeyReleased);
}


void Annoyme::handleEnterPressed()
{
  play(Sample::enterPressed);
}


void Annoyme::handleEnterReleased()
{
  play(Sample::enterReleased);
}


void Annoyme::handleBackspacePressed()
{
  play(Sample::backspacePressed);
}


void Annoyme::handleBackspaceReleased()
{
  play(Sample::backspaceReleased);
}


void Annoyme::init()
{
  m_config        = new StaticConfiguration;
  cout << "Creating key input reader.\n";
  m_input         = InputEventReaderFactory::getInstance()->getInputEventReader(
                         m_config->get("Input event reader"));
  cout << "Creating sound file loader.\n";
  m_soundLoader   = SoundLoaderFactory::getInstance()->getSoundLoader(
                         m_config->get("Sound loader"), m_config->get("Sample directory"));
  cout << "Creating sound output.\n";
  m_soundOutput   = SoundOutputFactory::getInstance()->getSoundOutput(
                         m_config->get("Sound output"), m_config->get("alsa output device"));

  cout << "Loading sound files.\n";
  m_soundLoader->loadFiles();
  cout << "Opening sound output.\n";
  m_soundOutput->open();
  cout << "Opening event input.\n";
  m_input->open();
}


void Annoyme::run()
{
  Event event;
  while (1)
  {
    // TODO create dynamic mapping table, event, (key) ,sound
    m_input->getNextEvent(event);

    cout << "Key " << event.getType() << " '" << event.getSymbol() << "'";
    if (isprint(event.getValue().c_str()[0]))
    {
      cout << " '" << event.getValue() << "' ";
    }
    cout << endl;

    switch (event.getType())
    {
      case eventNormalKeyPressed:
        handleNormalKeyPressed();
      break;
      case eventNormalKeyReleased:
        handleNormalKeyReleased();
      break;
      case eventEnterKeyPressed:
        handleEnterPressed();
      break;
      case eventEnterKeyReleased:
        handleEnterReleased();
      break;
      case eventBackspaceKeyPressed:
        handleBackspacePressed();
      break;
      case eventBackspaceKeyReleased:
        handleBackspaceReleased();
      break;
      default:
        cerr << "Event '"<< event.getType() << "' currently not supported.\n";
      break;
    }
  }
}


void Annoyme::close()
{
  m_input->close();
  m_soundOutput->close();
}


