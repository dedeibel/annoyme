
#include <string>
#include <cstring>

using namespace std;

#include "config.h"
#include "exceptions.h"
#include "Sample.h"
#include "SoundOutput.h"
#include "MixedOutput.h"

#ifdef WITH_ALSA
  extern "C" {
    #include <alsa/asoundlib.h>
  }
  #include "AlsaOutput.h"
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
    /* Chose the first best */
#ifdef WITH_ALSA
    return new MixedOutput(new AlsaOutput(param));
#elif WITH_AO
    return new AOutput(param);
#else
    throw AnnoymeException("No sound ouput available.");
#endif
  }
  /* Make all alternatives available */
#ifdef WITH_ALSA
  else if (name == "alsa")
  {
    return new AlsaOutput(param);
  }
  else if (name == "mixed-alsa")
  {
    return new MixedOutput(new AlsaOutput(param));
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

