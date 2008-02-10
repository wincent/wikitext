#line 1 "wikitext_ragel.rl"
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
#include "wikitext.h"
#include <stdio.h>

#define EMIT(t)     do { out->type = t; out->stop = p + 1; out->column_stop += (out->stop - out->start); } while (0)
#define MARK()      do { mark = p; } while (0)
#define REWIND()    do { p = mark; } while (0)
#define AT_END()    (p + 1 == pe)
#define DISTANCE()  (p + 1 - ts)
#define NEXT_CHAR() (*(p + 1))


#line 35 "wikitext_ragel.c"
static const int wikitext_start = 60;
static const int wikitext_first_final = 60;
static const int wikitext_error = 0;

static const int wikitext_en_main = 60;

#line 398 "wikitext_ragel.rl"


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
    
#line 85 "wikitext_ragel.c"
	{
	cs = wikitext_start;
	ts = 0;
	te = 0;
	act = 0;
	}
#line 440 "wikitext_ragel.rl"
    
#line 94 "wikitext_ragel.c"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
tr0:
#line 46 "wikitext_ragel.rl"
	{
        out->code_point = ((uint32_t)(*(p - 1)) & 0x1f) << 6 |
            (*p & 0x3f);
    }
#line 389 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(DEFAULT);
            out->column_stop = out->column_start + 1;
            {p++; cs = 60; goto _out;}
        }}
	goto st60;
tr3:
#line 52 "wikitext_ragel.rl"
	{
        out->code_point = ((uint32_t)(*(p - 2)) & 0x0f) << 12 |
            ((uint32_t)(*(p - 1)) & 0x3f) << 6 |
            (*p & 0x3f);
    }
#line 389 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(DEFAULT);
            out->column_stop = out->column_start + 1;
            {p++; cs = 60; goto _out;}
        }}
	goto st60;
tr6:
#line 59 "wikitext_ragel.rl"
	{
        out->code_point = ((uint32_t)(*(p - 3)) & 0x07) << 18 |
            ((uint32_t)(*(p - 2)) & 0x3f) << 12 |
            ((uint32_t)(*(p - 1)) & 0x3f) << 6 |
            (*p & 0x3f);
    }
#line 389 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(DEFAULT);
            out->column_stop = out->column_start + 1;
            {p++; cs = 60; goto _out;}
        }}
	goto st60;
tr7:
#line 339 "wikitext_ragel.rl"
	{{p = ((te))-1;}{
            EMIT(AMP);
            {p++; cs = 60; goto _out;}
        }}
	goto st60;
tr10:
#line 327 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(DECIMAL_ENTITY);
            {p++; cs = 60; goto _out;}
        }}
	goto st60;
tr12:
#line 321 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(HEX_ENTITY);
            {p++; cs = 60; goto _out;}
        }}
	goto st60;
tr14:
#line 315 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(NAMED_ENTITY);
            {p++; cs = 60; goto _out;}
        }}
	goto st60;
tr18:
#line 309 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(AMP_ENTITY);
            {p++; cs = 60; goto _out;}
        }}
	goto st60;
tr22:
#line 303 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(QUOT_ENTITY);
            {p++; cs = 60; goto _out;}
        }}
	goto st60;
tr23:
#line 345 "wikitext_ragel.rl"
	{{p = ((te))-1;}{
            EMIT(LESS);
            {p++; cs = 60; goto _out;}
        }}
	goto st60;
tr30:
#line 140 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(EM_END);
            {p++; cs = 60; goto _out;}
        }}
	goto st60;
tr36:
#line 86 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(NO_WIKI_END);
            {p++; cs = 60; goto _out;}
        }}
	goto st60;
tr39:
#line 98 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(PRE_END);
            {p++; cs = 60; goto _out;}
        }}
	goto st60;
tr45:
#line 128 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(STRONG_END);
            {p++; cs = 60; goto _out;}
        }}
	goto st60;
tr47:
#line 158 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(TT_END);
            {p++; cs = 60; goto _out;}
        }}
	goto st60;
tr49:
#line 134 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(EM_START);
            {p++; cs = 60; goto _out;}
        }}
	goto st60;
tr55:
#line 80 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(NO_WIKI_START);
            {p++; cs = 60; goto _out;}
        }}
	goto st60;
tr58:
#line 92 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(PRE_START);
            {p++; cs = 60; goto _out;}
        }}
	goto st60;
tr64:
#line 122 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(STRONG_START);
            {p++; cs = 60; goto _out;}
        }}
	goto st60;
tr66:
#line 152 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(TT_START);
            {p++; cs = 60; goto _out;}
        }}
	goto st60;
tr67:
#line 261 "wikitext_ragel.rl"
	{{p = ((te))-1;}{
            EMIT(URI);
            {p++; cs = 60; goto _out;}
        }}
	goto st60;
tr73:
#line 41 "wikitext_ragel.rl"
	{
        out->code_point = *p & 0x7f;
    }
#line 389 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(DEFAULT);
            out->column_stop = out->column_start + 1;
            {p++; cs = 60; goto _out;}
        }}
	goto st60;
tr74:
#line 357 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(CRLF);
            out->column_stop = 1;
            out->line_stop++;
            {p++; cs = 60; goto _out;}
        }}
#line 41 "wikitext_ragel.rl"
	{
        out->code_point = *p & 0x7f;
    }
	goto st60;
tr78:
#line 333 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(QUOT);
            {p++; cs = 60; goto _out;}
        }}
	goto st60;
tr79:
#line 191 "wikitext_ragel.rl"
	{te = p+1;{
            if (out->column_start == 1 || last_token_type == OL || last_token_type == UL || last_token_type == BLOCKQUOTE)
                EMIT(OL);
            else
                EMIT(PRINTABLE);
            {p++; cs = 60; goto _out;}
        }}
	goto st60;
tr82:
#line 200 "wikitext_ragel.rl"
	{te = p+1;{
            if (out->column_start == 1 || last_token_type == OL || last_token_type == UL || last_token_type == BLOCKQUOTE)
                EMIT(UL);
            else
                EMIT(PRINTABLE);
            {p++; cs = 60; goto _out;}
        }}
	goto st60;
tr93:
#line 146 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(TT);
            {p++; cs = 60; goto _out;}
        }}
	goto st60;
tr94:
#line 285 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(SEPARATOR);
            {p++; cs = 60; goto _out;}
        }}
	goto st60;
tr95:
#line 357 "wikitext_ragel.rl"
	{te = p;p--;{
            EMIT(CRLF);
            out->column_stop = 1;
            out->line_stop++;
            {p++; cs = 60; goto _out;}
        }}
	goto st60;
tr96:
#line 357 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(CRLF);
            out->column_stop = 1;
            out->line_stop++;
            {p++; cs = 60; goto _out;}
        }}
	goto st60;
tr97:
#line 179 "wikitext_ragel.rl"
	{te = p;p--;{
            if (out->column_start == 1 || last_token_type == BLOCKQUOTE)
            {
                REWIND();
                EMIT(PRE);
            }
            else
                EMIT(SPACE);
            {p++; cs = 60; goto _out;}
        }}
	goto st60;
tr99:
#line 369 "wikitext_ragel.rl"
	{te = p;p--;{
            EMIT(PRINTABLE);
            {p++; cs = 60; goto _out;}
        }}
	goto st60;
tr100:
#line 339 "wikitext_ragel.rl"
	{te = p;p--;{
            EMIT(AMP);
            {p++; cs = 60; goto _out;}
        }}
	goto st60;
tr104:
#line 104 "wikitext_ragel.rl"
	{te = p;p--;{
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
            {p++; cs = 60; goto _out;}
        }}
	goto st60;
tr108:
#line 104 "wikitext_ragel.rl"
	{te = p+1;{
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
            {p++; cs = 60; goto _out;}
        }}
	goto st60;
tr111:
#line 1 "wikitext_ragel.rl"
	{	switch( act ) {
	case 18:
	{{p = ((te))-1;}
            EMIT(URI);
            {p++; cs = 60; goto _out;}
        }
	break;
	case 19:
	{{p = ((te))-1;}
            EMIT(MAIL);
            {p++; cs = 60; goto _out;}
        }
	break;
	case 35:
	{{p = ((te))-1;}
            EMIT(PRINTABLE);
            {p++; cs = 60; goto _out;}
        }
	break;
	default: break;
	}
	}
	goto st60;
tr115:
#line 267 "wikitext_ragel.rl"
	{te = p;p--;{
            EMIT(MAIL);
            {p++; cs = 60; goto _out;}
        }}
	goto st60;
