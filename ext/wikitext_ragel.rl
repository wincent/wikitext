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

//----------------------------------------------------------------------//
// NOTE: wikitext_ragel.c is generated from wikitext_ragel.rl, so       //
//       if you make changes to the former they will be overwritten.    //
//       You should perform all your edits in wikitext_ragel.rl.        //
//----------------------------------------------------------------------//

#include "wikitext_ragel.h"
#include "wikitext.h"
#include <stdio.h>

#define EMIT(t)     do { out->type = t; out->stop = p + 1; out->column_stop += (out->stop - out->start); } while (0)
#define MARK()      do { mark = p; } while (0)
#define REWIND()    do { p = mark; } while (0)
#define AT_END()    (p + 1 == pe)
#define DISTANCE()  (p + 1 - ts)
#define NEXT_CHAR() (*(p + 1))

%%{
    machine wikitext;

    action mark
    {
        MARK();
    }

    action non_printable_ascii
    {
        out->code_point = *p & 0x7f;
    }

    action two_byte_utf8_sequence
    {
        out->code_point = ((uint32_t)(*(p - 1)) & 0x1f) << 6 |
            (*p & 0x3f);
    }

    action three_byte_utf8_sequence
    {
        out->code_point = ((uint32_t)(*(p - 2)) & 0x0f) << 12 |
            ((uint32_t)(*(p - 1)) & 0x3f) << 6 |
            (*p & 0x3f);
    }

    action four_byte_utf8_sequence
    {
        out->code_point = ((uint32_t)(*(p - 3)) & 0x07) << 18 |
            ((uint32_t)(*(p - 2)) & 0x3f) << 12 |
            ((uint32_t)(*(p - 1)) & 0x3f) << 6 |
            (*p & 0x3f);
    }

    # simple approximation for matching email addresses; not quite RFC 2822!
    user                = (alnum | [_\.] | '-')+ ;
    tld                 = alpha{2,5} ;
    domain              = (alnum+ '.')+ tld ;
    mail                = user '@' domain ;

    uri_chars           = (alnum | [@$&'(\*\+=%_~/#] | '-')+ ;
    special_uri_chars   = ([:!\(\),;\.\?])+ ;
    uri                 = ('mailto:'i mail) |
                          (('http'i [sS]? '://' | 'ftp://'i | 'svn://'i) uri_chars (special_uri_chars uri_chars)*) ;
    path                = '/' ([a-zA-Z0-9_\-.]+ '/'?)* ;

    main := |*

        '<nowiki>'i
        {
            EMIT(NO_WIKI_START);
            fbreak;
        };

        '</nowiki>'i
        {
            EMIT(NO_WIKI_END);
            fbreak;
        };

        '<pre>'i
        {
            EMIT(PRE_START);
            fbreak;
        };

        '</pre>'i
        {
            EMIT(PRE_END);
            fbreak;
        };

        '<blockquote>'i
        {
            EMIT(BLOCKQUOTE_START);
            fbreak;
        };

        '</blockquote>'i
        {
            EMIT(BLOCKQUOTE_END);
            fbreak;
        };

        "'"{1,5}
        {
            if (DISTANCE() == 5)
                EMIT(STRONG_EM);
            else if (DISTANCE() == 4)
            {
                p--;
                EMIT(STRONG_EM);
            }
            else if (DISTANCE() == 3)
                EMIT(STRONG);
            else if (DISTANCE() == 2)
                EMIT(EM);
            else
                EMIT(PRINTABLE);
            fbreak;
        };

        '<strong>'i
        {
            EMIT(STRONG_START);
            fbreak;
        };

        '</strong>'i
        {
            EMIT(STRONG_END);
            fbreak;
        };

        '<em>'i
        {
            EMIT(EM_START);
            fbreak;
        };

        '</em>'i
        {
            EMIT(EM_END);
            fbreak;
        };

        '`'
        {
            EMIT(TT);
            fbreak;
        };

        '<tt>'i
        {
            EMIT(TT_START);
            fbreak;
        };

        '</tt>'i
        {
            EMIT(TT_END);
            fbreak;
        };

        # shorthand for <blockquote> and </blockquote>
        '>' @mark ' '?
        {
            if (out->column_start == 1 || last_token_type == BLOCKQUOTE)
                EMIT(BLOCKQUOTE);
            else
            {
                REWIND();
                EMIT(GREATER);
            }
            fbreak;
        };

        # shorthand for <pre> and </pre>
        ' ' @mark ' '*
        {
            if (out->column_start == 1 || last_token_type == BLOCKQUOTE)
            {
                REWIND();
                EMIT(PRE);
            }
            else
                EMIT(SPACE);
            fbreak;
        };

        '#'
        {
            if (out->column_start == 1              ||
                last_token_type == OL               ||
                last_token_type == UL               ||
                last_token_type == BLOCKQUOTE       ||
                last_token_type == BLOCKQUOTE_START)
                EMIT(OL);
            else
                EMIT(PRINTABLE);
            fbreak;
        };

        '*'
        {
            if (out->column_start == 1              ||
                last_token_type == OL               ||
                last_token_type == UL               ||
                last_token_type == BLOCKQUOTE       ||
                last_token_type == BLOCKQUOTE_START)
                EMIT(UL);
            else
                EMIT(PRINTABLE);
            fbreak;
        };

        '='+ @mark ' '*
        {
            if (out->column_start == 1 || last_token_type == BLOCKQUOTE || last_token_type == BLOCKQUOTE_START)
            {
                REWIND();
                if (DISTANCE() == 1)
                    EMIT(H1_START);
                else if (DISTANCE() == 2)
                    EMIT(H2_START);
                else if (DISTANCE() == 3)
                    EMIT(H3_START);
                else if (DISTANCE() == 4)
                    EMIT(H4_START);
                else if (DISTANCE() == 5)
                    EMIT(H5_START);
                else if (DISTANCE() == 6)
                    EMIT(H6_START);
                else if (DISTANCE() > 6)
                {
                    p = ts + 6;
                    EMIT(H6_START);
                }
            }
            else if (AT_END() || NEXT_CHAR() == '\n' || NEXT_CHAR() == '\r')
            {
                REWIND();
                if (DISTANCE() == 1)
                    EMIT(H1_END);
                else if (DISTANCE() == 2)
                    EMIT(H2_END);
                else if (DISTANCE() == 3)
                    EMIT(H3_END);
                else if (DISTANCE() == 4)
                    EMIT(H4_END);
                else if (DISTANCE() == 5)
                    EMIT(H5_END);
                else if (DISTANCE() == 6)
                    EMIT(H6_END);
                else if (DISTANCE() > 6)
                {
                    p -= 6; // will scan the H6 on the next scan
                    EMIT(PRINTABLE);
                }
            }
            else
            {
                // note that a H*_END token will never match before a BLOCKQUOTE_END
                REWIND();
                EMIT(PRINTABLE);
            }
            fbreak;
        };

        uri
        {
            EMIT(URI);
            fbreak;
        };

        mail
        {
            EMIT(MAIL);
            fbreak;
        };

        path
        {
            EMIT(PATH);
            fbreak;
        };

        '[['
        {
            EMIT(LINK_START);
            fbreak;
        };

        ']]'
        {
            EMIT(LINK_END);
            fbreak;
        };

        '|'
        {
            EMIT(SEPARATOR);
            fbreak;
        };

        '['
        {
            EMIT(EXT_LINK_START);
            fbreak;
        };

        ']'
        {
            EMIT(EXT_LINK_END);
            fbreak;
        };

        '&quot;'
        {
            EMIT(QUOT_ENTITY);
            fbreak;
        };

        '&amp;'
        {
            EMIT(AMP_ENTITY);
            fbreak;
        };

        '&' alpha+ digit* ';'
        {
            EMIT(NAMED_ENTITY);
            fbreak;
        };

        '&#' [xX] xdigit+ ';'
        {
            EMIT(HEX_ENTITY);
            fbreak;
        };

        '&#' digit+ ';'
        {
            EMIT(DECIMAL_ENTITY);
            fbreak;
        };

        '"'
        {
            EMIT(QUOT);
            fbreak;
        };

        '&'
        {
            EMIT(AMP);
            fbreak;
        };

        '<'
        {
            EMIT(LESS);
            fbreak;
        };

        '>'
        {
            EMIT(GREATER);
            fbreak;
        };

        '{{'
        {
            EMIT(IMG_START);
            fbreak;
        };

        '}}'
        {
            EMIT(IMG_END);
            fbreak;
        };

        '{'
        {
            EMIT(LEFT_CURLY);
            fbreak;
        };

        '}'
        {
            EMIT(RIGHT_CURLY);
            fbreak;
        };

        ("\r"? "\n") | "\r"
        {
            EMIT(CRLF);
            out->column_stop = 1;
            out->line_stop++;
            fbreak;
        };

        # must tokenize these separately from the other PRINTABLE characters otherwise a string like:
        #   See http://example.com/.
        # will get greedily tokenized as PRINABLE, SPACE, PRINTABLE rather than PRINTABLE, SPACE, URI, SPECIAL_URI_CHARS
        # this also applies to MAIL tokenization and input strings like:
        #   Email me (user@example.com) for more info.
        special_uri_chars
        {
            EMIT(SPECIAL_URI_CHARS);
            fbreak;
        };

        alnum+
        {
            EMIT(ALNUM);
            fbreak;
        };

        # all the printable ASCII characters (0x20 to 0x7e) excluding those explicitly covered elsewhere:
        # we skip space (0x20), exclamation mark (0x21), quote (0x22), hash (0x23), ampersand (0x26), apostrophe (0x27),
        # left parenthesis (0x28), right parenthesis (0x29), numbers (0x30..0x39), asterisk (0x2a), comma (0x2c), period (0x2e),
        # colon (0x3a), semi-colon (0x3b), less than (0x3c), equals (0x3d), greater than (0x3e), question mark (0x3f), uppercase
        # letters (0x41..0x5a), left bracket (0x5b), right bracket (0x5d), backtick (0x60), lowercase letters (0x61..0x7a), left
        # curly brace (0x7b), vertical bar (0x7c) and right curly brace (0x7d).
        (0x24..0x25 | 0x2b | 0x2d | 0x2f | 0x40 | 0x5c | 0x5e..0x5f | 0x7e)+
        {
            EMIT(PRINTABLE);
            fbreak;
        };

        # here is where we handle the UTF-8 and everything else
        #
        #     one_byte_sequence   = byte begins with zero;
        #     two_byte_sequence   = first byte begins with 110 (0xc0..0xdf), next with 10 (0x80..9xbf);
        #     three_byte_sequence = first byte begins with 1110 (0xe0..0xef), next two with 10 (0x80..9xbf);
        #     four_byte_sequence  = first byte begins with 11110 (0xf0..0xf7), next three with 10 (0x80..9xbf);
        #
        #     within the ranges specified, we also exclude these illegal sequences:
        #       1100000x (c0 c1)    overlong encoding, lead byte of 2 byte seq but code point <= 127
        #       11110101 (f5)       restricted by RFC 3629 lead byte of 4-byte sequence for codepoint above 10ffff
        #       1111011x (f6, f7)   restricted by RFC 3629 lead byte of 4-byte sequence for codepoint above 10ffff
        (0x01..0x1f | 0x7f)                             @non_printable_ascii        |
        (0xc2..0xdf 0x80..0xbf)                         @two_byte_utf8_sequence     |
        (0xe0..0xef 0x80..0xbf 0x80..0xbf)              @three_byte_utf8_sequence   |
        (0xf0..0xf4 0x80..0xbf 0x80..0xbf 0x80..0xbf)   @four_byte_utf8_sequence
        {
            EMIT(DEFAULT);
            out->column_stop = out->column_start + 1;
            fbreak;
        };

    *|;

    write data;
}%%

// for now we use the scanner as a tokenizer that returns one token at a time, just like ANTLR
// ultimately we could look at embedding all of the transformation inside the scanner itself (combined scanner/parser)
// pass in the last token because that's useful for the scanner to know
// p data pointer (required by Ragel machine); overriden with contents of last_token if supplied
// pe data end pointer (required by Ragel machine)
void next_token(token_t *out, token_t *last_token, char *p, char *pe)
{
    int last_token_type = NO_TOKEN;
    if (last_token)
    {
        last_token_type     = last_token->type;
        p = last_token->stop;
        out->line_start     = out->line_stop    = last_token->line_stop;
        out->column_start   = out->column_stop  = last_token->column_stop;
    }
    else
    {
        out->line_start     = 1;
        out->column_start   = 1;
        out->line_stop      = 1;
        out->column_stop    = 1;
    }
    out->type       = NO_TOKEN;
    out->code_point = 0;
    out->start      = p;
    if (p == pe)
    {
        // all done, have reached end of input
        out->stop  = p;
        out->type  = END_OF_FILE;
        return;
    }

    char    *mark;      // for manual backtracking
    char    *eof = pe;  // required for backtracking (longest match determination)
    int     cs;         // current state (standard Ragel)
    char    *ts;        // token start (scanner)
    char    *te;        // token end (scanner)
    int     act;        // identity of last patterned matched (scanner)
    %% write init;
    %% write exec;
    if (cs == wikitext_error)
        rb_raise(eWikitextParserError, "failed before finding a token");
    else if (out->type == NO_TOKEN)
        rb_raise(eWikitextParserError, "failed to produce a token");
}
