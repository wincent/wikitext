// Copyright 2008 Wincent Colaiuta
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <ruby.h>
#include <stdint.h>

#define ruby_inspect(obj) rb_funcall(rb_mKernel, rb_intern("p"), 1, obj)

// module Wikitext
extern VALUE mWikitext;

// class Wikitext::Parser
extern VALUE cWikitextParser;

// class Wikitext::Parser::Error
extern VALUE eWikitextParserError;

// class Wikitext::Parser::Token
extern VALUE cWikitextParserToken;