tr119:
#line 345 "wikitext_ragel.rl"
	{te = p;p--;{
            EMIT(LESS);
            {p++; cs = 60; goto _out;}
        }}
	goto st60;
tr126:
#line 209 "wikitext_ragel.rl"
	{te = p;p--;{
            if (out->column_start == 1 || last_token_type == BLOCKQUOTE)
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
                REWIND();
                EMIT(PRINTABLE);
            }
            {p++; cs = 60; goto _out;}
        }}
	goto st60;
tr128:
#line 165 "wikitext_ragel.rl"
	{te = p;p--;{
            if (out->column_start == 1 || last_token_type == BLOCKQUOTE)
                EMIT(BLOCKQUOTE);
            else
            {
                REWIND();
                EMIT(GREATER);
            }
            {p++; cs = 60; goto _out;}
        }}
	goto st60;
tr129:
#line 165 "wikitext_ragel.rl"
	{te = p+1;{
            if (out->column_start == 1 || last_token_type == BLOCKQUOTE)
                EMIT(BLOCKQUOTE);
            else
            {
                REWIND();
                EMIT(GREATER);
            }
            {p++; cs = 60; goto _out;}
        }}
	goto st60;
tr136:
#line 261 "wikitext_ragel.rl"
	{te = p;p--;{
            EMIT(URI);
            {p++; cs = 60; goto _out;}
        }}
	goto st60;
tr157:
#line 291 "wikitext_ragel.rl"
	{te = p;p--;{
            EMIT(EXT_LINK_START);
            {p++; cs = 60; goto _out;}
        }}
	goto st60;
tr158:
#line 273 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(LINK_START);
            {p++; cs = 60; goto _out;}
        }}
	goto st60;
tr159:
#line 297 "wikitext_ragel.rl"
	{te = p;p--;{
            EMIT(EXT_LINK_END);
            {p++; cs = 60; goto _out;}
        }}
	goto st60;
tr160:
#line 279 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(LINK_END);
            {p++; cs = 60; goto _out;}
        }}
	goto st60;
st60:
#line 1 "wikitext_ragel.rl"
	{ts = 0;}
	if ( ++p == pe )
		goto _test_eof60;
case 60:
#line 1 "wikitext_ragel.rl"
	{ts = p;}
#line 578 "wikitext_ragel.c"
	switch( (*p) ) {
		case 10: goto tr74;
		case 13: goto tr75;
		case 32: goto tr76;
		case 34: goto tr78;
		case 35: goto tr79;
		case 38: goto tr80;
		case 39: goto st65;
		case 42: goto tr82;
		case 47: goto st63;
		case 60: goto tr84;
		case 61: goto tr85;
		case 62: goto tr86;
		case 70: goto st81;
		case 72: goto st89;
		case 77: goto st93;
		case 83: goto st108;
		case 91: goto st110;
		case 92: goto st63;
		case 93: goto st111;
		case 94: goto st63;
		case 96: goto tr93;
		case 102: goto st81;
		case 104: goto st89;
		case 109: goto st93;
		case 115: goto st108;
		case 124: goto tr94;
		case 127: goto tr73;
	}
	if ( (*p) < 33 ) {
		if ( (*p) < -32 ) {
			if ( -62 <= (*p) && (*p) <= -33 )
				goto st1;
		} else if ( (*p) > -17 ) {
			if ( (*p) > -12 ) {
				if ( 1 <= (*p) && (*p) <= 31 )
					goto tr73;
			} else if ( (*p) >= -16 )
				goto st4;
		} else
			goto st2;
	} else if ( (*p) > 44 ) {
		if ( (*p) < 58 ) {
			if ( 45 <= (*p) && (*p) <= 57 )
				goto st69;
		} else if ( (*p) > 64 ) {
			if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 126 )
					goto st63;
			} else if ( (*p) >= 65 )
				goto st69;
		} else
			goto st63;
	} else
		goto st63;
	goto st0;
st0:
cs = 0;
	goto _out;
st1:
	if ( ++p == pe )
		goto _test_eof1;
case 1:
	if ( (*p) <= -65 )
		goto tr0;
	goto st0;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	if ( (*p) <= -65 )
		goto st3;
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	if ( (*p) <= -65 )
		goto tr3;
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	if ( (*p) <= -65 )
		goto st5;
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	if ( (*p) <= -65 )
		goto st6;
	goto st0;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	if ( (*p) <= -65 )
		goto tr6;
	goto st0;
tr75:
#line 41 "wikitext_ragel.rl"
	{
        out->code_point = *p & 0x7f;
    }
	goto st61;
st61:
	if ( ++p == pe )
		goto _test_eof61;
case 61:
#line 690 "wikitext_ragel.c"
	if ( (*p) == 10 )
		goto tr96;
	goto tr95;
tr76:
#line 36 "wikitext_ragel.rl"
	{
        MARK();
    }
	goto st62;
st62:
	if ( ++p == pe )
		goto _test_eof62;
case 62:
#line 704 "wikitext_ragel.c"
	if ( (*p) == 32 )
		goto st62;
	goto tr97;
st63:
	if ( ++p == pe )
		goto _test_eof63;
case 63:
	switch( (*p) ) {
		case 33: goto st63;
		case 92: goto st63;
	}
	if ( (*p) < 63 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st63;
		} else if ( (*p) > 41 ) {
			if ( 43 <= (*p) && (*p) <= 59 )
				goto st63;
		} else
			goto st63;
	} else if ( (*p) > 90 ) {
		if ( (*p) < 97 ) {
			if ( 94 <= (*p) && (*p) <= 95 )
				goto st63;
		} else if ( (*p) > 123 ) {
			if ( 125 <= (*p) && (*p) <= 126 )
				goto st63;
		} else
			goto st63;
	} else
		goto st63;
	goto tr99;
tr80:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
	goto st64;
st64:
	if ( ++p == pe )
		goto _test_eof64;
case 64:
#line 745 "wikitext_ragel.c"
	switch( (*p) ) {
		case 35: goto st7;
		case 97: goto st13;
		case 113: goto st16;
	}
	if ( (*p) > 90 ) {
		if ( 98 <= (*p) && (*p) <= 122 )
			goto st11;
	} else if ( (*p) >= 65 )
		goto st11;
	goto tr100;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	switch( (*p) ) {
		case 88: goto st9;
		case 120: goto st9;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st8;
	goto tr7;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	if ( (*p) == 59 )
		goto tr10;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st8;
	goto tr7;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st10;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st10;
	} else
		goto st10;
	goto tr7;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	if ( (*p) == 59 )
		goto tr12;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st10;
	} else if ( (*p) > 70 ) {
		if ( 97 <= (*p) && (*p) <= 102 )
			goto st10;
	} else
		goto st10;
	goto tr7;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	if ( (*p) == 59 )
		goto tr14;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st12;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st11;
	} else
		goto st11;
	goto tr7;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	if ( (*p) == 59 )
		goto tr14;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st12;
	goto tr7;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
	switch( (*p) ) {
		case 59: goto tr14;
		case 109: goto st14;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st12;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st11;
	} else
		goto st11;
	goto tr7;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
	switch( (*p) ) {
		case 59: goto tr14;
		case 112: goto st15;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st12;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st11;
	} else
		goto st11;
	goto tr7;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
	if ( (*p) == 59 )
		goto tr18;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st12;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st11;
	} else
		goto st11;
	goto tr7;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
	switch( (*p) ) {
		case 59: goto tr14;
		case 117: goto st17;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st12;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st11;
	} else
		goto st11;
	goto tr7;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
	switch( (*p) ) {
		case 59: goto tr14;
		case 111: goto st18;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st12;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st11;
	} else
		goto st11;
	goto tr7;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
	switch( (*p) ) {
		case 59: goto tr14;
		case 116: goto st19;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st12;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st11;
	} else
		goto st11;
	goto tr7;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
	if ( (*p) == 59 )
		goto tr22;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st12;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st11;
	} else
		goto st11;
	goto tr7;
st65:
	if ( ++p == pe )
		goto _test_eof65;
case 65:
	if ( (*p) == 39 )
		goto st66;
	goto tr104;
st66:
	if ( ++p == pe )
		goto _test_eof66;
case 66:
	if ( (*p) == 39 )
		goto st67;
	goto tr104;
st67:
	if ( ++p == pe )
		goto _test_eof67;
case 67:
	if ( (*p) == 39 )
		goto st68;
	goto tr104;
st68:
	if ( ++p == pe )
		goto _test_eof68;
case 68:
	if ( (*p) == 39 )
		goto tr108;
	goto tr104;
st69:
	if ( ++p == pe )
		goto _test_eof69;
