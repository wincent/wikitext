#!/bin/sh -e
# Copyright 2007 Wincent Colaiuta

VERSION="3.0.1"
ANTLR_VERSION="antlr-${VERSION}"

echo "Cleaning..."
rm -rf "${ANTLR_VERSION}"

echo "Extracting ANTLR..."
tar xzf "${ANTLR_VERSION}.tar.gz"

echo "Extacting C target runtime..."
tar xzf "${ANTLR_VERSION}/runtime/C/dist/libantlr3c-${VERSION}.tar.gz" \
     -C "${ANTLR_VERSION}/runtime/C/dist"

echo "Patching..."
for PATCH in `ls *.diff`
do
  patch -p0 < "${PATCH}"
done

# build ANTLR itself only on the local Darwin machine
if [ `uname` = "Darwin" ]; then
  echo "Building ANTLR..."
  OLD_CLASSPATH="$CLASSPATH"
  export CLASSPATH="/usr/local/junit/junit.jar:./lib/antlr-2.7.7.jar"
  cd "${ANTLR_VERSION}"
  ant -quiet generator
  cd codegen
  FILES=`ls *.java`
  cd ..
  for FILE in $FILES
  do
    find src/org/antlr -name $FILE -delete
  done
  ant -quiet build
  cp build/antlr.jar lib/antlr-3.0.1-custom.jar
  mv lib/antlr-3.0.1.jar lib/antlr-3.0.1.jar.disabled
  export CLASSPATH="$OLD_CLASSPATH"
  cd ..
fi

echo "Building C target runtime..."
cd "${ANTLR_VERSION}/runtime/C/dist/libantlr3c-${VERSION}"
./configure --disable-shared
make
make check

echo "Done: run ./install.sh to install"
