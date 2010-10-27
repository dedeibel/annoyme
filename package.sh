#!/bin/bash
BUILD=build

MAKE_ARGS="-j2"
DATE=`date +"%Y-%m-%d"`
ARCH=`uname -m`
CMAKE_PARAMS="-DBUILD_DEBUGGING=OFF -DBUILD_TESTING=OFF"

if [ `dirname $0` != "." ]; then
  echo "Error: please only run from annoyme source dir."
  exit
fi

function package() {
  LIB=$1
  CMAKE_PARAMS_ADD=$2

  if [ -d "$BUILD" ]; then
    rm -r "$BUILD"
  fi
  mkdir "$BUILD"
  cd "$BUILD"

  echo "Building $ARCH-$LIB with $CMAKE_PARAMS $CMAKE_PARAMS_ADD"
  
  cmake $CMAKE_PARAMS $CMAKE_PARAMS_ADD ..
  make $MAKE_ARGS
  cpack -G TGZ ..
  TARNAME=`ls annoyme-*.tar.gz`
  NEWTAR=${TARNAME/-Linux/-Linux-$ARCH-$LIB}
  mv "$TARNAME" ../"$NEWTAR"
  cd ..
}

function package_src() {
  if [ -d "$BUILD" ]; then
    rm -r "$BUILD"
  fi
  SRCTAR="annoyme-$DATE-src.tar.gz"
  git archive master | gzip > "$SRCTAR"
  echo "Created $SRCTAR"
}

# Echo build with libao
package "libao" "-DALSA_ENABLED=OFF -DAO_ENABLED=ON"

# Echo build with alsa
package "alsa" "-DALSA_ENABLED=ON -DAO_ENABLED=OFF"

package_src

