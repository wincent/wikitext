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
static const int wikitext_start = 54;
static const int wikitext_first_final = 54;
static const int wikitext_error = 0;

static const int wikitext_en_main = 54;

#line 373 "wikitext_ragel.rl"


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
#line 415 "wikitext_ragel.rl"
    
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
#line 364 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(DEFAULT);
            out->column_stop = out->column_start + 1;
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr3:
#line 52 "wikitext_ragel.rl"
	{
        out->code_point = ((uint32_t)(*(p - 2)) & 0x0f) << 12 |
            ((uint32_t)(*(p - 1)) & 0x3f) << 6 |
            (*p & 0x3f);
    }
#line 364 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(DEFAULT);
            out->column_stop = out->column_start + 1;
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr6:
#line 59 "wikitext_ragel.rl"
	{
        out->code_point = ((uint32_t)(*(p - 3)) & 0x07) << 18 |
            ((uint32_t)(*(p - 2)) & 0x3f) << 12 |
            ((uint32_t)(*(p - 1)) & 0x3f) << 6 |
            (*p & 0x3f);
    }
#line 364 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(DEFAULT);
            out->column_stop = out->column_start + 1;
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr7:
#line 314 "wikitext_ragel.rl"
	{{p = ((te))-1;}{
            EMIT(AMP);
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr10:
#line 302 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(DECIMAL_ENTITY);
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr12:
#line 296 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(HEX_ENTITY);
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr14:
#line 290 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(NAMED_ENTITY);
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr18:
#line 284 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(AMP_ENTITY);
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr22:
#line 278 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(QUOT_ENTITY);
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr23:
#line 320 "wikitext_ragel.rl"
	{{p = ((te))-1;}{
            EMIT(LESS);
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr29:
#line 121 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(EM_END);
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr35:
#line 79 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(NO_WIKI_END);
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr41:
#line 109 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(STRONG_END);
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr43:
#line 139 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(TT_END);
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr45:
#line 115 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(EM_START);
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr51:
#line 73 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(NO_WIKI_START);
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr57:
#line 103 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(STRONG_START);
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr59:
#line 133 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(TT_START);
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr60:
#line 242 "wikitext_ragel.rl"
	{{p = ((te))-1;}{
            EMIT(URI);
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr66:
#line 41 "wikitext_ragel.rl"
	{
        out->code_point = *p & 0x7f;
    }
#line 364 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(DEFAULT);
            out->column_stop = out->column_start + 1;
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr67:
#line 332 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(CRLF);
            out->column_stop = 1;
            out->line_stop++;
            {p++; cs = 54; goto _out;}
        }}
#line 41 "wikitext_ragel.rl"
	{
        out->code_point = *p & 0x7f;
    }
	goto st54;
tr71:
#line 308 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(QUOT);
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr72:
#line 172 "wikitext_ragel.rl"
	{te = p+1;{
            if (out->column_start == 1 || last_token_type == OL || last_token_type == UL || last_token_type == BLOCKQUOTE)
                EMIT(OL);
            else
                EMIT(PRINTABLE);
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr75:
#line 181 "wikitext_ragel.rl"
	{te = p+1;{
            if (out->column_start == 1 || last_token_type == OL || last_token_type == UL || last_token_type == BLOCKQUOTE)
                EMIT(UL);
            else
                EMIT(PRINTABLE);
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr84:
#line 127 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(TT);
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr85:
#line 260 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(SEPARATOR);
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr86:
#line 332 "wikitext_ragel.rl"
	{te = p;p--;{
            EMIT(CRLF);
            out->column_stop = 1;
            out->line_stop++;
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr87:
#line 332 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(CRLF);
            out->column_stop = 1;
            out->line_stop++;
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr88:
#line 160 "wikitext_ragel.rl"
	{te = p;p--;{
            if (out->column_start == 1 || last_token_type == BLOCKQUOTE)
            {
                REWIND();
                EMIT(PRE);
            }
            else
                EMIT(SPACE);
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr90:
#line 344 "wikitext_ragel.rl"
	{te = p;p--;{
            EMIT(PRINTABLE);
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr91:
#line 314 "wikitext_ragel.rl"
	{te = p;p--;{
            EMIT(AMP);
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr95:
#line 85 "wikitext_ragel.rl"
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
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr99:
#line 85 "wikitext_ragel.rl"
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
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr100:
#line 320 "wikitext_ragel.rl"
	{te = p;p--;{
            EMIT(LESS);
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr106:
#line 190 "wikitext_ragel.rl"
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
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr108:
#line 146 "wikitext_ragel.rl"
	{te = p;p--;{
            if (out->column_start == 1 || last_token_type == BLOCKQUOTE)
                EMIT(BLOCKQUOTE);
            else
            {
                REWIND();
                EMIT(GREATER);
            }
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr109:
#line 146 "wikitext_ragel.rl"
	{te = p+1;{
            if (out->column_start == 1 || last_token_type == BLOCKQUOTE)
                EMIT(BLOCKQUOTE);
            else
            {
                REWIND();
                EMIT(GREATER);
            }
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr116:
#line 242 "wikitext_ragel.rl"
	{te = p;p--;{
            EMIT(URI);
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr117:
#line 1 "wikitext_ragel.rl"
	{	switch( act ) {
	case 16:
	{{p = ((te))-1;}
            EMIT(URI);
            {p++; cs = 54; goto _out;}
        }
	break;
	case 32:
	{{p = ((te))-1;}
            EMIT(PRINTABLE);
            {p++; cs = 54; goto _out;}
        }
	break;
	default: break;
	}
	}
	goto st54;
tr120:
#line 266 "wikitext_ragel.rl"
	{te = p;p--;{
            EMIT(EXT_LINK_START);
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr121:
#line 248 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(LINK_START);
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr122:
#line 272 "wikitext_ragel.rl"
	{te = p;p--;{
            EMIT(EXT_LINK_END);
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr123:
#line 254 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(LINK_END);
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
st54:
#line 1 "wikitext_ragel.rl"
	{ts = 0;}
	if ( ++p == pe )
		goto _test_eof54;
case 54:
#line 1 "wikitext_ragel.rl"
	{ts = p;}
#line 551 "wikitext_ragel.c"
	switch( (*p) ) {
		case 10: goto tr67;
		case 13: goto tr68;
		case 32: goto tr69;
		case 34: goto tr71;
		case 35: goto tr72;
		case 38: goto tr73;
		case 39: goto st59;
		case 42: goto tr75;
		case 60: goto tr76;
		case 61: goto tr77;
		case 62: goto tr78;
		case 70: goto st67;
		case 72: goto st75;
		case 83: goto st76;
		case 91: goto st78;
		case 93: goto st79;
		case 96: goto tr84;
		case 102: goto st67;
		case 104: goto st75;
		case 115: goto st76;
		case 124: goto tr85;
		case 126: goto st57;
		case 127: goto tr66;
	}
	if ( (*p) < -16 ) {
		if ( (*p) > -33 ) {
			if ( -32 <= (*p) && (*p) <= -17 )
				goto st2;
		} else if ( (*p) >= -62 )
			goto st1;
	} else if ( (*p) > -12 ) {
		if ( (*p) > 31 ) {
			if ( 33 <= (*p) && (*p) <= 123 )
				goto st57;
		} else if ( (*p) >= 1 )
			goto tr66;
	} else
		goto st4;
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
tr68:
#line 41 "wikitext_ragel.rl"
	{
        out->code_point = *p & 0x7f;
    }
	goto st55;
st55:
	if ( ++p == pe )
		goto _test_eof55;
case 55:
#line 647 "wikitext_ragel.c"
	if ( (*p) == 10 )
		goto tr87;
	goto tr86;
tr69:
#line 36 "wikitext_ragel.rl"
	{
        MARK();
    }
	goto st56;
st56:
	if ( ++p == pe )
		goto _test_eof56;
case 56:
#line 661 "wikitext_ragel.c"
	if ( (*p) == 32 )
		goto st56;
	goto tr88;
st57:
	if ( ++p == pe )
		goto _test_eof57;
case 57:
	switch( (*p) ) {
		case 33: goto st57;
		case 92: goto st57;
		case 126: goto st57;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 37 ) {
			if ( 40 <= (*p) && (*p) <= 41 )
				goto st57;
		} else if ( (*p) >= 36 )
			goto st57;
	} else if ( (*p) > 59 ) {
		if ( (*p) < 94 ) {
			if ( 63 <= (*p) && (*p) <= 90 )
				goto st57;
		} else if ( (*p) > 95 ) {
			if ( 97 <= (*p) && (*p) <= 123 )
				goto st57;
		} else
			goto st57;
	} else
		goto st57;
	goto tr90;
tr73:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
	goto st58;
st58:
	if ( ++p == pe )
		goto _test_eof58;
case 58:
#line 700 "wikitext_ragel.c"
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
	goto tr91;
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
st59:
	if ( ++p == pe )
		goto _test_eof59;
case 59:
	if ( (*p) == 39 )
		goto st60;
	goto tr95;
st60:
	if ( ++p == pe )
		goto _test_eof60;
case 60:
	if ( (*p) == 39 )
		goto st61;
	goto tr95;
st61:
	if ( ++p == pe )
		goto _test_eof61;
case 61:
	if ( (*p) == 39 )
		goto st62;
	goto tr95;
st62:
	if ( ++p == pe )
		goto _test_eof62;
case 62:
	if ( (*p) == 39 )
		goto tr99;
	goto tr95;
tr76:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
	goto st63;
st63:
	if ( ++p == pe )
		goto _test_eof63;
case 63:
#line 935 "wikitext_ragel.c"
	switch( (*p) ) {
		case 47: goto st20;
		case 69: goto st37;
		case 78: goto st39;
		case 83: goto st45;
		case 84: goto st51;
		case 101: goto st37;
		case 110: goto st39;
		case 115: goto st45;
		case 116: goto st51;
	}
	goto tr100;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
	switch( (*p) ) {
		case 69: goto st21;
		case 78: goto st23;
		case 83: goto st29;
		case 84: goto st35;
		case 101: goto st21;
		case 110: goto st23;
		case 115: goto st29;
		case 116: goto st35;
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
		goto tr29;
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
		goto tr35;
	goto tr23;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
	switch( (*p) ) {
		case 84: goto st30;
		case 116: goto st30;
	}
	goto tr23;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
	switch( (*p) ) {
		case 82: goto st31;
		case 114: goto st31;
	}
	goto tr23;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
	switch( (*p) ) {
		case 79: goto st32;
		case 111: goto st32;
	}
	goto tr23;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
	switch( (*p) ) {
		case 78: goto st33;
		case 110: goto st33;
	}
	goto tr23;
st33:
	if ( ++p == pe )
		goto _test_eof33;
case 33:
	switch( (*p) ) {
		case 71: goto st34;
		case 103: goto st34;
	}
	goto tr23;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
	if ( (*p) == 62 )
		goto tr41;
	goto tr23;
st35:
	if ( ++p == pe )
		goto _test_eof35;
case 35:
	switch( (*p) ) {
		case 84: goto st36;
		case 116: goto st36;
	}
	goto tr23;
st36:
	if ( ++p == pe )
		goto _test_eof36;
case 36:
	if ( (*p) == 62 )
		goto tr43;
	goto tr23;
st37:
	if ( ++p == pe )
		goto _test_eof37;
case 37:
	switch( (*p) ) {
		case 77: goto st38;
		case 109: goto st38;
	}
	goto tr23;
st38:
	if ( ++p == pe )
		goto _test_eof38;
case 38:
	if ( (*p) == 62 )
		goto tr45;
	goto tr23;
st39:
	if ( ++p == pe )
		goto _test_eof39;
case 39:
	switch( (*p) ) {
		case 79: goto st40;
		case 111: goto st40;
	}
	goto tr23;
st40:
	if ( ++p == pe )
		goto _test_eof40;
case 40:
	switch( (*p) ) {
		case 87: goto st41;
		case 119: goto st41;
	}
	goto tr23;
st41:
	if ( ++p == pe )
		goto _test_eof41;
case 41:
	switch( (*p) ) {
		case 73: goto st42;
		case 105: goto st42;
	}
	goto tr23;
st42:
	if ( ++p == pe )
		goto _test_eof42;
case 42:
	switch( (*p) ) {
		case 75: goto st43;
		case 107: goto st43;
	}
	goto tr23;
st43:
	if ( ++p == pe )
		goto _test_eof43;
case 43:
	switch( (*p) ) {
		case 73: goto st44;
		case 105: goto st44;
	}
	goto tr23;
st44:
	if ( ++p == pe )
		goto _test_eof44;
case 44:
	if ( (*p) == 62 )
		goto tr51;
	goto tr23;
st45:
	if ( ++p == pe )
		goto _test_eof45;
case 45:
	switch( (*p) ) {
		case 84: goto st46;
		case 116: goto st46;
	}
	goto tr23;
st46:
	if ( ++p == pe )
		goto _test_eof46;
case 46:
	switch( (*p) ) {
		case 82: goto st47;
		case 114: goto st47;
	}
	goto tr23;
st47:
	if ( ++p == pe )
		goto _test_eof47;
case 47:
	switch( (*p) ) {
		case 79: goto st48;
		case 111: goto st48;
	}
	goto tr23;
st48:
	if ( ++p == pe )
		goto _test_eof48;
case 48:
	switch( (*p) ) {
		case 78: goto st49;
		case 110: goto st49;
	}
	goto tr23;
st49:
	if ( ++p == pe )
		goto _test_eof49;
case 49:
	switch( (*p) ) {
		case 71: goto st50;
		case 103: goto st50;
	}
	goto tr23;
st50:
	if ( ++p == pe )
		goto _test_eof50;
case 50:
	if ( (*p) == 62 )
		goto tr57;
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
	if ( (*p) == 62 )
		goto tr59;
	goto tr23;
tr77:
#line 36 "wikitext_ragel.rl"
	{
        MARK();
    }
	goto st64;
st64:
	if ( ++p == pe )
		goto _test_eof64;
case 64:
#line 1245 "wikitext_ragel.c"
	switch( (*p) ) {
		case 32: goto st65;
		case 61: goto tr77;
	}
	goto tr106;
st65:
	if ( ++p == pe )
		goto _test_eof65;
case 65:
	if ( (*p) == 32 )
		goto st65;
	goto tr106;
tr78:
#line 36 "wikitext_ragel.rl"
	{
        MARK();
    }
	goto st66;
st66:
	if ( ++p == pe )
		goto _test_eof66;
case 66:
#line 1268 "wikitext_ragel.c"
	if ( (*p) == 32 )
		goto tr109;
	goto tr108;
st67:
	if ( ++p == pe )
		goto _test_eof67;
case 67:
	switch( (*p) ) {
		case 33: goto st57;
		case 84: goto st68;
		case 92: goto st57;
		case 116: goto st68;
		case 126: goto st57;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 37 ) {
			if ( 40 <= (*p) && (*p) <= 41 )
				goto st57;
		} else if ( (*p) >= 36 )
			goto st57;
	} else if ( (*p) > 59 ) {
		if ( (*p) < 94 ) {
			if ( 63 <= (*p) && (*p) <= 90 )
				goto st57;
		} else if ( (*p) > 95 ) {
			if ( 97 <= (*p) && (*p) <= 123 )
				goto st57;
		} else
			goto st57;
	} else
		goto st57;
	goto tr90;
st68:
	if ( ++p == pe )
		goto _test_eof68;
case 68:
	switch( (*p) ) {
		case 33: goto st57;
		case 80: goto st69;
		case 92: goto st57;
		case 112: goto st69;
		case 126: goto st57;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 37 ) {
			if ( 40 <= (*p) && (*p) <= 41 )
				goto st57;
		} else if ( (*p) >= 36 )
			goto st57;
	} else if ( (*p) > 59 ) {
		if ( (*p) < 94 ) {
			if ( 63 <= (*p) && (*p) <= 90 )
				goto st57;
		} else if ( (*p) > 95 ) {
			if ( 97 <= (*p) && (*p) <= 123 )
				goto st57;
		} else
			goto st57;
	} else
		goto st57;
	goto tr90;
st69:
	if ( ++p == pe )
		goto _test_eof69;
case 69:
	switch( (*p) ) {
		case 33: goto st57;
		case 58: goto st70;
		case 92: goto st57;
		case 126: goto st57;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 37 ) {
			if ( 40 <= (*p) && (*p) <= 41 )
				goto st57;
		} else if ( (*p) >= 36 )
			goto st57;
	} else if ( (*p) > 59 ) {
		if ( (*p) < 94 ) {
			if ( 63 <= (*p) && (*p) <= 90 )
				goto st57;
		} else if ( (*p) > 95 ) {
			if ( 97 <= (*p) && (*p) <= 123 )
				goto st57;
		} else
			goto st57;
	} else
		goto st57;
	goto tr90;
st70:
	if ( ++p == pe )
		goto _test_eof70;
case 70:
	switch( (*p) ) {
		case 33: goto st57;
		case 47: goto st71;
		case 92: goto st57;
		case 126: goto st57;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 37 ) {
			if ( 40 <= (*p) && (*p) <= 41 )
				goto st57;
		} else if ( (*p) >= 36 )
			goto st57;
	} else if ( (*p) > 59 ) {
		if ( (*p) < 94 ) {
			if ( 63 <= (*p) && (*p) <= 90 )
				goto st57;
		} else if ( (*p) > 95 ) {
			if ( 97 <= (*p) && (*p) <= 123 )
				goto st57;
		} else
			goto st57;
	} else
		goto st57;
	goto tr90;
st71:
	if ( ++p == pe )
		goto _test_eof71;
case 71:
	switch( (*p) ) {
		case 33: goto st57;
		case 47: goto st72;
		case 92: goto st57;
		case 126: goto st57;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 37 ) {
			if ( 40 <= (*p) && (*p) <= 41 )
				goto st57;
		} else if ( (*p) >= 36 )
			goto st57;
	} else if ( (*p) > 59 ) {
		if ( (*p) < 94 ) {
			if ( 63 <= (*p) && (*p) <= 90 )
				goto st57;
		} else if ( (*p) > 95 ) {
			if ( 97 <= (*p) && (*p) <= 123 )
				goto st57;
		} else
			goto st57;
	} else
		goto st57;
	goto tr90;
st72:
	if ( ++p == pe )
		goto _test_eof72;
case 72:
	switch( (*p) ) {
		case 33: goto st57;
		case 35: goto tr62;
		case 40: goto tr115;
		case 41: goto st57;
		case 44: goto st57;
		case 46: goto st57;
		case 61: goto tr62;
		case 63: goto st57;
		case 92: goto st57;
		case 94: goto st57;
		case 95: goto tr115;
		case 123: goto st57;
		case 126: goto tr115;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 37 ) {
			if ( 38 <= (*p) && (*p) <= 42 )
				goto tr62;
		} else if ( (*p) >= 36 )
			goto tr115;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 64 ) {
			if ( 58 <= (*p) && (*p) <= 59 )
				goto st57;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr115;
		} else
			goto tr115;
	} else
		goto tr115;
	goto tr90;
tr62:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
	goto st73;
st73:
	if ( ++p == pe )
		goto _test_eof73;
case 73:
#line 1459 "wikitext_ragel.c"
	switch( (*p) ) {
		case 33: goto st53;
		case 41: goto st53;
		case 44: goto st53;
		case 46: goto st53;
		case 61: goto tr62;
		case 63: goto st53;
		case 95: goto tr62;
		case 126: goto tr62;
	}
	if ( (*p) < 58 ) {
		if ( 35 <= (*p) && (*p) <= 57 )
			goto tr62;
	} else if ( (*p) > 59 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr62;
		} else if ( (*p) >= 64 )
			goto tr62;
	} else
		goto st53;
	goto tr116;
st53:
	if ( ++p == pe )
		goto _test_eof53;
case 53:
	switch( (*p) ) {
		case 33: goto st53;
		case 41: goto st53;
		case 44: goto st53;
		case 46: goto st53;
		case 61: goto tr62;
		case 63: goto st53;
		case 95: goto tr62;
		case 126: goto tr62;
	}
	if ( (*p) < 58 ) {
		if ( 35 <= (*p) && (*p) <= 57 )
			goto tr62;
	} else if ( (*p) > 59 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr62;
		} else if ( (*p) >= 64 )
			goto tr62;
	} else
		goto st53;
	goto tr60;
tr115:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
#line 242 "wikitext_ragel.rl"
	{act = 16;}
	goto st74;
tr118:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
#line 344 "wikitext_ragel.rl"
	{act = 32;}
	goto st74;
st74:
	if ( ++p == pe )
		goto _test_eof74;
case 74:
#line 1524 "wikitext_ragel.c"
	switch( (*p) ) {
		case 33: goto tr118;
		case 35: goto tr62;
		case 40: goto tr115;
		case 41: goto tr118;
		case 44: goto tr118;
		case 46: goto tr118;
		case 61: goto tr62;
		case 63: goto tr118;
		case 92: goto st57;
		case 94: goto st57;
		case 95: goto tr115;
		case 123: goto st57;
		case 126: goto tr115;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 37 ) {
			if ( 38 <= (*p) && (*p) <= 42 )
				goto tr62;
		} else if ( (*p) >= 36 )
			goto tr115;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 64 ) {
			if ( 58 <= (*p) && (*p) <= 59 )
				goto tr118;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr115;
		} else
			goto tr115;
	} else
		goto tr115;
	goto tr117;
st75:
	if ( ++p == pe )
		goto _test_eof75;
case 75:
	switch( (*p) ) {
		case 33: goto st57;
		case 84: goto st67;
		case 92: goto st57;
		case 116: goto st67;
		case 126: goto st57;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 37 ) {
			if ( 40 <= (*p) && (*p) <= 41 )
				goto st57;
		} else if ( (*p) >= 36 )
			goto st57;
	} else if ( (*p) > 59 ) {
		if ( (*p) < 94 ) {
			if ( 63 <= (*p) && (*p) <= 90 )
				goto st57;
		} else if ( (*p) > 95 ) {
			if ( 97 <= (*p) && (*p) <= 123 )
				goto st57;
		} else
			goto st57;
	} else
		goto st57;
	goto tr90;
st76:
	if ( ++p == pe )
		goto _test_eof76;
case 76:
	switch( (*p) ) {
		case 33: goto st57;
		case 86: goto st77;
		case 92: goto st57;
		case 118: goto st77;
		case 126: goto st57;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 37 ) {
			if ( 40 <= (*p) && (*p) <= 41 )
				goto st57;
		} else if ( (*p) >= 36 )
			goto st57;
	} else if ( (*p) > 59 ) {
		if ( (*p) < 94 ) {
			if ( 63 <= (*p) && (*p) <= 90 )
				goto st57;
		} else if ( (*p) > 95 ) {
			if ( 97 <= (*p) && (*p) <= 123 )
				goto st57;
		} else
			goto st57;
	} else
		goto st57;
	goto tr90;
st77:
	if ( ++p == pe )
		goto _test_eof77;
case 77:
	switch( (*p) ) {
		case 33: goto st57;
		case 78: goto st69;
		case 92: goto st57;
		case 110: goto st69;
		case 126: goto st57;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 37 ) {
			if ( 40 <= (*p) && (*p) <= 41 )
				goto st57;
		} else if ( (*p) >= 36 )
			goto st57;
	} else if ( (*p) > 59 ) {
		if ( (*p) < 94 ) {
			if ( 63 <= (*p) && (*p) <= 90 )
				goto st57;
		} else if ( (*p) > 95 ) {
			if ( 97 <= (*p) && (*p) <= 123 )
				goto st57;
		} else
			goto st57;
	} else
		goto st57;
	goto tr90;
st78:
	if ( ++p == pe )
		goto _test_eof78;
case 78:
	if ( (*p) == 91 )
		goto tr121;
	goto tr120;
st79:
	if ( ++p == pe )
		goto _test_eof79;
case 79:
	if ( (*p) == 93 )
		goto tr123;
	goto tr122;
	}
	_test_eof54: cs = 54; goto _test_eof; 
	_test_eof1: cs = 1; goto _test_eof; 
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof55: cs = 55; goto _test_eof; 
	_test_eof56: cs = 56; goto _test_eof; 
	_test_eof57: cs = 57; goto _test_eof; 
	_test_eof58: cs = 58; goto _test_eof; 
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
	_test_eof59: cs = 59; goto _test_eof; 
	_test_eof60: cs = 60; goto _test_eof; 
	_test_eof61: cs = 61; goto _test_eof; 
	_test_eof62: cs = 62; goto _test_eof; 
	_test_eof63: cs = 63; goto _test_eof; 
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
	_test_eof64: cs = 64; goto _test_eof; 
	_test_eof65: cs = 65; goto _test_eof; 
	_test_eof66: cs = 66; goto _test_eof; 
	_test_eof67: cs = 67; goto _test_eof; 
	_test_eof68: cs = 68; goto _test_eof; 
	_test_eof69: cs = 69; goto _test_eof; 
	_test_eof70: cs = 70; goto _test_eof; 
	_test_eof71: cs = 71; goto _test_eof; 
	_test_eof72: cs = 72; goto _test_eof; 
	_test_eof73: cs = 73; goto _test_eof; 
	_test_eof53: cs = 53; goto _test_eof; 
	_test_eof74: cs = 74; goto _test_eof; 
	_test_eof75: cs = 75; goto _test_eof; 
	_test_eof76: cs = 76; goto _test_eof; 
	_test_eof77: cs = 77; goto _test_eof; 
	_test_eof78: cs = 78; goto _test_eof; 
	_test_eof79: cs = 79; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 55: goto tr86;
	case 56: goto tr88;
	case 57: goto tr90;
	case 58: goto tr91;
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
	case 59: goto tr95;
	case 60: goto tr95;
	case 61: goto tr95;
	case 62: goto tr95;
	case 63: goto tr100;
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
	case 64: goto tr106;
	case 65: goto tr106;
	case 66: goto tr108;
	case 67: goto tr90;
	case 68: goto tr90;
	case 69: goto tr90;
	case 70: goto tr90;
	case 71: goto tr90;
	case 72: goto tr90;
	case 73: goto tr116;
	case 53: goto tr60;
	case 74: goto tr117;
	case 75: goto tr90;
	case 76: goto tr90;
	case 77: goto tr90;
	case 78: goto tr120;
	case 79: goto tr122;
	}
	}

	_out: {}
	}
#line 416 "wikitext_ragel.rl"
    if (cs == wikitext_error)
        rb_raise(eWikitextParserError, "failed before finding a token");
    else if (out->type == NO_TOKEN)
        rb_raise(eWikitextParserError, "failed to produce a token");
}
