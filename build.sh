#!/bin/sh
echo "Note: If your are getting compile errors, please check your dependencies and project build settings using \"cd build; ccmake ..\"\n"
BUILD=build
if [ ! -d "$BUILD" ]; then
  mkdir "$BUILD"
fi
cd "$BUILD"
cmake ..
make &&
echo "\ndone. Binary can be found in ./$BUILD/"
