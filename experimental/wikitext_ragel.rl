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

#define EMIT(t) do { token.type = t; token.column_stop += (pe - p); } while (0)

%%{
    machine wikitext;

#    uri

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
        '>' ' '?
        {
            if (token.column_start == 1 || last_token_type == BLOCKQUOTE)
                EMIT(BLOCKQUOTE);
            else
            {
                EMIT(GREATER);
                token.stop = token.start + 1;
                token.column_stop++;
            }
            fbreak;
        };

        # shorthand for <pre> and </pre>
        # consider adding real <pre> and </pre> HTML tags later on
        ' '
        {
            if (token.column_start == 1)
                EMIT(PRE);
            else
                EMIT(SPACE);
            fbreak;
        };

        ("\r"? "\n") | "\r"
        {
            EMIT(CRLF);
            token.column_stop = 1;
            token.line_stop++;
            fbreak;
        };

    *|;
    
#     " " => { prepare_token... if col == 0, token->type = PRE, else = SPACE }
# 
#     # only if in first column, immediately after another list token, or immediately preceded by blockquote
#     ol                  = "#"; # in action we set token type appropriately, likewise for others below...
#     ul                  = "*";
# 
#     # only if in first column, or immediately after blockquote
#     h1_start            = "=";
#     h2_start            = "==";
#     h3_start            = "===";
#     h4_start            = "====";
#     h5_start            = "=====";
#     h6_start            = "======";
# 
#     # only if last thing on line
#     # see Ragel manual 6.6 "implementing lookahead"
#     h1_end              = "=";
#     h2_end              = "==";
#     h3_end              = "===";
#     h4_end              = "====";
#     h5_end              = "=====";
#     h6_end              = "======";
# 
#     link_start          = "[[";
#     link_end            = "]]";
#     separator           = "|";
#     external_link_start = "[";
#     external_link_end   = "]";
#     space               = " ";
# 
#     quot_entity         = "&quot;";
#     amp_entity          = "&amp;";
#     named_entity        = "&" alpha+ digit* ";";
#     hex_entity          = "&#" [xX] xdigit+ ";";
#     decimal_entity      = "&#" digit+ ";";
# 
#     quot                = '"';
#     amp                 = "&";
#     less                = "<";
#     greater             = ">";
# 
#     crlf                = ("\r"? "\n") | "\r";
#     printable           = printable ascii not explicitly handled above +;
#     # have to be careful here
#     # ragel will try the longest match
#     # so =================== wouldn't match h6 followed by =+
#     # it would just match =+
#     # so have to be careful not to include stuff that can be included in printable
#     # and may have to lose the + on the printable
#     
#     # all the printable ASCII characters (0x20 to 0x7e) excluding those explicitly covered elsewhere
#     # skip space (0x20), quote (0x22), ampersand (0x26), less than (0x3c),greater than (0x3e),
#     # left bracket 0x5b, right bracket 0x5d, backtick (0x60), and vertical bar (0x7c)
#     printable           = (0x21 | 0x23..0x25 | 0x27..0x3b | 0x3d | 0x3f..0x5a | 0x5c | 0x5e..0x5f | 0x61..0x7b | 0x7e)+;
#     # may need to handle =, * and # here as well, and apostrophe  0x3d, 0x2a, 0x23, 0x27
#     # otherwise the longest match thing will bite us
#     # and think about how it interacts with the URI matching
# 
#     # here is where we handle the UTF-8
#     default             = everything else
#                           and unprintable ASCII
# 
#     one_byte_sequence   = byte begins with zero;
#     two_byte_sequence   = first byte begins with 110, next with 10;
#     three_byte_sequence = first byte begins with 1110, next two with 10;
#     four_byte_sequence  = first byte begins with 11110, next three with 10;
#     
#     illegal sequences:
#         11000000x C0 C1 overlong encoding, lead byte of 2 byte seq but code point <= 127
#         
#         11110101  F5, F6, F7 restricted by RFC 3629 lead byte of 4-byte sequence for codepoint above 10ffff
#         1111011x
#     
#         111110xx  F8, F9, FA, FB, FC, FD RFC 3629, lead byte of a seq 5 or 6 bytes long
#         1111110x
#     
#         1111111x  FE, FF invalid, lead byte of a sequence 7 or 8 bytes long
#     

    write data;
}%%

// with a Ragel scanner seeing as it does longest match
// my printable tokens can be runs now
// so i won't have to accumulate them in the parser
// still want the default tokens to be per-char, seeing as I have to convert them into entities

// for now we use the scanner as a tokenizer that returns one token at a time, just like ANTLR
// ultimately we could look at embedding all of the transformation inside the scanner itself (combined scanner/parser)
// pass in the last token because that's useful for the scanner to know
// p data pointer (required by Ragel machine); overriden with contents of last_token if supplied
// pe data end pointer (required by Ragel machine)
token_t next_token(token_t *last_token, char *p, char *pe)
{
    // the token object to be returned
    token_t token;
    token.type              = NO_TOKEN;
    int     last_token_type = NO_TOKEN;
    if (last_token)
    {
        last_token_type     = last_token->type;
        p = last_token->stop;
        token.line_start    = token.line_stop   = last_token->line_stop;
        token.column_start  = token.column_stop = last_token->column_stop;
    }
    else
    {
        token.line_start    = 1;
        token.column_start  = 1;
        token.line_stop     = 1;
        token.column_stop   = 1;
    }
    token.start = p;
    if (p == pe)
    {
        // all done, have reached end of input
        token.stop  = p;
        token.type  = END_OF_FILE;
        return token;
    }

    char    *eof = pe;  // required for backtracking (longest match determination)
    int     cs;         // current state (standard Ragel)
    char    *ts;        // token start (scanner)
    char    *te;        // token end (scanner)
    int     act;        // identity of last patterned matched (scanner)
    %% write init;
    %% write exec;
    if (cs == wikitext_error)
        rb_raise(eWikitextError, "failed before finding a token");
    else if (token.type == NO_TOKEN)
        rb_raise(eWikitextError, "failed to produce a token");
    else
        token.stop = p; // token.type already filled out in action
    return token;
}