case 69:
	switch( (*p) ) {
		case 33: goto st63;
		case 47: goto st63;
		case 63: goto st63;
		case 64: goto st70;
		case 92: goto st63;
		case 94: goto st63;
		case 95: goto st69;
		case 123: goto st63;
	}
	if ( (*p) < 45 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st63;
		} else if ( (*p) > 41 ) {
			if ( 43 <= (*p) && (*p) <= 44 )
				goto st63;
		} else
			goto st63;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 65 ) {
			if ( 58 <= (*p) && (*p) <= 59 )
				goto st63;
		} else if ( (*p) > 90 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st63;
			} else if ( (*p) >= 97 )
				goto st69;
		} else
			goto st69;
	} else
		goto st69;
	goto tr99;
st70:
	if ( ++p == pe )
		goto _test_eof70;
case 70:
	switch( (*p) ) {
		case 33: goto st63;
		case 92: goto st63;
		case 123: goto st63;
	}
	if ( (*p) < 58 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st63;
		} else if ( (*p) > 41 ) {
			if ( (*p) > 47 ) {
				if ( 48 <= (*p) && (*p) <= 57 )
					goto tr110;
			} else if ( (*p) >= 43 )
				goto st63;
		} else
			goto st63;
	} else if ( (*p) > 59 ) {
		if ( (*p) < 94 ) {
			if ( (*p) > 64 ) {
				if ( 65 <= (*p) && (*p) <= 90 )
					goto tr110;
			} else if ( (*p) >= 63 )
				goto st63;
		} else if ( (*p) > 95 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st63;
			} else if ( (*p) >= 97 )
				goto tr110;
		} else
			goto st63;
	} else
		goto st63;
	goto tr99;
tr110:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
#line 369 "wikitext_ragel.rl"
	{act = 35;}
	goto st71;
tr118:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
#line 267 "wikitext_ragel.rl"
	{act = 19;}
	goto st71;
st71:
	if ( ++p == pe )
		goto _test_eof71;
case 71:
#line 1065 "wikitext_ragel.c"
	switch( (*p) ) {
		case 33: goto st63;
		case 46: goto st72;
		case 92: goto st63;
		case 123: goto st63;
	}
	if ( (*p) < 58 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st63;
		} else if ( (*p) > 41 ) {
			if ( (*p) > 47 ) {
				if ( 48 <= (*p) && (*p) <= 57 )
					goto tr110;
			} else if ( (*p) >= 43 )
				goto st63;
		} else
			goto st63;
	} else if ( (*p) > 59 ) {
		if ( (*p) < 94 ) {
			if ( (*p) > 64 ) {
				if ( 65 <= (*p) && (*p) <= 90 )
					goto tr110;
			} else if ( (*p) >= 63 )
				goto st63;
		} else if ( (*p) > 95 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st63;
			} else if ( (*p) >= 97 )
				goto tr110;
		} else
			goto st63;
	} else
		goto st63;
	goto tr111;
st72:
	if ( ++p == pe )
		goto _test_eof72;
case 72:
	switch( (*p) ) {
		case 33: goto st63;
		case 92: goto st63;
		case 123: goto st63;
	}
	if ( (*p) < 58 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st63;
		} else if ( (*p) > 41 ) {
			if ( (*p) > 47 ) {
				if ( 48 <= (*p) && (*p) <= 57 )
					goto tr110;
			} else if ( (*p) >= 43 )
				goto st63;
		} else
			goto st63;
	} else if ( (*p) > 59 ) {
		if ( (*p) < 94 ) {
			if ( (*p) > 64 ) {
				if ( 65 <= (*p) && (*p) <= 90 )
					goto st73;
			} else if ( (*p) >= 63 )
				goto st63;
		} else if ( (*p) > 95 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st63;
			} else if ( (*p) >= 97 )
				goto st73;
		} else
			goto st63;
	} else
		goto st63;
	goto tr99;
st73:
	if ( ++p == pe )
		goto _test_eof73;
case 73:
	switch( (*p) ) {
		case 33: goto st63;
		case 46: goto st72;
		case 92: goto st63;
		case 123: goto st63;
	}
	if ( (*p) < 58 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st63;
		} else if ( (*p) > 41 ) {
			if ( (*p) > 47 ) {
				if ( 48 <= (*p) && (*p) <= 57 )
					goto tr110;
			} else if ( (*p) >= 43 )
				goto st63;
		} else
			goto st63;
	} else if ( (*p) > 59 ) {
		if ( (*p) < 94 ) {
			if ( (*p) > 64 ) {
				if ( 65 <= (*p) && (*p) <= 90 )
					goto st74;
			} else if ( (*p) >= 63 )
				goto st63;
		} else if ( (*p) > 95 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st63;
			} else if ( (*p) >= 97 )
				goto st74;
		} else
			goto st63;
	} else
		goto st63;
	goto tr99;
st74:
	if ( ++p == pe )
		goto _test_eof74;
case 74:
	switch( (*p) ) {
		case 33: goto st63;
		case 46: goto st72;
		case 92: goto st63;
		case 123: goto st63;
	}
	if ( (*p) < 58 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st63;
		} else if ( (*p) > 41 ) {
			if ( (*p) > 47 ) {
				if ( 48 <= (*p) && (*p) <= 57 )
					goto tr110;
			} else if ( (*p) >= 43 )
				goto st63;
		} else
			goto st63;
	} else if ( (*p) > 59 ) {
		if ( (*p) < 94 ) {
			if ( (*p) > 64 ) {
				if ( 65 <= (*p) && (*p) <= 90 )
					goto st75;
			} else if ( (*p) >= 63 )
				goto st63;
		} else if ( (*p) > 95 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st63;
			} else if ( (*p) >= 97 )
				goto st75;
		} else
			goto st63;
	} else
		goto st63;
	goto tr115;
st75:
	if ( ++p == pe )
		goto _test_eof75;
case 75:
	switch( (*p) ) {
		case 33: goto st63;
		case 46: goto st72;
		case 92: goto st63;
		case 123: goto st63;
	}
	if ( (*p) < 58 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st63;
		} else if ( (*p) > 41 ) {
			if ( (*p) > 47 ) {
				if ( 48 <= (*p) && (*p) <= 57 )
					goto tr110;
			} else if ( (*p) >= 43 )
				goto st63;
		} else
			goto st63;
	} else if ( (*p) > 59 ) {
		if ( (*p) < 94 ) {
			if ( (*p) > 64 ) {
				if ( 65 <= (*p) && (*p) <= 90 )
					goto st76;
			} else if ( (*p) >= 63 )
				goto st63;
		} else if ( (*p) > 95 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st63;
			} else if ( (*p) >= 97 )
				goto st76;
		} else
			goto st63;
	} else
		goto st63;
	goto tr115;
st76:
	if ( ++p == pe )
		goto _test_eof76;
case 76:
	switch( (*p) ) {
		case 33: goto st63;
		case 46: goto st72;
		case 92: goto st63;
		case 123: goto st63;
	}
	if ( (*p) < 58 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st63;
		} else if ( (*p) > 41 ) {
			if ( (*p) > 47 ) {
				if ( 48 <= (*p) && (*p) <= 57 )
					goto tr110;
			} else if ( (*p) >= 43 )
				goto st63;
		} else
			goto st63;
	} else if ( (*p) > 59 ) {
		if ( (*p) < 94 ) {
			if ( (*p) > 64 ) {
				if ( 65 <= (*p) && (*p) <= 90 )
					goto tr118;
			} else if ( (*p) >= 63 )
				goto st63;
		} else if ( (*p) > 95 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st63;
			} else if ( (*p) >= 97 )
				goto tr118;
		} else
			goto st63;
	} else
		goto st63;
	goto tr115;
tr84:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
	goto st77;
st77:
	if ( ++p == pe )
		goto _test_eof77;
