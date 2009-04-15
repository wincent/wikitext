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

#include "ruby_compat.h"
#include <stdint.h>     /* uint32_t */

#define TOKEN_TEXT(token)   rb_str_new((const char *)token->start, (token->stop - token->start))
#define TOKEN_LEN(token)    (token->stop - token->start)

typedef struct
{
    char        *start;
    char        *stop;
    size_t      line_start;
    size_t      line_stop;
    size_t      column_start;
    size_t      column_stop;
    uint32_t    code_point;
    int         type;
} token_t;

enum token_types {
    NO_TOKEN,
    P,              // imaginary token (never explicitly marked up)
    LI,             // imaginary token (never explicitly marked up)
    NESTED_LIST,    // imaginary token (never explicitly marked up)
    PRE,
    PRE_START,
    PRE_END,
    NO_WIKI_START,
    NO_WIKI_END,
    BLOCKQUOTE,
    BLOCKQUOTE_START,
    BLOCKQUOTE_END,
    STRONG_EM,
    STRONG_START,
    STRONG_END,
    STRONG,
    EM_START,
    EM_END,
    EM,
    TT_START,
    TT_END,
    TT,
    OL,
    UL,

    // keep these consecutive, and in ascending order
    // (the arithmetic for the base_heading_level feature assumes this)
    H1_START,
    H2_START,
    H3_START,
    H4_START,
    H5_START,
    H6_START,

    // likewise for the H*_END tokens
    H1_END,
    H2_END,
    H3_END,
    H4_END,
    H5_END,
    H6_END,

    URI,
    MAIL,
    PATH,
    LINK_START,
    LINK_END,
    EXT_LINK_START,
    EXT_LINK_END,
    SEPARATOR,
    SPACE,
    QUOT_ENTITY,
    AMP_ENTITY,
    NAMED_ENTITY,
    HEX_ENTITY,
    DECIMAL_ENTITY,
    QUOT,
    AMP,
    LESS,
    GREATER,
    IMG_START,
    IMG_END,
    LEFT_CURLY,
    RIGHT_CURLY,
    CRLF,
    SPECIAL_URI_CHARS,
    PRINTABLE,
    ALNUM,
    DEFAULT,
    END_OF_FILE
};

VALUE Wikitext_parser_token_types(VALUE self);

VALUE _Wikitext_token(token_t *token);
