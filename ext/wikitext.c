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

#include "wikitext_ragel.h"
#include "parser.h"

VALUE mWikitext              = 0;   // module Wikitext
VALUE cWikitextParser        = 0;   // class Wikitext::Parser
VALUE eWikitextParserError   = 0;   // class Wikitext::Parser::Error
VALUE cWikitextParserToken   = 0;   // class Wikitext::Parser::Token

void Init_wikitext()
{
    // Wikitext
    mWikitext = rb_define_module("Wikitext");

    // Wikitext::Parser
    cWikitextParser = rb_define_class_under(mWikitext, "Parser", rb_cObject);
    rb_define_method(cWikitextParser, "initialize", Wikitext_parser_initialize, -1);
    rb_define_method(cWikitextParser, "parse", Wikitext_parser_parse, -1);
    rb_define_method(cWikitextParser, "profiling_parse", Wikitext_parser_profiling_parse, 1);
    rb_define_method(cWikitextParser, "tokenize", Wikitext_parser_tokenize, 1);
    rb_define_method(cWikitextParser, "benchmarking_tokenize", Wikitext_parser_benchmarking_tokenize, 1);
    rb_define_method(cWikitextParser, "fulltext_tokenize", Wikitext_parser_fulltext_tokenize, -1);
    rb_define_singleton_method(cWikitextParser, "sanitize_link_target", Wikitext_parser_sanitize_link_target, 1);
    rb_define_singleton_method(cWikitextParser, "encode_link_target", Wikitext_parser_encode_link_target, 1);
    rb_define_singleton_method(cWikitextParser, "encode_special_link_target", Wikitext_parser_encode_special_link_target, 1);
    rb_define_attr(cWikitextParser, "line_ending", Qtrue, Qtrue);
    rb_define_attr(cWikitextParser, "internal_link_prefix", Qtrue, Qtrue);
    rb_define_attr(cWikitextParser, "img_prefix", Qtrue, Qtrue);
    rb_define_attr(cWikitextParser, "external_link_class", Qtrue, Qtrue);
    rb_define_attr(cWikitextParser, "mailto_class", Qtrue, Qtrue);
    rb_define_attr(cWikitextParser, "autolink", Qtrue, Qtrue);
    rb_define_attr(cWikitextParser, "treat_slash_as_special", Qtrue, Qtrue);
    rb_define_attr(cWikitextParser, "space_to_underscore", Qtrue, Qtrue);
    rb_define_attr(cWikitextParser, "minimum_fulltext_token_length", Qtrue, Qtrue);

    // Wikitext::Parser::Error
    eWikitextParserError = rb_define_class_under(cWikitextParser, "Error", rb_eException);

    // Wikitext::Parser::Token
    cWikitextParserToken = rb_define_class_under(cWikitextParser, "Token", rb_cObject);
    rb_define_singleton_method(cWikitextParserToken, "types", Wikitext_parser_token_types, 0);
    rb_define_attr(cWikitextParserToken, "start", Qtrue, Qfalse);
    rb_define_attr(cWikitextParserToken, "stop", Qtrue, Qfalse);
    rb_define_attr(cWikitextParserToken, "line_start", Qtrue, Qfalse);
    rb_define_attr(cWikitextParserToken, "line_stop", Qtrue, Qfalse);
    rb_define_attr(cWikitextParserToken, "column_start", Qtrue, Qfalse);
    rb_define_attr(cWikitextParserToken, "column_stop", Qtrue, Qfalse);
    rb_define_attr(cWikitextParserToken, "code_point", Qtrue, Qfalse);
    rb_define_attr(cWikitextParserToken, "token_type", Qtrue, Qfalse);
    rb_define_attr(cWikitextParserToken, "string_value", Qtrue, Qfalse);
}