case 77:
#line 1309 "wikitext_ragel.c"
	switch( (*p) ) {
		case 47: goto st20;
		case 69: goto st40;
		case 78: goto st42;
		case 80: goto st48;
		case 83: goto st51;
		case 84: goto st57;
		case 101: goto st40;
		case 110: goto st42;
		case 112: goto st48;
		case 115: goto st51;
		case 116: goto st57;
	}
	goto tr119;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
	switch( (*p) ) {
		case 69: goto st21;
		case 78: goto st23;
		case 80: goto st29;
		case 83: goto st32;
		case 84: goto st38;
		case 101: goto st21;
		case 110: goto st23;
		case 112: goto st29;
		case 115: goto st32;
		case 116: goto st38;
	}
	goto tr23;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
	switch( (*p) ) {
		case 77: goto st22;
		case 109: goto st22;
	}
	goto tr23;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
	if ( (*p) == 62 )
		goto tr30;
	goto tr23;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
	switch( (*p) ) {
		case 79: goto st24;
		case 111: goto st24;
	}
	goto tr23;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
	switch( (*p) ) {
		case 87: goto st25;
		case 119: goto st25;
	}
	goto tr23;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
	switch( (*p) ) {
		case 73: goto st26;
		case 105: goto st26;
	}
	goto tr23;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
	switch( (*p) ) {
		case 75: goto st27;
		case 107: goto st27;
	}
	goto tr23;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
	switch( (*p) ) {
		case 73: goto st28;
		case 105: goto st28;
	}
	goto tr23;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
	if ( (*p) == 62 )
		goto tr36;
	goto tr23;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
	switch( (*p) ) {
		case 82: goto st30;
		case 114: goto st30;
	}
	goto tr23;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
	switch( (*p) ) {
		case 69: goto st31;
		case 101: goto st31;
	}
	goto tr23;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
	if ( (*p) == 62 )
		goto tr39;
	goto tr23;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
	switch( (*p) ) {
		case 84: goto st33;
		case 116: goto st33;
	}
	goto tr23;
st33:
	if ( ++p == pe )
		goto _test_eof33;
case 33:
	switch( (*p) ) {
		case 82: goto st34;
		case 114: goto st34;
	}
	goto tr23;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
	switch( (*p) ) {
		case 79: goto st35;
		case 111: goto st35;
	}
	goto tr23;
st35:
	if ( ++p == pe )
		goto _test_eof35;
case 35:
	switch( (*p) ) {
		case 78: goto st36;
		case 110: goto st36;
	}
	goto tr23;
st36:
	if ( ++p == pe )
		goto _test_eof36;
case 36:
	switch( (*p) ) {
		case 71: goto st37;
		case 103: goto st37;
	}
	goto tr23;
st37:
	if ( ++p == pe )
		goto _test_eof37;
case 37:
	if ( (*p) == 62 )
		goto tr45;
	goto tr23;
st38:
	if ( ++p == pe )
		goto _test_eof38;
case 38:
	switch( (*p) ) {
		case 84: goto st39;
		case 116: goto st39;
	}
	goto tr23;
st39:
	if ( ++p == pe )
		goto _test_eof39;
case 39:
	if ( (*p) == 62 )
		goto tr47;
	goto tr23;
st40:
	if ( ++p == pe )
		goto _test_eof40;
case 40:
	switch( (*p) ) {
		case 77: goto st41;
		case 109: goto st41;
	}
	goto tr23;
st41:
	if ( ++p == pe )
		goto _test_eof41;
case 41:
	if ( (*p) == 62 )
		goto tr49;
	goto tr23;
st42:
	if ( ++p == pe )
		goto _test_eof42;
case 42:
	switch( (*p) ) {
		case 79: goto st43;
		case 111: goto st43;
	}
	goto tr23;
st43:
	if ( ++p == pe )
		goto _test_eof43;
case 43:
	switch( (*p) ) {
		case 87: goto st44;
		case 119: goto st44;
	}
	goto tr23;
st44:
	if ( ++p == pe )
		goto _test_eof44;
case 44:
	switch( (*p) ) {
		case 73: goto st45;
		case 105: goto st45;
	}
	goto tr23;
st45:
	if ( ++p == pe )
		goto _test_eof45;
case 45:
	switch( (*p) ) {
		case 75: goto st46;
		case 107: goto st46;
	}
	goto tr23;
st46:
	if ( ++p == pe )
		goto _test_eof46;
case 46:
	switch( (*p) ) {
		case 73: goto st47;
		case 105: goto st47;
	}
	goto tr23;
st47:
	if ( ++p == pe )
		goto _test_eof47;
case 47:
	if ( (*p) == 62 )
		goto tr55;
	goto tr23;
st48:
	if ( ++p == pe )
		goto _test_eof48;
case 48:
	switch( (*p) ) {
		case 82: goto st49;
		case 114: goto st49;
	}
	goto tr23;
st49:
	if ( ++p == pe )
		goto _test_eof49;
case 49:
	switch( (*p) ) {
		case 69: goto st50;
		case 101: goto st50;
	}
	goto tr23;
st50:
	if ( ++p == pe )
		goto _test_eof50;
case 50:
	if ( (*p) == 62 )
		goto tr58;
	goto tr23;
st51:
	if ( ++p == pe )
		goto _test_eof51;
case 51:
	switch( (*p) ) {
		case 84: goto st52;
		case 116: goto st52;
	}
	goto tr23;
st52:
	if ( ++p == pe )
		goto _test_eof52;
case 52:
	switch( (*p) ) {
		case 82: goto st53;
		case 114: goto st53;
	}
	goto tr23;
st53:
	if ( ++p == pe )
		goto _test_eof53;
case 53:
	switch( (*p) ) {
		case 79: goto st54;
		case 111: goto st54;
	}
	goto tr23;
st54:
	if ( ++p == pe )
		goto _test_eof54;
case 54:
	switch( (*p) ) {
		case 78: goto st55;
		case 110: goto st55;
	}
	goto tr23;
st55:
	if ( ++p == pe )
		goto _test_eof55;
case 55:
	switch( (*p) ) {
		case 71: goto st56;
		case 103: goto st56;
	}
	goto tr23;
st56:
	if ( ++p == pe )
		goto _test_eof56;
case 56:
	if ( (*p) == 62 )
		goto tr64;
	goto tr23;
st57:
	if ( ++p == pe )
		goto _test_eof57;
case 57:
	switch( (*p) ) {
		case 84: goto st58;
		case 116: goto st58;
	}
	goto tr23;
st58:
	if ( ++p == pe )
		goto _test_eof58;
case 58:
	if ( (*p) == 62 )
		goto tr66;
	goto tr23;
tr85:
#line 36 "wikitext_ragel.rl"
	{
        MARK();
    }
	goto st78;
st78:
	if ( ++p == pe )
		goto _test_eof78;
case 78:
#line 1673 "wikitext_ragel.c"
	switch( (*p) ) {
		case 32: goto st79;
		case 61: goto tr85;
	}
	goto tr126;
st79:
	if ( ++p == pe )
		goto _test_eof79;
case 79:
	if ( (*p) == 32 )
		goto st79;
	goto tr126;
tr86:
#line 36 "wikitext_ragel.rl"
	{
        MARK();
    }
	goto st80;
st80:
	if ( ++p == pe )
		goto _test_eof80;
case 80:
#line 1696 "wikitext_ragel.c"
	if ( (*p) == 32 )
		goto tr129;
	goto tr128;
st81:
	if ( ++p == pe )
		goto _test_eof81;
case 81:
	switch( (*p) ) {
		case 33: goto st63;
		case 47: goto st63;
		case 63: goto st63;
		case 64: goto st70;
		case 84: goto st82;
		case 92: goto st63;
		case 94: goto st63;
		case 95: goto st69;
		case 116: goto st82;
		case 123: goto st63;
	}
	if ( (*p) < 45 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st63;
		} else if ( (*p) > 41 ) {
			if ( 43 <= (*p) && (*p) <= 44 )
				goto st63;
		} else
			goto st63;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 65 ) {
			if ( 58 <= (*p) && (*p) <= 59 )
				goto st63;
		} else if ( (*p) > 90 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st63;
			} else if ( (*p) >= 97 )
				goto st69;
		} else
			goto st69;
	} else
		goto st69;
	goto tr99;
st82:
	if ( ++p == pe )
		goto _test_eof82;
case 82:
	switch( (*p) ) {
		case 33: goto st63;
		case 47: goto st63;
		case 63: goto st63;
		case 64: goto st70;
		case 80: goto st83;
		case 92: goto st63;
		case 94: goto st63;
		case 95: goto st69;
		case 112: goto st83;
		case 123: goto st63;
	}
	if ( (*p) < 45 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st63;
		} else if ( (*p) > 41 ) {
			if ( 43 <= (*p) && (*p) <= 44 )
				goto st63;
		} else
			goto st63;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 65 ) {
			if ( 58 <= (*p) && (*p) <= 59 )
				goto st63;
		} else if ( (*p) > 90 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st63;
			} else if ( (*p) >= 97 )
				goto st69;
		} else
			goto st69;
	} else
		goto st69;
	goto tr99;
st83:
	if ( ++p == pe )
		goto _test_eof83;
