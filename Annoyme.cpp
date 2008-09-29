
#include "Annoyme.h"

#include "InputEventReader.h"
#include "XevieInput.h"
#include "Configuration.h"
#include "StaticConfiguration.h"
#include "Configuration.h"
#include "StaticConfiguration.h"
#include "SoundLoader.h"
#include "SimpleWaveLoader.h"
#include "SoundOutput.h"
#include "AlsaOuput.h"

Annoyme::Annoyme()
{

}

Annoyme::~Annoyme()
{
}


void Annoyme::normalKeyPressed()
{


}


void Annoyme::normalKeyReleased()
{


}


void Annoyme::enterPressed()
{


}


void Annoyme::enterReleased()
{


}


void Annoyme::backspacePressed()
{


}


void Annoyme::backspaceReleased()
{


}


void Annoyme::init()
{
  config        = new StaticConfiguration();
  input         = new XevieInput();
  soundLoader   = new SimpleWaveFileLoader(config["Sample directory"]);
  soundOutput   = new AlsaOutput(config["ALSA output device"]);
  config["fail"];
}


void Annoyme::run()
{


}


void Annoyme::close()
{


}


