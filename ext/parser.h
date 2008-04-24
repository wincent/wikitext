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

#include "ruby_compat.h"

VALUE Wikitext_parser_initialize(int argc, VALUE *argv, VALUE self);

VALUE Wikitext_parser_tokenize(VALUE self, VALUE string);

VALUE Wikitext_parser_benchmarking_tokenize(VALUE self, VALUE string);

VALUE Wikitext_parser_fulltext_tokenize(VALUE self, VALUE string);

VALUE Wikitext_parser_sanitize_link_target(VALUE self, VALUE string);

VALUE Wikitext_parser_encode_link_target(VALUE self, VALUE in);

VALUE Wikitext_parser_encode_special_link_target(VALUE self, VALUE in);

VALUE Wikitext_parser_parse(int argc, VALUE *argv, VALUE self);

VALUE Wikitext_parser_profiling_parse(VALUE self, VALUE string);
