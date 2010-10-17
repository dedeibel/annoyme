
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

#include <vector>
#include "ResourceLoader.h"

#include "config/Configuration.h"
#include "config/BasicConfiguration.h"
#include "config/AnnoymeConfiguration.h"

SoundLoaderFactory *SoundLoaderFactory::m_instance = 0;

SoundLoaderFactory::SoundLoaderFactory()
: m_resourceLoader(new ResourceLoader(AnnoymeConfiguration::value("resource_directory")))
{
  // TODO resource loader should be build by factory
}

SoundLoaderFactory::~SoundLoaderFactory() {
  if (m_resourceLoader != 0) {
    delete m_resourceLoader;
  }
}

SoundLoader *SoundLoaderFactory::getSoundLoader(const string &name, const string &param)
{
  if (name == "wav")
  {
    return new SimpleWaveFileLoader(m_resourceLoader);
  }
  else
  {
    throw InvalidNameException("Sound Loader unkown", name);
  }
}

