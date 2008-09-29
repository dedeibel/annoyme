
#ifndef SOUNDLOADER_H
#define SOUNDLOADER_H

class SoundLoader
{
public:
  virtual void loadFiles();
  virtual void getSample(SoundSample name, Sample sample);
};

#endif // SOUNDLOADER_H
