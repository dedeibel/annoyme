
#include <string>
#include <cstring>
#include <alsa/asoundlib.h>

using namespace std;

#include "exceptions.h"
#include "Sample.h"
#include "SoundOutput.h"
#include "AlsaOutput.h"

#include "SoundOutputFactory.h"

SoundOutputFactory *SoundOutputFactory::m_instance = 0;

SoundOutput *SoundOutputFactory::getSoundOutput(const string &name, const string &param)
{
  if (name == "alsa")
  {
    return new AlsaOutput(param);
  }
  else
  {
    throw InvalidNameException("Sound output unkown", name);
  }
}

