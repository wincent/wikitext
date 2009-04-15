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

#include "token.h"
#include "wikitext.h"

// return a hash of token types
// we make this available for unit testing purposes

VALUE Wikitext_parser_token_types(VALUE self)
{
    VALUE hash = rb_hash_new();

#define SET_TOKEN_TYPE(identifier)  (void)rb_hash_aset(hash, INT2FIX(identifier), \
    rb_funcall(rb_funcall(rb_str_new2(#identifier), rb_intern("downcase"), 0), rb_intern("to_sym"), 0))

    SET_TOKEN_TYPE(NO_TOKEN);
    SET_TOKEN_TYPE(P);
    SET_TOKEN_TYPE(LI);
    SET_TOKEN_TYPE(NESTED_LIST);
    SET_TOKEN_TYPE(PRE);
    SET_TOKEN_TYPE(PRE_START);
    SET_TOKEN_TYPE(PRE_END);
    SET_TOKEN_TYPE(NO_WIKI_START);
    SET_TOKEN_TYPE(NO_WIKI_END);
    SET_TOKEN_TYPE(BLOCKQUOTE);
    SET_TOKEN_TYPE(BLOCKQUOTE_START);
    SET_TOKEN_TYPE(BLOCKQUOTE_END);
    SET_TOKEN_TYPE(STRONG_EM);
    SET_TOKEN_TYPE(STRONG_START);
    SET_TOKEN_TYPE(STRONG_END);
    SET_TOKEN_TYPE(STRONG);
    SET_TOKEN_TYPE(EM_START);
    SET_TOKEN_TYPE(EM_END);
    SET_TOKEN_TYPE(EM);
    SET_TOKEN_TYPE(TT_START);
    SET_TOKEN_TYPE(TT_END);
    SET_TOKEN_TYPE(TT);
    SET_TOKEN_TYPE(OL);
    SET_TOKEN_TYPE(UL);
    SET_TOKEN_TYPE(H1_START);
    SET_TOKEN_TYPE(H2_START);
    SET_TOKEN_TYPE(H3_START);
    SET_TOKEN_TYPE(H4_START);
    SET_TOKEN_TYPE(H5_START);
    SET_TOKEN_TYPE(H6_START);
    SET_TOKEN_TYPE(H1_END);
    SET_TOKEN_TYPE(H2_END);
    SET_TOKEN_TYPE(H3_END);
    SET_TOKEN_TYPE(H4_END);
    SET_TOKEN_TYPE(H5_END);
    SET_TOKEN_TYPE(H6_END);
    SET_TOKEN_TYPE(URI);
    SET_TOKEN_TYPE(MAIL);
    SET_TOKEN_TYPE(PATH);
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
    SET_TOKEN_TYPE(IMG_START);
    SET_TOKEN_TYPE(IMG_END);
    SET_TOKEN_TYPE(LEFT_CURLY);
    SET_TOKEN_TYPE(RIGHT_CURLY);
    SET_TOKEN_TYPE(CRLF);
    SET_TOKEN_TYPE(SPECIAL_URI_CHARS);
    SET_TOKEN_TYPE(PRINTABLE);
    SET_TOKEN_TYPE(ALNUM);
    SET_TOKEN_TYPE(DEFAULT);
    SET_TOKEN_TYPE(END_OF_FILE);

#undef SET_TOKEN_TYPE

    return hash;
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
    VALUE types = Wikitext_parser_token_types(Qnil);
    VALUE type  = rb_hash_aref(types, INT2FIX(token->type));
    (void)rb_iv_set(object, "@token_type",      type);
    (void)rb_iv_set(object, "@string_value",    rb_str_new(token->start, token->stop - token->start));
    return object;
}
