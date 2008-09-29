#include <string>
#include <exception>
#include <cctype>
#include "Configuration.h"
#include "exceptions.h"

string Configuration::getValue(string name)
{
  std::transform(name.begin(), name.end(), name.begin(), ::tolower);
  if (name.compare("sample directory")   == 0) { return string("."); };
  if (name.compare("alsa output device") == 0) { return string("plughw:0,0"); };

  throw UnknownOptionException(name);
}


void Configuration::init()
{


}
