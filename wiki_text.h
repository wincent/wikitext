// Copyright 2007 Wincent Colaiuta

#include <ruby.h>

// ruby.h pulls in config.h which defines these macros
// undef them to avoid warnings when antlr.h ends up defining the same macros further down
#ifdef PACKAGE_NAME
#undef PACKAGE_NAME
#endif

#ifdef PACKAGE_TARNAME
#undef PACKAGE_TARNAME
#endif

#ifdef PACKAGE_VERSION
#undef PACKAGE_VERSION
#endif

#ifdef PACKAGE_STRING
#undef PACKAGE_STRING
#endif

#ifdef PACKAGE_BUGREPORT
#undef PACKAGE_BUGREPORT
#endif

// the ANTLR-generated lexer
#include "WikiTextLexer.h"

