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

//----------------------------------------------------------------------//
// NOTE: wikitext_ragel.c is generated from wikitext_ragel.rl, so       //
//       if you make changes to the former they will be overwritten.    //
//       You should perform all your edits in wikitext_ragel.rl.        //
//----------------------------------------------------------------------//

#include "wikitext_ragel.h"
#include <stdio.h>

#define EMIT(t)     do { out->type = t; out->stop = p + 1; out->column_stop += (out->stop - out->start); } while (0)
#define MARK()      do { mark = p; } while (0)
#define REWIND()    do { p = mark; } while (0)

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

    uri_chars           = (alnum | [@$&'(\*\+=%_~/#] | '-')+ ;
    special_uri_chars   = ([:!),;\.\?])+ ;
    uri                 = (/http:\/\//i | /ftp:\/\//i | /svn:\/\//i) uri_chars (special_uri_chars uri_chars)* ;

    main := |*

        /<nowiki>/i
        {
            EMIT(NO_WIKI_START);
            fbreak;
        };

        /<\/nowiki>/i   # this comment because the Ragel TextMate bundle miscolorizes without it /
        {
            EMIT(NO_WIKI_END);
            fbreak;
        };

        "'"{5}
        {
            EMIT(STRONG_EM);
            fbreak;
        };

        "'"{3}
        {
            EMIT(STRONG);
            fbreak;
        };

        "'"{2}
        {
            EMIT(EM);
            fbreak;
        };

        '`'
        {
            EMIT(TT);
            fbreak;
        };

        /<tt>/i
        {
            EMIT(TT_START);
            fbreak;
        };

        /<\/tt>/i   # this comment because the Ragel TextMate bundle miscolorizes without it /
        {
            EMIT(TT_END);
            fbreak;
        };

        # consider adding <blockquote></blockquote> HTML tags as well later on
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
        # consider adding real <pre> and </pre> HTML tags later on
        ' '
        {
            if (out->column_start == 1)
                EMIT(PRE);
            else
                EMIT(SPACE);
            fbreak;
        };

        '#'
        {
            if (out->column_start == 1 || last_token_type == OL || last_token_type == UL || last_token_type == BLOCKQUOTE)
                EMIT(OL);
            else
                EMIT(PRINTABLE);
            fbreak;
        };

        '*'
        {
            if (out->column_start == 1 || last_token_type == OL || last_token_type == UL || last_token_type == BLOCKQUOTE)
                EMIT(UL);
            else
                EMIT(PRINTABLE);
            fbreak;
        };

        '='{6} @mark ' '*
        {
            if (out->column_start == 1 || last_token_type == BLOCKQUOTE)
            {
                REWIND();
                EMIT(H6_START);
            }
            else if (p > mark && (p == pe || *(p + 1) == '\n' || *(p + 1) == '\r'))
            {
                REWIND();
                EMIT(H6_END);
            }
            else
                EMIT(PRINTABLE);
            fbreak;
        };

        '='{5} @mark ' '*
        {
            if (out->column_start == 1 || last_token_type == BLOCKQUOTE)
            {
                REWIND();
                EMIT(H5_START);
            }
            else if (p > mark && (p == pe || *(p + 1) == '\n' || *(p + 1) == '\r'))
            {
                REWIND();
                EMIT(H6_END);
            }
                EMIT(PRINTABLE);
            fbreak;
        };

        '='{4} @mark ' '*
        {
            if (out->column_start == 1 || last_token_type == BLOCKQUOTE)
            {
                REWIND();
                EMIT(H4_START);
            }
            else if (p > mark && (p == pe || *(p + 1) == '\n' || *(p + 1) == '\r'))
            {
                REWIND();
                EMIT(H4_END);
            }
            else
                EMIT(PRINTABLE);
            fbreak;
        };

        '='{3} @mark ' '*
        {
            if (out->column_start == 1 || last_token_type == BLOCKQUOTE)
            {
                REWIND();
                EMIT(H3_START);
            }
            else if (p > mark && (p == pe || *(p + 1) == '\n' || *(p + 1) == '\r'))
            {
                REWIND();
                EMIT(H3_END);
            }
            else
                EMIT(PRINTABLE);
            fbreak;
        };

        '='{2} @mark ' '*
        {
            if (out->column_start == 1 || last_token_type == BLOCKQUOTE)
            {
                REWIND();
                EMIT(H2_START);
            }
            else if (p > mark && (p == pe || *(p + 1) == '\n' || *(p + 1) == '\r'))
            {
                REWIND();
                EMIT(H2_END);
            }
            else
                EMIT(PRINTABLE);
            fbreak;
        };

        '=' @mark ' '*
        {
            if (out->column_start == 1 || last_token_type == BLOCKQUOTE)
            {
                REWIND();
                EMIT(H1_START);
            }
            else if (p > mark && (p == pe || *(p + 1) == '\n' || *(p + 1) == '\r'))
            {
                REWIND();
                EMIT(H1_END);
            }
            else
                EMIT(PRINTABLE);
            fbreak;
        };

        uri
        {
            EMIT(URI);
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

        ("\r"? "\n") | "\r"
        {
            EMIT(CRLF);
            out->column_stop = 1;
            out->line_stop++;
            fbreak;
        };

        # all the printable ASCII characters (0x20 to 0x7e) excluding those explicitly covered elsewhere:
        # skip space (0x20), quote (0x22), ampersand (0x26), less than (0x3c), greater than (0x3e),
        # left bracket 0x5b, right bracket 0x5d, backtick (0x60), and vertical bar (0x7c)
        (0x21 | 0x23..0x25 | 0x27..0x3b | 0x3d | 0x3f..0x5a | 0x5c | 0x5e..0x5f | 0x61..0x7b | 0x7e)+
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
        (0x01..0x1f | 0x7f)                             %non_printable_ascii        |
        (0xc2..0xdf 0x80..0xbf)                         %two_byte_utf8_sequence     |
        (0xe0..0xef 0x80..0xbf 0x80..0xbf)              %three_byte_utf8_sequence   |
        (0xf0..0xf4 0x80..0xbf 0x80..0xbf 0x80..0xbf)   %four_byte_utf8_sequence
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
        rb_raise(eWikitextError, "failed before finding a token");
    else if (out->type == NO_TOKEN)
        rb_raise(eWikitextError, "failed to produce a token");
}
