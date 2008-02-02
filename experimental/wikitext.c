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

static VALUE mWikitext              = 0;    // Wikitext
static VALUE cWikitextParser        = 0;    // Wikitext::Parser
static VALUE cWikitextParserToken   = 0;    // Wikitext::Parser::Token
VALUE eWikitextError                = 0;    // Wikitext::Error

// return a hash of token types
// we make this available for unit testing purposes
VALUE Wikitext_token_types(VALUE self)
{
    VALUE hash = rb_hash_new();

#define SET_TOKEN_TYPE(identifier)  (void)rb_hash_aset(hash, INT2FIX(identifier), \
    rb_funcall(rb_funcall(rb_str_new2(#identifier), rb_intern("downcase"), 0), rb_intern("to_sym"), 0))

    SET_TOKEN_TYPE(NO_TOKEN);
    SET_TOKEN_TYPE(P);
    SET_TOKEN_TYPE(LI);
    SET_TOKEN_TYPE(PRE);
    SET_TOKEN_TYPE(NO_WIKI_START);
    SET_TOKEN_TYPE(NO_WIKI_END);
    SET_TOKEN_TYPE(BLOCKQUOTE);
    SET_TOKEN_TYPE(STRONG_EM);
    SET_TOKEN_TYPE(STRONG);
    SET_TOKEN_TYPE(EM);
    SET_TOKEN_TYPE(TT_START);
    SET_TOKEN_TYPE(TT_END);
    SET_TOKEN_TYPE(TT);
    SET_TOKEN_TYPE(OL);
    SET_TOKEN_TYPE(UL);
    SET_TOKEN_TYPE(H6_START);
    SET_TOKEN_TYPE(H5_START);
    SET_TOKEN_TYPE(H4_START);
    SET_TOKEN_TYPE(H3_START);
    SET_TOKEN_TYPE(H2_START);
    SET_TOKEN_TYPE(H1_START);
    SET_TOKEN_TYPE(H6_END);
    SET_TOKEN_TYPE(H5_END);
    SET_TOKEN_TYPE(H4_END);
    SET_TOKEN_TYPE(H3_END);
    SET_TOKEN_TYPE(H2_END);
    SET_TOKEN_TYPE(H1_END);
    SET_TOKEN_TYPE(URI);
    SET_TOKEN_TYPE(LINK_START);
    SET_TOKEN_TYPE(LINK_END);
    SET_TOKEN_TYPE(EXT_LINK_START);
    SET_TOKEN_TYPE(EXT_LINK_END);
    SET_TOKEN_TYPE(SEPARATOR);
    SET_TOKEN_TYPE(SPACE);
    SET_TOKEN_TYPE(QUOT_ENTITY);
    SET_TOKEN_TYPE(AMP_ENTITY);
    SET_TOKEN_TYPE(NAMED_ENTITY);
    SET_TOKEN_TYPE(HEX_ENTITY);
    SET_TOKEN_TYPE(DECIMAL_ENTITY);
    SET_TOKEN_TYPE(QUOT);
    SET_TOKEN_TYPE(AMP);
    SET_TOKEN_TYPE(LESS);
    SET_TOKEN_TYPE(GREATER);
    SET_TOKEN_TYPE(CRLF);
    SET_TOKEN_TYPE(PRINTABLE);
    SET_TOKEN_TYPE(DEFAULT);
    SET_TOKEN_TYPE(END_OF_FILE);

#undef SET_TOKEN_TYPE

    return hash;
}

VALUE Wikitext_parser_initialize(VALUE self)
{
    // no need to call super here; rb_call_super()
    return self;
}

// for testing and debugging only
VALUE _Wikitext_token(token_t *token)
{
    VALUE object = rb_class_new_instance(0, NULL, cWikitextParserToken);
    (void)rb_iv_set(object, "@start",           LONG2NUM((long)token->start));
    (void)rb_iv_set(object, "@stop",            LONG2NUM((long)token->stop));
    (void)rb_iv_set(object, "@line_start",      LONG2NUM(token->line_start));
    (void)rb_iv_set(object, "@line_stop",       LONG2NUM(token->line_stop));
    (void)rb_iv_set(object, "@column_start",    LONG2NUM(token->column_start));
    (void)rb_iv_set(object, "@column_stop",     LONG2NUM(token->column_stop));
    (void)rb_iv_set(object, "@code_point",      INT2NUM(token->code_point));

    // look-up the token type
    VALUE types = Wikitext_token_types(Qnil);
    VALUE type  = rb_hash_aref(types, INT2FIX(token->type));
    (void)rb_iv_set(object, "@token_type",      type);
    (void)rb_iv_set(object, "@string_value",    rb_str_new(token->start, token->stop - token->start));
    return object;
}

// for testing and debugging only
VALUE Wikitext_parser_tokenize(VALUE self, VALUE string)
{
    if (NIL_P(string))
        return Qnil;
    string = StringValue(string);
    VALUE tokens = rb_ary_new();
    char *p = RSTRING_PTR(string);
    long len = RSTRING_LEN(string);
    char *pe = p + len;
    token_t token;
    next_token(&token, NULL, p, pe);
    rb_ary_push(tokens, _Wikitext_token(&token));
    while (token.type != END_OF_FILE)
    {
        next_token(&token, &token, NULL, pe);
        rb_ary_push(tokens, _Wikitext_token(&token));
    }
    return tokens;
}

// for benchmarking raw tokenization speed only
VALUE Wikitext_parser_benchmarking_tokenize(VALUE self, VALUE string)
{
    if (NIL_P(string))
        return Qnil;
    string = StringValue(string);
    char *p = RSTRING_PTR(string);
    long len = RSTRING_LEN(string);
    char *pe = p + len;
    token_t token;
    next_token(&token, NULL, p, pe);
    while (token.type != END_OF_FILE)
        next_token(&token, &token, NULL, pe);
    return Qnil;
}

void Init_wikitext()
{
    // Wikitext
    mWikitext = rb_define_module("Wikitext");
    rb_define_singleton_method(mWikitext, "token_types", Wikitext_token_types, 0);

    // Wikitext::Error
    eWikitextError = rb_define_class_under(mWikitext, "Error", rb_eException);

    // Wikitext::Parser
    cWikitextParser = rb_define_class_under(mWikitext, "Parser", rb_cObject);
    rb_define_method(cWikitextParser, "initialize", Wikitext_parser_initialize, 0);
    rb_define_method(cWikitextParser, "tokenize", Wikitext_parser_tokenize, 1);
    rb_define_method(cWikitextParser, "benchmarking_tokenize", Wikitext_parser_benchmarking_tokenize, 1);

    // Wikitext::Parser::Token
    cWikitextParserToken = rb_define_class_under(cWikitextParser, "Token", rb_cObject);
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
