// Copyright 2008-2009 Wincent Colaiuta. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

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
    rb_define_attr(cWikitextParser, "space_to_underscore", Qtrue, Qtrue);
    rb_define_attr(cWikitextParser, "minimum_fulltext_token_length", Qtrue, Qtrue);
    rb_define_attr(cWikitextParser, "base_heading_level", Qtrue, Qtrue);

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
