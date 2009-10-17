#!/bin/sh

if [ $# -ne 1 ]; then
  echo "$0 VERSION"
  exit
fi

ORIGINDIR=$PWD
cd ..
SRCDIR=$PWD

APPNAME=annoyme
APPVERS=$1
CODIR=$APPNAME-$APPVERS
PACKAGENAME=$APPNAME-$APPVERS-src.tar.bz2
TEMP=`mktemp -d`

cd $TEMP
git clone $SRCDIR $CODIR
rm -r $CODIR/.git
rm $CODIR/.gitignore
mkdir $CODIR/build

tar cvjf $PACKAGENAME $CODIR
mv $PACKAGENAME $ORIGINDIR
rm -r $TEMP

echo "created $PACKAGENAME"