case 83:
	switch( (*p) ) {
		case 33: goto st63;
		case 47: goto st63;
		case 58: goto st84;
		case 59: goto st63;
		case 63: goto st63;
		case 64: goto st70;
		case 92: goto st63;
		case 94: goto st63;
		case 95: goto st69;
		case 123: goto st63;
	}
	if ( (*p) < 45 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st63;
		} else if ( (*p) > 41 ) {
			if ( 43 <= (*p) && (*p) <= 44 )
				goto st63;
		} else
			goto st63;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 97 ) {
			if ( 65 <= (*p) && (*p) <= 90 )
				goto st69;
		} else if ( (*p) > 122 ) {
			if ( 125 <= (*p) && (*p) <= 126 )
				goto st63;
		} else
			goto st69;
	} else
		goto st69;
	goto tr99;
st84:
	if ( ++p == pe )
		goto _test_eof84;
case 84:
	switch( (*p) ) {
		case 33: goto st63;
		case 47: goto st85;
		case 92: goto st63;
	}
	if ( (*p) < 63 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st63;
		} else if ( (*p) > 41 ) {
			if ( 43 <= (*p) && (*p) <= 59 )
				goto st63;
		} else
			goto st63;
	} else if ( (*p) > 90 ) {
		if ( (*p) < 97 ) {
			if ( 94 <= (*p) && (*p) <= 95 )
				goto st63;
		} else if ( (*p) > 123 ) {
			if ( 125 <= (*p) && (*p) <= 126 )
				goto st63;
		} else
			goto st63;
	} else
		goto st63;
	goto tr99;
st85:
	if ( ++p == pe )
		goto _test_eof85;
case 85:
	switch( (*p) ) {
		case 33: goto st63;
		case 47: goto st86;
		case 92: goto st63;
	}
	if ( (*p) < 63 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st63;
		} else if ( (*p) > 41 ) {
			if ( 43 <= (*p) && (*p) <= 59 )
				goto st63;
		} else
			goto st63;
	} else if ( (*p) > 90 ) {
		if ( (*p) < 97 ) {
			if ( 94 <= (*p) && (*p) <= 95 )
				goto st63;
		} else if ( (*p) > 123 ) {
			if ( 125 <= (*p) && (*p) <= 126 )
				goto st63;
		} else
			goto st63;
	} else
		goto st63;
	goto tr99;
st86:
	if ( ++p == pe )
		goto _test_eof86;
case 86:
	switch( (*p) ) {
		case 33: goto st63;
		case 35: goto tr69;
		case 40: goto tr135;
		case 41: goto st63;
		case 44: goto st63;
		case 46: goto st63;
		case 61: goto tr69;
		case 63: goto st63;
		case 92: goto st63;
		case 94: goto st63;
		case 95: goto tr135;
		case 123: goto st63;
		case 125: goto st63;
		case 126: goto tr135;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 37 ) {
			if ( 38 <= (*p) && (*p) <= 42 )
				goto tr69;
		} else if ( (*p) >= 36 )
			goto tr135;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 64 ) {
			if ( 58 <= (*p) && (*p) <= 59 )
				goto st63;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr135;
		} else
			goto tr135;
	} else
		goto tr135;
	goto tr99;
tr69:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
	goto st87;
st87:
	if ( ++p == pe )
		goto _test_eof87;
case 87:
#line 1923 "wikitext_ragel.c"
	switch( (*p) ) {
		case 33: goto st59;
		case 41: goto st59;
		case 44: goto st59;
		case 46: goto st59;
		case 61: goto tr69;
		case 63: goto st59;
		case 95: goto tr69;
		case 126: goto tr69;
	}
	if ( (*p) < 58 ) {
		if ( 35 <= (*p) && (*p) <= 57 )
			goto tr69;
	} else if ( (*p) > 59 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr69;
		} else if ( (*p) >= 64 )
			goto tr69;
	} else
		goto st59;
	goto tr136;
st59:
	if ( ++p == pe )
		goto _test_eof59;
case 59:
	switch( (*p) ) {
		case 33: goto st59;
		case 41: goto st59;
		case 44: goto st59;
		case 46: goto st59;
		case 61: goto tr69;
		case 63: goto st59;
		case 95: goto tr69;
		case 126: goto tr69;
	}
	if ( (*p) < 58 ) {
		if ( 35 <= (*p) && (*p) <= 57 )
			goto tr69;
	} else if ( (*p) > 59 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr69;
		} else if ( (*p) >= 64 )
			goto tr69;
	} else
		goto st59;
	goto tr67;
tr137:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
#line 369 "wikitext_ragel.rl"
	{act = 35;}
	goto st88;
tr135:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
#line 261 "wikitext_ragel.rl"
	{act = 18;}
	goto st88;
st88:
	if ( ++p == pe )
		goto _test_eof88;
case 88:
#line 1988 "wikitext_ragel.c"
	switch( (*p) ) {
		case 33: goto tr137;
		case 35: goto tr69;
		case 40: goto tr135;
		case 41: goto tr137;
		case 44: goto tr137;
		case 46: goto tr137;
		case 61: goto tr69;
		case 63: goto tr137;
		case 92: goto st63;
		case 94: goto st63;
		case 95: goto tr135;
		case 123: goto st63;
		case 125: goto st63;
		case 126: goto tr135;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 37 ) {
			if ( 38 <= (*p) && (*p) <= 42 )
				goto tr69;
		} else if ( (*p) >= 36 )
			goto tr135;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 64 ) {
			if ( 58 <= (*p) && (*p) <= 59 )
				goto tr137;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr135;
		} else
			goto tr135;
	} else
		goto tr135;
	goto tr111;
st89:
	if ( ++p == pe )
		goto _test_eof89;
case 89:
	switch( (*p) ) {
		case 33: goto st63;
		case 47: goto st63;
		case 63: goto st63;
		case 64: goto st70;
		case 84: goto st90;
		case 92: goto st63;
		case 94: goto st63;
		case 95: goto st69;
		case 116: goto st90;
		case 123: goto st63;
	}
	if ( (*p) < 45 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st63;
		} else if ( (*p) > 41 ) {
			if ( 43 <= (*p) && (*p) <= 44 )
				goto st63;
		} else
			goto st63;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 65 ) {
			if ( 58 <= (*p) && (*p) <= 59 )
				goto st63;
		} else if ( (*p) > 90 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st63;
			} else if ( (*p) >= 97 )
				goto st69;
		} else
			goto st69;
	} else
		goto st69;
	goto tr99;
st90:
	if ( ++p == pe )
		goto _test_eof90;
case 90:
	switch( (*p) ) {
		case 33: goto st63;
		case 47: goto st63;
		case 63: goto st63;
		case 64: goto st70;
		case 84: goto st91;
		case 92: goto st63;
		case 94: goto st63;
		case 95: goto st69;
		case 116: goto st91;
		case 123: goto st63;
	}
	if ( (*p) < 45 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st63;
		} else if ( (*p) > 41 ) {
			if ( 43 <= (*p) && (*p) <= 44 )
				goto st63;
		} else
			goto st63;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 65 ) {
			if ( 58 <= (*p) && (*p) <= 59 )
				goto st63;
		} else if ( (*p) > 90 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st63;
			} else if ( (*p) >= 97 )
				goto st69;
		} else
			goto st69;
	} else
		goto st69;
	goto tr99;
st91:
	if ( ++p == pe )
		goto _test_eof91;
case 91:
	switch( (*p) ) {
		case 33: goto st63;
		case 47: goto st63;
		case 63: goto st63;
		case 64: goto st70;
		case 80: goto st92;
		case 92: goto st63;
		case 94: goto st63;
		case 95: goto st69;
		case 112: goto st92;
		case 123: goto st63;
	}
	if ( (*p) < 45 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st63;
		} else if ( (*p) > 41 ) {
			if ( 43 <= (*p) && (*p) <= 44 )
				goto st63;
		} else
			goto st63;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 65 ) {
			if ( 58 <= (*p) && (*p) <= 59 )
				goto st63;
		} else if ( (*p) > 90 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st63;
			} else if ( (*p) >= 97 )
				goto st69;
		} else
			goto st69;
	} else
		goto st69;
	goto tr99;
st92:
	if ( ++p == pe )
		goto _test_eof92;
case 92:
	switch( (*p) ) {
		case 33: goto st63;
		case 47: goto st63;
		case 58: goto st84;
		case 59: goto st63;
		case 63: goto st63;
		case 64: goto st70;
		case 83: goto st83;
		case 92: goto st63;
		case 94: goto st63;
		case 95: goto st69;
		case 115: goto st83;
		case 123: goto st63;
	}
	if ( (*p) < 45 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st63;
		} else if ( (*p) > 41 ) {
			if ( 43 <= (*p) && (*p) <= 44 )
				goto st63;
		} else
			goto st63;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 97 ) {
			if ( 65 <= (*p) && (*p) <= 90 )
				goto st69;
		} else if ( (*p) > 122 ) {
			if ( 125 <= (*p) && (*p) <= 126 )
				goto st63;
		} else
			goto st69;
	} else
		goto st69;
	goto tr99;
