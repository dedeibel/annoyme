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

#include "stdheaders.h"

#include "Event.h"
#include "XevieInput.h"
#include "StaticConfiguration.h"
#include "Sample.h"
#include "SimpleWaveFileLoader.h"
#include "AlsaOutput.h"

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

void Annoyme::play(Sample::SampleType type)
{
  const Sample *sample;
  m_soundLoader->getSample(type, sample);
  m_soundOutput->playSound(sample);
}

void Annoyme::normalKeyPressed()
{
  play(Sample::normalKeyPressed);
}


void Annoyme::normalKeyReleased()
{
  play(Sample::normalKeyReleased);
}


void Annoyme::enterPressed()
{
  play(Sample::enterPressed);
}


void Annoyme::enterReleased()
{
  play(Sample::enterReleased);
}


void Annoyme::backspacePressed()
{
  play(Sample::backspacePressed);
}


void Annoyme::backspaceReleased()
{
  play(Sample::backspaceReleased);
}


void Annoyme::init()
{
  m_config        = new StaticConfiguration;
  m_input         = new XevieInput;
  cout << "Creating sound file loader.\n";
  m_soundLoader   = new SimpleWaveFileLoader(m_config->get("Sample directory"));
  cout << "Creating sound output.\n";
  m_soundOutput   = new AlsaOutput(m_config->get("ALSA output device"));

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
    // TODO create dynamic mapping table, event, key ,sound
    m_input->getNextEvent(event);
    switch (event.getSymbol()) {
      // Make key mapping universal, elsewhere
      case XK_BackSpace:
      case XK_Tab:
      case XK_Escape:
      case XK_Delete:
        if (event.getType() == keyPressed) {
          backspacePressed();
        }
        else if (event.getType() == keyReleased) {
          backspaceReleased();
        }
      break;
      case XK_Return:
        if (event.getType() == keyPressed) {
          enterPressed();
        }
        else if (event.getType() == keyReleased) {
          enterReleased();
        }
      break;
      default:
        if (event.getType() == keyPressed) {
          normalKeyPressed();
        }
        else if (event.getType() == keyReleased) {
          normalKeyReleased();
        }
      break;
    }

    cout << "Key " << event.getType() << " '" << event.getSymbol() << "'";
    if (isprint(event.getValue().c_str()[0]))
    {
      cout << " '" << event.getValue() << "' ";
    }
    cout << endl;
  }
}


void Annoyme::close()
{
  m_input->close();
  m_soundOutput->close();
}


