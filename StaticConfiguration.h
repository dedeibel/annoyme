
#ifndef CONFIGURATION_H
#define CONFIGURATION_H

using namespace std;

class Configuration
{
public:
  virtual string getValue(string name);
  virtual void init();

  string operator[](string name)
  {
    return getValue(name);
  }

};

#endif // CONFIGURATION_H
