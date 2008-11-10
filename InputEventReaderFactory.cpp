
#include <string>
#include <cstring>

// OS specific headers
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xproto.h>
#include <X11/X.h>
#include <X11/extensions/Xevie.h>
#include <X11/Xutil.h>

using namespace std;

#include "exceptions.h"

#include "Event.h"
#include "InputEventReader.h"

#include "XevieInput.h"

#include "InputEventReaderFactory.h"

InputEventReaderFactory *InputEventReaderFactory::m_instance = 0;

InputEventReader *InputEventReaderFactory::getInputEventReader(
                                          const string &name, const string &)
{
  if (name == "xevie")
  {
    return new XevieInput();
  }
  else
  {
    throw InvalidNameException("Input event reader unkown", name);
  }
}

