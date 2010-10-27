
=README

Annoyme

=WARNING

This project is merely alpha, any comments,
requests or suggestions are welcome.

Feel free to contact me before writing patches.

=DESCRIPTION

Adds beautiful typewriter sounds to your X

=DEPENDENCIES

- CMake is required to build the project.
- XEvIE       - http://www.freedesktop.org/wiki/Software/XEvIE
- yaml-cpp    - http://code.google.com/p/yaml-cpp
- cppunit     - May be disabled using cmake BUILD_TESTING = false
- One of the following sound output libraries:
 * alsa
 * ao

=INSTALLATION

mkdir build
cd build
cmake ..
make

(or run build.sh)

=EXECUTION

./annoyme

=UNIT_TESTING

(be sure BUILD_TESTING is set, see ccmake ..)
build annoyme

make test

APPENDIX A - Activating XEvIE

In case you want to use the XEvIE input and get a message like the following,
you will have to active it in your X Server configuration first.

  'terminate called after throwing an instance of
  'XevieExtensionNotInstalledException'
  what():  Xevie Extension is not installed or enabled.'

To activate XEvIE you just have to add an entry to your "Extensions"
section of the X configuration.

  Section "Extensions"
    Option "XEVIE" "Enable"
  EndSection
 
XEvIE project page http://www.freedesktop.org/wiki/Software/XEvIE

APPENDIX B - Notes for Mac

There seems to be a Problem with libao and MacOS 10.4, have a look
at https://trac.xiph.org/ticket/727 before compiling the lib.

Currently the binary can be built but is not working, have a look :)

  ./annoyme
  ...
  Opening sound output.
  ao_macosx_open: Only support 44.1kHz right now
  terminate called after throwing an instance of 'SoundOutputException'

