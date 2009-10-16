
#include <string>
#include <map>
#include <cerrno>
#include <cstring>

using namespace std;

extern "C" {
  #include <glob.h>
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <fcntl.h>
  #include <libgen.h>
}

#include "stdutil.h"
#include "Sample.h"
#include "exceptions.h"
#include "SoundLoader.h"
#include "SimpleWaveFileLoader.h"

#include "SoundLoaderFactory.h"

SoundLoaderFactory *SoundLoaderFactory::m_instance = 0;

SoundLoader *SoundLoaderFactory::getSoundLoader(const string &name, const string &param)
{
  if (name == "wav")
  {
    return new SimpleWaveFileLoader(param);
  }
  else
  {
    throw InvalidNameException("Sound Loader unkown", name);
  }
}

