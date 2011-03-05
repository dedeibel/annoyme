
README for XKeyMapMonitorPrinter
--

Best used with xxd to print the keymap.

cd build
make xkeymapmonitorprinter
./xkeymapmonitorprinter | xxd -c32 -g1

Example:

0000040: 00 00 40 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ..@.............................

Which is in decimal according to the method documentation:
byte index * 8 + active LSB bit position = key code
-> 2*8+7 = 22

http://tronche.com/gui/x/xlib/input/XQueryKeymap.html

According to my /usr/share/X11/xkb/keycodes/evdev this is BKSP = 22
which is correct.

Find correct key codes:

$ setxkbmap -print    
xkb_keymap {
        xkb_keycodes  { include "evdev+aliases(qwertz)" };
        xkb_types     { include "complete"      };
        xkb_compat    { include "complete"      };
        xkb_symbols   { include "pc+de(nodeadkeys)+inet(evdev)" };
        xkb_geometry  { include "pc(pc105)"     };
};