st93:
	if ( ++p == pe )
		goto _test_eof93;
case 93:
	switch( (*p) ) {
		case 33: goto st63;
		case 47: goto st63;
		case 63: goto st63;
		case 64: goto st70;
		case 65: goto st94;
		case 92: goto st63;
		case 94: goto st63;
		case 95: goto st69;
		case 97: goto st94;
		case 123: goto st63;
	}
	if ( (*p) < 45 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st63;
		} else if ( (*p) > 41 ) {
			if ( 43 <= (*p) && (*p) <= 44 )
				goto st63;
		} else
			goto st63;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 66 ) {
			if ( 58 <= (*p) && (*p) <= 59 )
				goto st63;
		} else if ( (*p) > 90 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st63;
			} else if ( (*p) >= 98 )
				goto st69;
		} else
			goto st69;
	} else
		goto st69;
	goto tr99;
st94:
	if ( ++p == pe )
		goto _test_eof94;
case 94:
	switch( (*p) ) {
		case 33: goto st63;
		case 47: goto st63;
		case 63: goto st63;
		case 64: goto st70;
		case 73: goto st95;
		case 92: goto st63;
		case 94: goto st63;
		case 95: goto st69;
		case 105: goto st95;
		case 123: goto st63;
	}
	if ( (*p) < 45 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st63;
		} else if ( (*p) > 41 ) {
			if ( 43 <= (*p) && (*p) <= 44 )
				goto st63;
		} else
			goto st63;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 65 ) {
			if ( 58 <= (*p) && (*p) <= 59 )
				goto st63;
		} else if ( (*p) > 90 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st63;
			} else if ( (*p) >= 97 )
				goto st69;
		} else
			goto st69;
	} else
		goto st69;
	goto tr99;
st95:
	if ( ++p == pe )
		goto _test_eof95;
case 95:
	switch( (*p) ) {
		case 33: goto st63;
		case 47: goto st63;
		case 63: goto st63;
		case 64: goto st70;
		case 76: goto st96;
		case 92: goto st63;
		case 94: goto st63;
		case 95: goto st69;
		case 108: goto st96;
		case 123: goto st63;
	}
	if ( (*p) < 45 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st63;
		} else if ( (*p) > 41 ) {
			if ( 43 <= (*p) && (*p) <= 44 )
				goto st63;
		} else
			goto st63;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 65 ) {
			if ( 58 <= (*p) && (*p) <= 59 )
				goto st63;
		} else if ( (*p) > 90 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st63;
			} else if ( (*p) >= 97 )
				goto st69;
		} else
			goto st69;
	} else
		goto st69;
	goto tr99;
st96:
	if ( ++p == pe )
		goto _test_eof96;
case 96:
	switch( (*p) ) {
		case 33: goto st63;
		case 47: goto st63;
		case 63: goto st63;
		case 64: goto st70;
		case 84: goto st97;
		case 92: goto st63;
		case 94: goto st63;
		case 95: goto st69;
		case 116: goto st97;
		case 123: goto st63;
	}
	if ( (*p) < 45 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st63;
		} else if ( (*p) > 41 ) {
			if ( 43 <= (*p) && (*p) <= 44 )
				goto st63;
		} else
			goto st63;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 65 ) {
			if ( 58 <= (*p) && (*p) <= 59 )
				goto st63;
		} else if ( (*p) > 90 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st63;
			} else if ( (*p) >= 97 )
				goto st69;
		} else
			goto st69;
	} else
		goto st69;
	goto tr99;
st97:
	if ( ++p == pe )
		goto _test_eof97;
case 97:
	switch( (*p) ) {
		case 33: goto st63;
		case 47: goto st63;
		case 63: goto st63;
		case 64: goto st70;
		case 79: goto st98;
		case 92: goto st63;
		case 94: goto st63;
		case 95: goto st69;
		case 111: goto st98;
		case 123: goto st63;
	}
	if ( (*p) < 45 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st63;
		} else if ( (*p) > 41 ) {
			if ( 43 <= (*p) && (*p) <= 44 )
				goto st63;
		} else
			goto st63;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 65 ) {
			if ( 58 <= (*p) && (*p) <= 59 )
				goto st63;
		} else if ( (*p) > 90 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st63;
			} else if ( (*p) >= 97 )
				goto st69;
		} else
			goto st69;
	} else
		goto st69;
	goto tr99;
st98:
	if ( ++p == pe )
		goto _test_eof98;
case 98:
	switch( (*p) ) {
		case 33: goto st63;
		case 47: goto st63;
		case 58: goto st99;
		case 59: goto st63;
		case 63: goto st63;
		case 64: goto st70;
		case 92: goto st63;
		case 94: goto st63;
		case 95: goto st69;
		case 123: goto st63;
	}
	if ( (*p) < 45 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st63;
		} else if ( (*p) > 41 ) {
			if ( 43 <= (*p) && (*p) <= 44 )
				goto st63;
		} else
			goto st63;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 97 ) {
			if ( 65 <= (*p) && (*p) <= 90 )
				goto st69;
		} else if ( (*p) > 122 ) {
			if ( 125 <= (*p) && (*p) <= 126 )
				goto st63;
		} else
			goto st69;
	} else
		goto st69;
	goto tr99;
st99:
	if ( ++p == pe )
		goto _test_eof99;
case 99:
	switch( (*p) ) {
		case 33: goto st63;
		case 47: goto st63;
		case 92: goto st63;
		case 94: goto st63;
		case 95: goto st100;
		case 123: goto st63;
	}
	if ( (*p) < 58 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st63;
		} else if ( (*p) > 41 ) {
			if ( (*p) > 44 ) {
				if ( 45 <= (*p) && (*p) <= 57 )
					goto st100;
			} else if ( (*p) >= 43 )
				goto st63;
		} else
			goto st63;
	} else if ( (*p) > 59 ) {
		if ( (*p) < 65 ) {
			if ( 63 <= (*p) && (*p) <= 64 )
				goto st63;
		} else if ( (*p) > 90 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st63;
			} else if ( (*p) >= 97 )
				goto st100;
		} else
			goto st100;
	} else
		goto st63;
	goto tr99;
st100:
	if ( ++p == pe )
		goto _test_eof100;
case 100:
	switch( (*p) ) {
		case 33: goto st63;
		case 47: goto st63;
		case 63: goto st63;
		case 64: goto st101;
		case 92: goto st63;
		case 94: goto st63;
		case 95: goto st100;
		case 123: goto st63;
	}
	if ( (*p) < 45 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st63;
		} else if ( (*p) > 41 ) {
			if ( 43 <= (*p) && (*p) <= 44 )
				goto st63;
		} else
			goto st63;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 65 ) {
			if ( 58 <= (*p) && (*p) <= 59 )
				goto st63;
		} else if ( (*p) > 90 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st63;
			} else if ( (*p) >= 97 )
				goto st100;
		} else
			goto st100;
	} else
		goto st100;
	goto tr99;
st101:
	if ( ++p == pe )
		goto _test_eof101;
case 101:
	switch( (*p) ) {
		case 33: goto st63;
		case 92: goto st63;
		case 123: goto st63;
	}
	if ( (*p) < 58 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st63;
		} else if ( (*p) > 41 ) {
			if ( (*p) > 47 ) {
				if ( 48 <= (*p) && (*p) <= 57 )
					goto tr149;
			} else if ( (*p) >= 43 )
				goto st63;
		} else
			goto st63;
	} else if ( (*p) > 59 ) {
		if ( (*p) < 94 ) {
			if ( (*p) > 64 ) {
				if ( 65 <= (*p) && (*p) <= 90 )
					goto tr149;
			} else if ( (*p) >= 63 )
				goto st63;
		} else if ( (*p) > 95 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st63;
			} else if ( (*p) >= 97 )
				goto tr149;
		} else
			goto st63;
	} else
		goto st63;
	goto tr99;
tr149:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
#line 369 "wikitext_ragel.rl"
	{act = 35;}
	goto st102;
tr155:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
#line 261 "wikitext_ragel.rl"
	{act = 18;}
	goto st102;
st102:
	if ( ++p == pe )
		goto _test_eof102;
