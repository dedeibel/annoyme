
#ifndef ALSAOUPUT_H
#define ALSAOUPUT_H
#include "SoundOutput.h"

class AlsaOuput : virtual public SoundOutput
{
public:
  AlsaOuput();
  virtual ~AlsaOuput();
  virtual void playSound(Sample sound);
};

#endif // ALSAOUPUT_H
