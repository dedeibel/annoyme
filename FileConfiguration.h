
#ifndef FILECONFIGURATION_H
#define FILECONFIGURATION_H

#include "Configuration.h"

class FileConfiguration : virtual public Configuration
{
public:
  FileConfiguration ();
  virtual ~FileConfiguration ();
};

#endif // FILECONFIGURATION_H