case 102:
#line 2551 "wikitext_ragel.c"
	switch( (*p) ) {
		case 33: goto st63;
		case 46: goto st103;
		case 92: goto st63;
		case 123: goto st63;
	}
	if ( (*p) < 58 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st63;
		} else if ( (*p) > 41 ) {
			if ( (*p) > 47 ) {
				if ( 48 <= (*p) && (*p) <= 57 )
					goto tr149;
			} else if ( (*p) >= 43 )
				goto st63;
		} else
			goto st63;
	} else if ( (*p) > 59 ) {
		if ( (*p) < 94 ) {
			if ( (*p) > 64 ) {
				if ( 65 <= (*p) && (*p) <= 90 )
					goto tr149;
			} else if ( (*p) >= 63 )
				goto st63;
		} else if ( (*p) > 95 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st63;
			} else if ( (*p) >= 97 )
				goto tr149;
		} else
			goto st63;
	} else
		goto st63;
	goto tr111;
st103:
	if ( ++p == pe )
		goto _test_eof103;
case 103:
	switch( (*p) ) {
		case 33: goto st63;
		case 92: goto st63;
		case 123: goto st63;
	}
	if ( (*p) < 58 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st63;
		} else if ( (*p) > 41 ) {
			if ( (*p) > 47 ) {
				if ( 48 <= (*p) && (*p) <= 57 )
					goto tr149;
			} else if ( (*p) >= 43 )
				goto st63;
		} else
			goto st63;
	} else if ( (*p) > 59 ) {
		if ( (*p) < 94 ) {
			if ( (*p) > 64 ) {
				if ( 65 <= (*p) && (*p) <= 90 )
					goto st104;
			} else if ( (*p) >= 63 )
				goto st63;
		} else if ( (*p) > 95 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st63;
			} else if ( (*p) >= 97 )
				goto st104;
		} else
			goto st63;
	} else
		goto st63;
	goto tr99;
st104:
	if ( ++p == pe )
		goto _test_eof104;
case 104:
	switch( (*p) ) {
		case 33: goto st63;
		case 46: goto st103;
		case 92: goto st63;
		case 123: goto st63;
	}
	if ( (*p) < 58 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st63;
		} else if ( (*p) > 41 ) {
			if ( (*p) > 47 ) {
				if ( 48 <= (*p) && (*p) <= 57 )
					goto tr149;
			} else if ( (*p) >= 43 )
				goto st63;
		} else
			goto st63;
	} else if ( (*p) > 59 ) {
		if ( (*p) < 94 ) {
			if ( (*p) > 64 ) {
				if ( 65 <= (*p) && (*p) <= 90 )
					goto st105;
			} else if ( (*p) >= 63 )
				goto st63;
		} else if ( (*p) > 95 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st63;
			} else if ( (*p) >= 97 )
				goto st105;
		} else
			goto st63;
	} else
		goto st63;
	goto tr99;
st105:
	if ( ++p == pe )
		goto _test_eof105;
case 105:
	switch( (*p) ) {
		case 33: goto st63;
		case 46: goto st103;
		case 92: goto st63;
		case 123: goto st63;
	}
	if ( (*p) < 58 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st63;
		} else if ( (*p) > 41 ) {
			if ( (*p) > 47 ) {
				if ( 48 <= (*p) && (*p) <= 57 )
					goto tr149;
			} else if ( (*p) >= 43 )
				goto st63;
		} else
			goto st63;
	} else if ( (*p) > 59 ) {
		if ( (*p) < 94 ) {
			if ( (*p) > 64 ) {
				if ( 65 <= (*p) && (*p) <= 90 )
					goto st106;
			} else if ( (*p) >= 63 )
				goto st63;
		} else if ( (*p) > 95 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st63;
			} else if ( (*p) >= 97 )
				goto st106;
		} else
			goto st63;
	} else
		goto st63;
	goto tr136;
st106:
	if ( ++p == pe )
		goto _test_eof106;
case 106:
	switch( (*p) ) {
		case 33: goto st63;
		case 46: goto st103;
		case 92: goto st63;
		case 123: goto st63;
	}
	if ( (*p) < 58 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st63;
		} else if ( (*p) > 41 ) {
			if ( (*p) > 47 ) {
				if ( 48 <= (*p) && (*p) <= 57 )
					goto tr149;
			} else if ( (*p) >= 43 )
				goto st63;
		} else
			goto st63;
	} else if ( (*p) > 59 ) {
		if ( (*p) < 94 ) {
			if ( (*p) > 64 ) {
				if ( 65 <= (*p) && (*p) <= 90 )
					goto st107;
			} else if ( (*p) >= 63 )
				goto st63;
		} else if ( (*p) > 95 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st63;
			} else if ( (*p) >= 97 )
				goto st107;
		} else
			goto st63;
	} else
		goto st63;
	goto tr136;
st107:
	if ( ++p == pe )
		goto _test_eof107;
case 107:
	switch( (*p) ) {
		case 33: goto st63;
		case 46: goto st103;
		case 92: goto st63;
		case 123: goto st63;
	}
	if ( (*p) < 58 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st63;
		} else if ( (*p) > 41 ) {
			if ( (*p) > 47 ) {
				if ( 48 <= (*p) && (*p) <= 57 )
					goto tr149;
			} else if ( (*p) >= 43 )
				goto st63;
		} else
			goto st63;
	} else if ( (*p) > 59 ) {
		if ( (*p) < 94 ) {
			if ( (*p) > 64 ) {
				if ( 65 <= (*p) && (*p) <= 90 )
					goto tr155;
			} else if ( (*p) >= 63 )
				goto st63;
		} else if ( (*p) > 95 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st63;
			} else if ( (*p) >= 97 )
				goto tr155;
		} else
			goto st63;
	} else
		goto st63;
	goto tr136;
st108:
	if ( ++p == pe )
		goto _test_eof108;
case 108:
	switch( (*p) ) {
		case 33: goto st63;
		case 47: goto st63;
		case 63: goto st63;
		case 64: goto st70;
		case 86: goto st109;
		case 92: goto st63;
		case 94: goto st63;
		case 95: goto st69;
		case 118: goto st109;
		case 123: goto st63;
	}
	if ( (*p) < 45 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st63;
		} else if ( (*p) > 41 ) {
			if ( 43 <= (*p) && (*p) <= 44 )
				goto st63;
		} else
			goto st63;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 65 ) {
			if ( 58 <= (*p) && (*p) <= 59 )
				goto st63;
		} else if ( (*p) > 90 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st63;
			} else if ( (*p) >= 97 )
				goto st69;
		} else
			goto st69;
	} else
		goto st69;
	goto tr99;
st109:
	if ( ++p == pe )
		goto _test_eof109;
case 109:
	switch( (*p) ) {
		case 33: goto st63;
		case 47: goto st63;
		case 63: goto st63;
		case 64: goto st70;
		case 78: goto st83;
		case 92: goto st63;
		case 94: goto st63;
		case 95: goto st69;
		case 110: goto st83;
		case 123: goto st63;
	}
	if ( (*p) < 45 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st63;
		} else if ( (*p) > 41 ) {
			if ( 43 <= (*p) && (*p) <= 44 )
				goto st63;
		} else
			goto st63;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 65 ) {
			if ( 58 <= (*p) && (*p) <= 59 )
				goto st63;
		} else if ( (*p) > 90 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st63;
			} else if ( (*p) >= 97 )
				goto st69;
		} else
			goto st69;
	} else
		goto st69;
	goto tr99;
st110:
	if ( ++p == pe )
		goto _test_eof110;
case 110:
	if ( (*p) == 91 )
		goto tr158;
	goto tr157;
st111:
	if ( ++p == pe )
		goto _test_eof111;
