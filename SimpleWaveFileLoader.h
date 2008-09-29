
#ifndef SIMPLEWAVEFILELOADER_H
#define SIMPLEWAVEFILELOADER_H
#include "SoundLoader.h"

class SimpleWaveFileLoader : virtual public SoundLoader
{
public:
  SimpleWaveFileLoader();
  virtual ~SimpleWaveFileLoader();
};

#endif // SIMPLEWAVEFILELOADER_H
