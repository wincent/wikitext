#!/bin/sh -e
# Copyright 2007-2008 Wincent Colaiuta
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

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