case 111:
	if ( (*p) == 93 )
		goto tr160;
	goto tr159;
	}
	_test_eof60: cs = 60; goto _test_eof; 
	_test_eof1: cs = 1; goto _test_eof; 
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof61: cs = 61; goto _test_eof; 
	_test_eof62: cs = 62; goto _test_eof; 
	_test_eof63: cs = 63; goto _test_eof; 
	_test_eof64: cs = 64; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 
	_test_eof11: cs = 11; goto _test_eof; 
	_test_eof12: cs = 12; goto _test_eof; 
	_test_eof13: cs = 13; goto _test_eof; 
	_test_eof14: cs = 14; goto _test_eof; 
	_test_eof15: cs = 15; goto _test_eof; 
	_test_eof16: cs = 16; goto _test_eof; 
	_test_eof17: cs = 17; goto _test_eof; 
	_test_eof18: cs = 18; goto _test_eof; 
	_test_eof19: cs = 19; goto _test_eof; 
	_test_eof65: cs = 65; goto _test_eof; 
	_test_eof66: cs = 66; goto _test_eof; 
	_test_eof67: cs = 67; goto _test_eof; 
	_test_eof68: cs = 68; goto _test_eof; 
	_test_eof69: cs = 69; goto _test_eof; 
	_test_eof70: cs = 70; goto _test_eof; 
	_test_eof71: cs = 71; goto _test_eof; 
	_test_eof72: cs = 72; goto _test_eof; 
	_test_eof73: cs = 73; goto _test_eof; 
	_test_eof74: cs = 74; goto _test_eof; 
	_test_eof75: cs = 75; goto _test_eof; 
	_test_eof76: cs = 76; goto _test_eof; 
	_test_eof77: cs = 77; goto _test_eof; 
	_test_eof20: cs = 20; goto _test_eof; 
	_test_eof21: cs = 21; goto _test_eof; 
	_test_eof22: cs = 22; goto _test_eof; 
	_test_eof23: cs = 23; goto _test_eof; 
	_test_eof24: cs = 24; goto _test_eof; 
	_test_eof25: cs = 25; goto _test_eof; 
	_test_eof26: cs = 26; goto _test_eof; 
	_test_eof27: cs = 27; goto _test_eof; 
	_test_eof28: cs = 28; goto _test_eof; 
	_test_eof29: cs = 29; goto _test_eof; 
	_test_eof30: cs = 30; goto _test_eof; 
	_test_eof31: cs = 31; goto _test_eof; 
	_test_eof32: cs = 32; goto _test_eof; 
	_test_eof33: cs = 33; goto _test_eof; 
	_test_eof34: cs = 34; goto _test_eof; 
	_test_eof35: cs = 35; goto _test_eof; 
	_test_eof36: cs = 36; goto _test_eof; 
	_test_eof37: cs = 37; goto _test_eof; 
	_test_eof38: cs = 38; goto _test_eof; 
	_test_eof39: cs = 39; goto _test_eof; 
	_test_eof40: cs = 40; goto _test_eof; 
	_test_eof41: cs = 41; goto _test_eof; 
	_test_eof42: cs = 42; goto _test_eof; 
	_test_eof43: cs = 43; goto _test_eof; 
	_test_eof44: cs = 44; goto _test_eof; 
	_test_eof45: cs = 45; goto _test_eof; 
	_test_eof46: cs = 46; goto _test_eof; 
	_test_eof47: cs = 47; goto _test_eof; 
	_test_eof48: cs = 48; goto _test_eof; 
	_test_eof49: cs = 49; goto _test_eof; 
	_test_eof50: cs = 50; goto _test_eof; 
	_test_eof51: cs = 51; goto _test_eof; 
	_test_eof52: cs = 52; goto _test_eof; 
	_test_eof53: cs = 53; goto _test_eof; 
	_test_eof54: cs = 54; goto _test_eof; 
	_test_eof55: cs = 55; goto _test_eof; 
	_test_eof56: cs = 56; goto _test_eof; 
	_test_eof57: cs = 57; goto _test_eof; 
	_test_eof58: cs = 58; goto _test_eof; 
	_test_eof78: cs = 78; goto _test_eof; 
	_test_eof79: cs = 79; goto _test_eof; 
	_test_eof80: cs = 80; goto _test_eof; 
	_test_eof81: cs = 81; goto _test_eof; 
	_test_eof82: cs = 82; goto _test_eof; 
	_test_eof83: cs = 83; goto _test_eof; 
	_test_eof84: cs = 84; goto _test_eof; 
	_test_eof85: cs = 85; goto _test_eof; 
	_test_eof86: cs = 86; goto _test_eof; 
	_test_eof87: cs = 87; goto _test_eof; 
	_test_eof59: cs = 59; goto _test_eof; 
	_test_eof88: cs = 88; goto _test_eof; 
	_test_eof89: cs = 89; goto _test_eof; 
	_test_eof90: cs = 90; goto _test_eof; 
	_test_eof91: cs = 91; goto _test_eof; 
	_test_eof92: cs = 92; goto _test_eof; 
	_test_eof93: cs = 93; goto _test_eof; 
	_test_eof94: cs = 94; goto _test_eof; 
	_test_eof95: cs = 95; goto _test_eof; 
	_test_eof96: cs = 96; goto _test_eof; 
	_test_eof97: cs = 97; goto _test_eof; 
	_test_eof98: cs = 98; goto _test_eof; 
	_test_eof99: cs = 99; goto _test_eof; 
	_test_eof100: cs = 100; goto _test_eof; 
	_test_eof101: cs = 101; goto _test_eof; 
	_test_eof102: cs = 102; goto _test_eof; 
	_test_eof103: cs = 103; goto _test_eof; 
	_test_eof104: cs = 104; goto _test_eof; 
	_test_eof105: cs = 105; goto _test_eof; 
	_test_eof106: cs = 106; goto _test_eof; 
	_test_eof107: cs = 107; goto _test_eof; 
	_test_eof108: cs = 108; goto _test_eof; 
	_test_eof109: cs = 109; goto _test_eof; 
	_test_eof110: cs = 110; goto _test_eof; 
	_test_eof111: cs = 111; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 61: goto tr95;
	case 62: goto tr97;
	case 63: goto tr99;
	case 64: goto tr100;
	case 7: goto tr7;
	case 8: goto tr7;
	case 9: goto tr7;
	case 10: goto tr7;
	case 11: goto tr7;
	case 12: goto tr7;
	case 13: goto tr7;
	case 14: goto tr7;
	case 15: goto tr7;
	case 16: goto tr7;
	case 17: goto tr7;
	case 18: goto tr7;
	case 19: goto tr7;
	case 65: goto tr104;
	case 66: goto tr104;
	case 67: goto tr104;
	case 68: goto tr104;
	case 69: goto tr99;
	case 70: goto tr99;
	case 71: goto tr111;
	case 72: goto tr99;
	case 73: goto tr99;
	case 74: goto tr115;
	case 75: goto tr115;
	case 76: goto tr115;
	case 77: goto tr119;
	case 20: goto tr23;
	case 21: goto tr23;
	case 22: goto tr23;
	case 23: goto tr23;
	case 24: goto tr23;
	case 25: goto tr23;
	case 26: goto tr23;
	case 27: goto tr23;
	case 28: goto tr23;
	case 29: goto tr23;
	case 30: goto tr23;
	case 31: goto tr23;
	case 32: goto tr23;
	case 33: goto tr23;
	case 34: goto tr23;
	case 35: goto tr23;
	case 36: goto tr23;
	case 37: goto tr23;
	case 38: goto tr23;
	case 39: goto tr23;
	case 40: goto tr23;
	case 41: goto tr23;
	case 42: goto tr23;
	case 43: goto tr23;
	case 44: goto tr23;
	case 45: goto tr23;
	case 46: goto tr23;
	case 47: goto tr23;
	case 48: goto tr23;
	case 49: goto tr23;
	case 50: goto tr23;
	case 51: goto tr23;
	case 52: goto tr23;
	case 53: goto tr23;
	case 54: goto tr23;
	case 55: goto tr23;
	case 56: goto tr23;
	case 57: goto tr23;
	case 58: goto tr23;
	case 78: goto tr126;
	case 79: goto tr126;
	case 80: goto tr128;
	case 81: goto tr99;
	case 82: goto tr99;
	case 83: goto tr99;
	case 84: goto tr99;
	case 85: goto tr99;
	case 86: goto tr99;
	case 87: goto tr136;
	case 59: goto tr67;
	case 88: goto tr111;
	case 89: goto tr99;
	case 90: goto tr99;
	case 91: goto tr99;
	case 92: goto tr99;
	case 93: goto tr99;
	case 94: goto tr99;
	case 95: goto tr99;
	case 96: goto tr99;
	case 97: goto tr99;
	case 98: goto tr99;
	case 99: goto tr99;
	case 100: goto tr99;
	case 101: goto tr99;
	case 102: goto tr111;
	case 103: goto tr99;
	case 104: goto tr99;
	case 105: goto tr136;
	case 106: goto tr136;
	case 107: goto tr136;
	case 108: goto tr99;
	case 109: goto tr99;
	case 110: goto tr157;
	case 111: goto tr159;
	}
	}

	_out: {}
	}
#line 441 "wikitext_ragel.rl"
    if (cs == wikitext_error)
        rb_raise(eWikitextParserError, "failed before finding a token");
    else if (out->type == NO_TOKEN)
        rb_raise(eWikitextParserError, "failed to produce a token");
}
