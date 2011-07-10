
#include "config.h"

#include <string>
#include <cstring>

// OS specific headers
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xproto.h>
#include <X11/X.h>
#include <X11/Xutil.h>

#ifdef WITH_XEVIE
	#include <X11/extensions/Xevie.h>
#endif

using namespace std;

#include "exceptions.h"

#include "Event.h"
#include "InputEventReader.h"

#ifdef WITH_XEVIE
	#include "XevieInput.h"
#endif

#ifdef WITH_XKEYMAP
	#include <set>

	extern "C"
	{
		#include <X11/Xlib.h>
	}

	#include "XKeyMapInput.h"
#endif

#include "InputEventReaderFactory.h"

InputEventReaderFactory *InputEventReaderFactory::m_instance = 0;

InputEventReader *InputEventReaderFactory::getInputEventReader(
		const string &name, const string &)
{
#ifdef WITH_XEVIE
	if (name == "xevie")
	{
		return new XevieInput();
	}
#endif
#ifdef WITH_XKEYMAP
	if (name == "xkeymap"  || name == "default") {
		return new XKeyMapInput();
	}
#endif
	throw InvalidNameException("Input event reader unkown", name);
}

