
#include <string>
#include <cstring>

using namespace std;

#include "config.h"
#include "exceptions.h"
#include "Sample.h"
#include "SoundOutput.h"

#ifdef WITH_ALSA
  extern "C" {
    #include <alsa/asoundlib.h>
  }
  #include "AlsaOutput.h"
  #include "MixedAlsaOutput.h"
#endif
#ifdef WITH_AO
  extern "C" {
    #include <ao/ao.h>
  }
  #include "AOutput.h"
#endif

#include "SoundOutputFactory.h"

SoundOutputFactory *SoundOutputFactory::m_instance = 0;

SoundOutput *SoundOutputFactory::getSoundOutput(const string &name, const string &param)
{
  if (name == "default")
  {
    throw InvalidNameException("Sound output unkown", name);
  }
#ifdef WITH_ALSA
  else if (name == "alsa")
  {
    return new AlsaOutput(param);
  }
  else if (name == "mixed-alsa")
  {
    return new MixedAlsaOutput(param);
  }
#endif
#ifdef WITH_AO
  else if (name == "ao")
  {
    return new AOutput(param);
  }
#endif
  else
  {
    throw InvalidNameException("Sound output unkown", name);
  }
}

