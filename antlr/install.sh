#!/bin/sh -e

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
