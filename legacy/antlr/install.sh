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

echo "Installing C target runtime..."
cd "${ANTLR_VERSION}/runtime/C/dist/libantlr3c-${VERSION}"
sudo make install
cd "../../../../.."

# install ANTLR itself only on the local Darwin machine
if [ `uname` = "Darwin" ]; then
  echo "Installing ANTLR..."
  sudo rm -rf "/usr/local/${ANTLR_VERSION}"
  sudo mv "${ANTLR_VERSION}" /usr/local/
  sudo rm -f /usr/local/antlr
  $(cd /usr/local && sudo ln -s "${ANTLR_VERSION}" antlr)
  echo "To use ANTLR, add the following exports to your environment:"
  cat <<EXPORTS
  export CLASSPATH=".:/usr/local/antlr/lib/antlr-3.0.1-custom.jar"
  export CLASSPATH="\$CLASSPATH:/usr/local/antlr/lib/antlr-2.7.7.jar"
  export CLASSPATH="\$CLASSPATH:/usr/local/antlr/lib/antlr-runtime-3.0.1.jar"
  export CLASSPATH="\$CLASSPATH:/usr/local/antlr/lib/stringtemplate-3.1b1.jar"
EXPORTS
fi

echo "Done"
