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

#line 385 "wikitext_ragel.rl"


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
#line 427 "wikitext_ragel.rl"
    
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
#line 376 "wikitext_ragel.rl"
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
#line 376 "wikitext_ragel.rl"
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
#line 376 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(DEFAULT);
            out->column_stop = out->column_start + 1;
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr7:
#line 326 "wikitext_ragel.rl"
	{{p = ((te))-1;}{
            EMIT(AMP);
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr10:
#line 314 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(DECIMAL_ENTITY);
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr12:
#line 308 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(HEX_ENTITY);
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr14:
#line 302 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(NAMED_ENTITY);
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr18:
#line 296 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(AMP_ENTITY);
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr22:
#line 290 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(QUOT_ENTITY);
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr23:
#line 332 "wikitext_ragel.rl"
	{{p = ((te))-1;}{
            EMIT(LESS);
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr29:
#line 127 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(EM_END);
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr35:
#line 85 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(NO_WIKI_END);
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr41:
#line 115 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(STRONG_END);
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr43:
#line 145 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(TT_END);
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr45:
#line 121 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(EM_START);
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr51:
#line 79 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(NO_WIKI_START);
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr57:
#line 109 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(STRONG_START);
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr59:
#line 139 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(TT_START);
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr60:
#line 248 "wikitext_ragel.rl"
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
#line 376 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(DEFAULT);
            out->column_stop = out->column_start + 1;
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr67:
#line 344 "wikitext_ragel.rl"
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
#line 320 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(QUOT);
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr72:
#line 178 "wikitext_ragel.rl"
	{te = p+1;{
            if (out->column_start == 1 || last_token_type == OL || last_token_type == UL || last_token_type == BLOCKQUOTE)
                EMIT(OL);
            else
                EMIT(PRINTABLE);
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr75:
#line 187 "wikitext_ragel.rl"
	{te = p+1;{
            if (out->column_start == 1 || last_token_type == OL || last_token_type == UL || last_token_type == BLOCKQUOTE)
                EMIT(UL);
            else
                EMIT(PRINTABLE);
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr85:
#line 133 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(TT);
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr86:
#line 272 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(SEPARATOR);
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr87:
#line 344 "wikitext_ragel.rl"
	{te = p;p--;{
            EMIT(CRLF);
            out->column_stop = 1;
            out->line_stop++;
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr88:
#line 344 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(CRLF);
            out->column_stop = 1;
            out->line_stop++;
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr89:
#line 166 "wikitext_ragel.rl"
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
tr91:
#line 356 "wikitext_ragel.rl"
	{te = p;p--;{
            EMIT(PRINTABLE);
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr92:
#line 326 "wikitext_ragel.rl"
	{te = p;p--;{
            EMIT(AMP);
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr96:
#line 91 "wikitext_ragel.rl"
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
tr100:
#line 91 "wikitext_ragel.rl"
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
tr103:
#line 1 "wikitext_ragel.rl"
	{	switch( act ) {
	case 16:
	{{p = ((te))-1;}
            EMIT(URI);
            {p++; cs = 54; goto _out;}
        }
	break;
	case 17:
	{{p = ((te))-1;}
            EMIT(MAIL);
            {p++; cs = 54; goto _out;}
        }
	break;
	case 33:
	{{p = ((te))-1;}
            EMIT(PRINTABLE);
            {p++; cs = 54; goto _out;}
        }
	break;
	default: break;
	}
	}
	goto st54;
tr107:
#line 254 "wikitext_ragel.rl"
	{te = p;p--;{
            EMIT(MAIL);
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr111:
#line 332 "wikitext_ragel.rl"
	{te = p;p--;{
            EMIT(LESS);
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr117:
#line 196 "wikitext_ragel.rl"
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
tr119:
#line 152 "wikitext_ragel.rl"
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
tr120:
#line 152 "wikitext_ragel.rl"
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
tr127:
#line 248 "wikitext_ragel.rl"
	{te = p;p--;{
            EMIT(URI);
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr130:
#line 278 "wikitext_ragel.rl"
	{te = p;p--;{
            EMIT(EXT_LINK_START);
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr131:
#line 260 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(LINK_START);
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr132:
#line 284 "wikitext_ragel.rl"
	{te = p;p--;{
            EMIT(EXT_LINK_END);
            {p++; cs = 54; goto _out;}
        }}
	goto st54;
tr133:
#line 266 "wikitext_ragel.rl"
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
#line 564 "wikitext_ragel.c"
	switch( (*p) ) {
		case 10: goto tr67;
		case 13: goto tr68;
		case 32: goto tr69;
		case 34: goto tr71;
		case 35: goto tr72;
		case 38: goto tr73;
		case 39: goto st59;
		case 42: goto tr75;
		case 47: goto st57;
		case 60: goto tr77;
		case 61: goto tr78;
		case 62: goto tr79;
		case 70: goto st75;
		case 72: goto st83;
		case 83: goto st84;
		case 91: goto st86;
		case 92: goto st57;
		case 93: goto st87;
		case 94: goto st57;
		case 96: goto tr85;
		case 102: goto st75;
		case 104: goto st83;
		case 115: goto st84;
		case 124: goto tr86;
		case 127: goto tr66;
	}
	if ( (*p) < 33 ) {
		if ( (*p) < -32 ) {
			if ( -62 <= (*p) && (*p) <= -33 )
				goto st1;
		} else if ( (*p) > -17 ) {
			if ( (*p) > -12 ) {
				if ( 1 <= (*p) && (*p) <= 31 )
					goto tr66;
			} else if ( (*p) >= -16 )
				goto st4;
		} else
			goto st2;
	} else if ( (*p) > 44 ) {
		if ( (*p) < 58 ) {
			if ( 45 <= (*p) && (*p) <= 57 )
				goto st63;
		} else if ( (*p) > 64 ) {
			if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 126 )
					goto st57;
			} else if ( (*p) >= 65 )
				goto st63;
		} else
			goto st57;
	} else
		goto st57;
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
#line 674 "wikitext_ragel.c"
	if ( (*p) == 10 )
		goto tr88;
	goto tr87;
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
#line 688 "wikitext_ragel.c"
	if ( (*p) == 32 )
		goto st56;
	goto tr89;
st57:
	if ( ++p == pe )
		goto _test_eof57;
case 57:
	switch( (*p) ) {
		case 33: goto st57;
		case 92: goto st57;
	}
	if ( (*p) < 63 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st57;
		} else if ( (*p) > 41 ) {
			if ( 43 <= (*p) && (*p) <= 59 )
				goto st57;
		} else
			goto st57;
	} else if ( (*p) > 90 ) {
		if ( (*p) < 97 ) {
			if ( 94 <= (*p) && (*p) <= 95 )
				goto st57;
		} else if ( (*p) > 123 ) {
			if ( 125 <= (*p) && (*p) <= 126 )
				goto st57;
		} else
			goto st57;
	} else
		goto st57;
	goto tr91;
tr73:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
	goto st58;
st58:
	if ( ++p == pe )
		goto _test_eof58;
case 58:
#line 729 "wikitext_ragel.c"
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
	goto tr92;
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
	goto tr96;
st60:
	if ( ++p == pe )
		goto _test_eof60;
case 60:
	if ( (*p) == 39 )
		goto st61;
	goto tr96;
st61:
	if ( ++p == pe )
		goto _test_eof61;
case 61:
	if ( (*p) == 39 )
		goto st62;
	goto tr96;
st62:
	if ( ++p == pe )
		goto _test_eof62;
case 62:
	if ( (*p) == 39 )
		goto tr100;
	goto tr96;
st63:
	if ( ++p == pe )
		goto _test_eof63;
case 63:
	switch( (*p) ) {
		case 33: goto st57;
		case 47: goto st57;
		case 63: goto st57;
		case 64: goto st64;
		case 92: goto st57;
		case 94: goto st57;
		case 95: goto st63;
		case 123: goto st57;
	}
	if ( (*p) < 45 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st57;
		} else if ( (*p) > 41 ) {
			if ( 43 <= (*p) && (*p) <= 44 )
				goto st57;
		} else
			goto st57;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 65 ) {
			if ( 58 <= (*p) && (*p) <= 59 )
				goto st57;
		} else if ( (*p) > 90 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st57;
			} else if ( (*p) >= 97 )
				goto st63;
		} else
			goto st63;
	} else
		goto st63;
	goto tr91;
st64:
	if ( ++p == pe )
		goto _test_eof64;
case 64:
	switch( (*p) ) {
		case 33: goto st57;
		case 92: goto st57;
		case 123: goto st57;
	}
	if ( (*p) < 58 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st57;
		} else if ( (*p) > 41 ) {
			if ( (*p) > 47 ) {
				if ( 48 <= (*p) && (*p) <= 57 )
					goto tr102;
			} else if ( (*p) >= 43 )
				goto st57;
		} else
			goto st57;
	} else if ( (*p) > 59 ) {
		if ( (*p) < 94 ) {
			if ( (*p) > 64 ) {
				if ( 65 <= (*p) && (*p) <= 90 )
					goto tr102;
			} else if ( (*p) >= 63 )
				goto st57;
		} else if ( (*p) > 95 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st57;
			} else if ( (*p) >= 97 )
				goto tr102;
		} else
			goto st57;
	} else
		goto st57;
	goto tr91;
tr102:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
#line 356 "wikitext_ragel.rl"
	{act = 33;}
	goto st65;
tr110:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
#line 254 "wikitext_ragel.rl"
	{act = 17;}
	goto st65;
st65:
	if ( ++p == pe )
		goto _test_eof65;
case 65:
#line 1049 "wikitext_ragel.c"
	switch( (*p) ) {
		case 33: goto st57;
		case 46: goto st66;
		case 92: goto st57;
		case 123: goto st57;
	}
	if ( (*p) < 58 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st57;
		} else if ( (*p) > 41 ) {
			if ( (*p) > 47 ) {
				if ( 48 <= (*p) && (*p) <= 57 )
					goto tr102;
			} else if ( (*p) >= 43 )
				goto st57;
		} else
			goto st57;
	} else if ( (*p) > 59 ) {
		if ( (*p) < 94 ) {
			if ( (*p) > 64 ) {
				if ( 65 <= (*p) && (*p) <= 90 )
					goto tr102;
			} else if ( (*p) >= 63 )
				goto st57;
		} else if ( (*p) > 95 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st57;
			} else if ( (*p) >= 97 )
				goto tr102;
		} else
			goto st57;
	} else
		goto st57;
	goto tr103;
st66:
	if ( ++p == pe )
		goto _test_eof66;
case 66:
	switch( (*p) ) {
		case 33: goto st57;
		case 92: goto st57;
		case 123: goto st57;
	}
	if ( (*p) < 58 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st57;
		} else if ( (*p) > 41 ) {
			if ( (*p) > 47 ) {
				if ( 48 <= (*p) && (*p) <= 57 )
					goto tr102;
			} else if ( (*p) >= 43 )
				goto st57;
		} else
			goto st57;
	} else if ( (*p) > 59 ) {
		if ( (*p) < 94 ) {
			if ( (*p) > 64 ) {
				if ( 65 <= (*p) && (*p) <= 90 )
					goto st67;
			} else if ( (*p) >= 63 )
				goto st57;
		} else if ( (*p) > 95 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st57;
			} else if ( (*p) >= 97 )
				goto st67;
		} else
			goto st57;
	} else
		goto st57;
	goto tr91;
st67:
	if ( ++p == pe )
		goto _test_eof67;
case 67:
	switch( (*p) ) {
		case 33: goto st57;
		case 46: goto st66;
		case 92: goto st57;
		case 123: goto st57;
	}
	if ( (*p) < 58 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st57;
		} else if ( (*p) > 41 ) {
			if ( (*p) > 47 ) {
				if ( 48 <= (*p) && (*p) <= 57 )
					goto tr102;
			} else if ( (*p) >= 43 )
				goto st57;
		} else
			goto st57;
	} else if ( (*p) > 59 ) {
		if ( (*p) < 94 ) {
			if ( (*p) > 64 ) {
				if ( 65 <= (*p) && (*p) <= 90 )
					goto st68;
			} else if ( (*p) >= 63 )
				goto st57;
		} else if ( (*p) > 95 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st57;
			} else if ( (*p) >= 97 )
				goto st68;
		} else
			goto st57;
	} else
		goto st57;
	goto tr91;
st68:
	if ( ++p == pe )
		goto _test_eof68;
case 68:
	switch( (*p) ) {
		case 33: goto st57;
		case 46: goto st66;
		case 92: goto st57;
		case 123: goto st57;
	}
	if ( (*p) < 58 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st57;
		} else if ( (*p) > 41 ) {
			if ( (*p) > 47 ) {
				if ( 48 <= (*p) && (*p) <= 57 )
					goto tr102;
			} else if ( (*p) >= 43 )
				goto st57;
		} else
			goto st57;
	} else if ( (*p) > 59 ) {
		if ( (*p) < 94 ) {
			if ( (*p) > 64 ) {
				if ( 65 <= (*p) && (*p) <= 90 )
					goto st69;
			} else if ( (*p) >= 63 )
				goto st57;
		} else if ( (*p) > 95 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st57;
			} else if ( (*p) >= 97 )
				goto st69;
		} else
			goto st57;
	} else
		goto st57;
	goto tr107;
st69:
	if ( ++p == pe )
		goto _test_eof69;
case 69:
	switch( (*p) ) {
		case 33: goto st57;
		case 46: goto st66;
		case 92: goto st57;
		case 123: goto st57;
	}
	if ( (*p) < 58 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st57;
		} else if ( (*p) > 41 ) {
			if ( (*p) > 47 ) {
				if ( 48 <= (*p) && (*p) <= 57 )
					goto tr102;
			} else if ( (*p) >= 43 )
				goto st57;
		} else
			goto st57;
	} else if ( (*p) > 59 ) {
		if ( (*p) < 94 ) {
			if ( (*p) > 64 ) {
				if ( 65 <= (*p) && (*p) <= 90 )
					goto st70;
			} else if ( (*p) >= 63 )
				goto st57;
		} else if ( (*p) > 95 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st57;
			} else if ( (*p) >= 97 )
				goto st70;
		} else
			goto st57;
	} else
		goto st57;
	goto tr107;
st70:
	if ( ++p == pe )
		goto _test_eof70;
case 70:
	switch( (*p) ) {
		case 33: goto st57;
		case 46: goto st66;
		case 92: goto st57;
		case 123: goto st57;
	}
	if ( (*p) < 58 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st57;
		} else if ( (*p) > 41 ) {
			if ( (*p) > 47 ) {
				if ( 48 <= (*p) && (*p) <= 57 )
					goto tr102;
			} else if ( (*p) >= 43 )
				goto st57;
		} else
			goto st57;
	} else if ( (*p) > 59 ) {
		if ( (*p) < 94 ) {
			if ( (*p) > 64 ) {
				if ( 65 <= (*p) && (*p) <= 90 )
					goto tr110;
			} else if ( (*p) >= 63 )
				goto st57;
		} else if ( (*p) > 95 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st57;
			} else if ( (*p) >= 97 )
				goto tr110;
		} else
			goto st57;
	} else
		goto st57;
	goto tr107;
tr77:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
	goto st71;
st71:
	if ( ++p == pe )
		goto _test_eof71;
case 71:
#line 1293 "wikitext_ragel.c"
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
	goto tr111;
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
tr78:
#line 36 "wikitext_ragel.rl"
	{
        MARK();
    }
	goto st72;
st72:
	if ( ++p == pe )
		goto _test_eof72;
case 72:
#line 1603 "wikitext_ragel.c"
	switch( (*p) ) {
		case 32: goto st73;
		case 61: goto tr78;
	}
	goto tr117;
st73:
	if ( ++p == pe )
		goto _test_eof73;
case 73:
	if ( (*p) == 32 )
		goto st73;
	goto tr117;
tr79:
#line 36 "wikitext_ragel.rl"
	{
        MARK();
    }
	goto st74;
st74:
	if ( ++p == pe )
		goto _test_eof74;
case 74:
#line 1626 "wikitext_ragel.c"
	if ( (*p) == 32 )
		goto tr120;
	goto tr119;
st75:
	if ( ++p == pe )
		goto _test_eof75;
case 75:
	switch( (*p) ) {
		case 33: goto st57;
		case 47: goto st57;
		case 63: goto st57;
		case 64: goto st64;
		case 84: goto st76;
		case 92: goto st57;
		case 94: goto st57;
		case 95: goto st63;
		case 116: goto st76;
		case 123: goto st57;
	}
	if ( (*p) < 45 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st57;
		} else if ( (*p) > 41 ) {
			if ( 43 <= (*p) && (*p) <= 44 )
				goto st57;
		} else
			goto st57;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 65 ) {
			if ( 58 <= (*p) && (*p) <= 59 )
				goto st57;
		} else if ( (*p) > 90 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st57;
			} else if ( (*p) >= 97 )
				goto st63;
		} else
			goto st63;
	} else
		goto st63;
	goto tr91;
st76:
	if ( ++p == pe )
		goto _test_eof76;
case 76:
	switch( (*p) ) {
		case 33: goto st57;
		case 47: goto st57;
		case 63: goto st57;
		case 64: goto st64;
		case 80: goto st77;
		case 92: goto st57;
		case 94: goto st57;
		case 95: goto st63;
		case 112: goto st77;
		case 123: goto st57;
	}
	if ( (*p) < 45 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st57;
		} else if ( (*p) > 41 ) {
			if ( 43 <= (*p) && (*p) <= 44 )
				goto st57;
		} else
			goto st57;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 65 ) {
			if ( 58 <= (*p) && (*p) <= 59 )
				goto st57;
		} else if ( (*p) > 90 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st57;
			} else if ( (*p) >= 97 )
				goto st63;
		} else
			goto st63;
	} else
		goto st63;
	goto tr91;
st77:
	if ( ++p == pe )
		goto _test_eof77;
case 77:
	switch( (*p) ) {
		case 33: goto st57;
		case 47: goto st57;
		case 58: goto st78;
		case 59: goto st57;
		case 63: goto st57;
		case 64: goto st64;
		case 92: goto st57;
		case 94: goto st57;
		case 95: goto st63;
		case 123: goto st57;
	}
	if ( (*p) < 45 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st57;
		} else if ( (*p) > 41 ) {
			if ( 43 <= (*p) && (*p) <= 44 )
				goto st57;
		} else
			goto st57;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 97 ) {
			if ( 65 <= (*p) && (*p) <= 90 )
				goto st63;
		} else if ( (*p) > 122 ) {
			if ( 125 <= (*p) && (*p) <= 126 )
				goto st57;
		} else
			goto st63;
	} else
		goto st63;
	goto tr91;
st78:
	if ( ++p == pe )
		goto _test_eof78;
case 78:
	switch( (*p) ) {
		case 33: goto st57;
		case 47: goto st79;
		case 92: goto st57;
	}
	if ( (*p) < 63 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st57;
		} else if ( (*p) > 41 ) {
			if ( 43 <= (*p) && (*p) <= 59 )
				goto st57;
		} else
			goto st57;
	} else if ( (*p) > 90 ) {
		if ( (*p) < 97 ) {
			if ( 94 <= (*p) && (*p) <= 95 )
				goto st57;
		} else if ( (*p) > 123 ) {
			if ( 125 <= (*p) && (*p) <= 126 )
				goto st57;
		} else
			goto st57;
	} else
		goto st57;
	goto tr91;
st79:
	if ( ++p == pe )
		goto _test_eof79;
case 79:
	switch( (*p) ) {
		case 33: goto st57;
		case 47: goto st80;
		case 92: goto st57;
	}
	if ( (*p) < 63 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st57;
		} else if ( (*p) > 41 ) {
			if ( 43 <= (*p) && (*p) <= 59 )
				goto st57;
		} else
			goto st57;
	} else if ( (*p) > 90 ) {
		if ( (*p) < 97 ) {
			if ( 94 <= (*p) && (*p) <= 95 )
				goto st57;
		} else if ( (*p) > 123 ) {
			if ( 125 <= (*p) && (*p) <= 126 )
				goto st57;
		} else
			goto st57;
	} else
		goto st57;
	goto tr91;
st80:
	if ( ++p == pe )
		goto _test_eof80;
case 80:
	switch( (*p) ) {
		case 33: goto st57;
		case 35: goto tr62;
		case 40: goto tr126;
		case 41: goto st57;
		case 44: goto st57;
		case 46: goto st57;
		case 61: goto tr62;
		case 63: goto st57;
		case 92: goto st57;
		case 94: goto st57;
		case 95: goto tr126;
		case 123: goto st57;
		case 125: goto st57;
		case 126: goto tr126;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 37 ) {
			if ( 38 <= (*p) && (*p) <= 42 )
				goto tr62;
		} else if ( (*p) >= 36 )
			goto tr126;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 64 ) {
			if ( 58 <= (*p) && (*p) <= 59 )
				goto st57;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr126;
		} else
			goto tr126;
	} else
		goto tr126;
	goto tr91;
tr62:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
	goto st81;
st81:
	if ( ++p == pe )
		goto _test_eof81;
case 81:
#line 1853 "wikitext_ragel.c"
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
	goto tr127;
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
tr128:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
#line 356 "wikitext_ragel.rl"
	{act = 33;}
	goto st82;
tr126:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
#line 248 "wikitext_ragel.rl"
	{act = 16;}
	goto st82;
st82:
	if ( ++p == pe )
		goto _test_eof82;
case 82:
#line 1918 "wikitext_ragel.c"
	switch( (*p) ) {
		case 33: goto tr128;
		case 35: goto tr62;
		case 40: goto tr126;
		case 41: goto tr128;
		case 44: goto tr128;
		case 46: goto tr128;
		case 61: goto tr62;
		case 63: goto tr128;
		case 92: goto st57;
		case 94: goto st57;
		case 95: goto tr126;
		case 123: goto st57;
		case 125: goto st57;
		case 126: goto tr126;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 37 ) {
			if ( 38 <= (*p) && (*p) <= 42 )
				goto tr62;
		} else if ( (*p) >= 36 )
			goto tr126;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 64 ) {
			if ( 58 <= (*p) && (*p) <= 59 )
				goto tr128;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr126;
		} else
			goto tr126;
	} else
		goto tr126;
	goto tr103;
st83:
	if ( ++p == pe )
		goto _test_eof83;
case 83:
	switch( (*p) ) {
		case 33: goto st57;
		case 47: goto st57;
		case 63: goto st57;
		case 64: goto st64;
		case 84: goto st75;
		case 92: goto st57;
		case 94: goto st57;
		case 95: goto st63;
		case 116: goto st75;
		case 123: goto st57;
	}
	if ( (*p) < 45 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st57;
		} else if ( (*p) > 41 ) {
			if ( 43 <= (*p) && (*p) <= 44 )
				goto st57;
		} else
			goto st57;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 65 ) {
			if ( 58 <= (*p) && (*p) <= 59 )
				goto st57;
		} else if ( (*p) > 90 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st57;
			} else if ( (*p) >= 97 )
				goto st63;
		} else
			goto st63;
	} else
		goto st63;
	goto tr91;
st84:
	if ( ++p == pe )
		goto _test_eof84;
case 84:
	switch( (*p) ) {
		case 33: goto st57;
		case 47: goto st57;
		case 63: goto st57;
		case 64: goto st64;
		case 86: goto st85;
		case 92: goto st57;
		case 94: goto st57;
		case 95: goto st63;
		case 118: goto st85;
		case 123: goto st57;
	}
	if ( (*p) < 45 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st57;
		} else if ( (*p) > 41 ) {
			if ( 43 <= (*p) && (*p) <= 44 )
				goto st57;
		} else
			goto st57;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 65 ) {
			if ( 58 <= (*p) && (*p) <= 59 )
				goto st57;
		} else if ( (*p) > 90 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st57;
			} else if ( (*p) >= 97 )
				goto st63;
		} else
			goto st63;
	} else
		goto st63;
	goto tr91;
st85:
	if ( ++p == pe )
		goto _test_eof85;
case 85:
	switch( (*p) ) {
		case 33: goto st57;
		case 47: goto st57;
		case 63: goto st57;
		case 64: goto st64;
		case 78: goto st77;
		case 92: goto st57;
		case 94: goto st57;
		case 95: goto st63;
		case 110: goto st77;
		case 123: goto st57;
	}
	if ( (*p) < 45 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st57;
		} else if ( (*p) > 41 ) {
			if ( 43 <= (*p) && (*p) <= 44 )
				goto st57;
		} else
			goto st57;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 65 ) {
			if ( 58 <= (*p) && (*p) <= 59 )
				goto st57;
		} else if ( (*p) > 90 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st57;
			} else if ( (*p) >= 97 )
				goto st63;
		} else
			goto st63;
	} else
		goto st63;
	goto tr91;
st86:
	if ( ++p == pe )
		goto _test_eof86;
case 86:
	if ( (*p) == 91 )
		goto tr131;
	goto tr130;
st87:
	if ( ++p == pe )
		goto _test_eof87;
case 87:
	if ( (*p) == 93 )
		goto tr133;
	goto tr132;
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
	_test_eof64: cs = 64; goto _test_eof; 
	_test_eof65: cs = 65; goto _test_eof; 
	_test_eof66: cs = 66; goto _test_eof; 
	_test_eof67: cs = 67; goto _test_eof; 
	_test_eof68: cs = 68; goto _test_eof; 
	_test_eof69: cs = 69; goto _test_eof; 
	_test_eof70: cs = 70; goto _test_eof; 
	_test_eof71: cs = 71; goto _test_eof; 
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
	_test_eof72: cs = 72; goto _test_eof; 
	_test_eof73: cs = 73; goto _test_eof; 
	_test_eof74: cs = 74; goto _test_eof; 
	_test_eof75: cs = 75; goto _test_eof; 
	_test_eof76: cs = 76; goto _test_eof; 
	_test_eof77: cs = 77; goto _test_eof; 
	_test_eof78: cs = 78; goto _test_eof; 
	_test_eof79: cs = 79; goto _test_eof; 
	_test_eof80: cs = 80; goto _test_eof; 
	_test_eof81: cs = 81; goto _test_eof; 
	_test_eof53: cs = 53; goto _test_eof; 
	_test_eof82: cs = 82; goto _test_eof; 
	_test_eof83: cs = 83; goto _test_eof; 
	_test_eof84: cs = 84; goto _test_eof; 
	_test_eof85: cs = 85; goto _test_eof; 
	_test_eof86: cs = 86; goto _test_eof; 
	_test_eof87: cs = 87; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 55: goto tr87;
	case 56: goto tr89;
	case 57: goto tr91;
	case 58: goto tr92;
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
	case 59: goto tr96;
	case 60: goto tr96;
	case 61: goto tr96;
	case 62: goto tr96;
	case 63: goto tr91;
	case 64: goto tr91;
	case 65: goto tr103;
	case 66: goto tr91;
	case 67: goto tr91;
	case 68: goto tr107;
	case 69: goto tr107;
	case 70: goto tr107;
	case 71: goto tr111;
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
	case 72: goto tr117;
	case 73: goto tr117;
	case 74: goto tr119;
	case 75: goto tr91;
	case 76: goto tr91;
	case 77: goto tr91;
	case 78: goto tr91;
	case 79: goto tr91;
	case 80: goto tr91;
	case 81: goto tr127;
	case 53: goto tr60;
	case 82: goto tr103;
	case 83: goto tr91;
	case 84: goto tr91;
	case 85: goto tr91;
	case 86: goto tr130;
	case 87: goto tr132;
	}
	}

	_out: {}
	}
#line 428 "wikitext_ragel.rl"
    if (cs == wikitext_error)
        rb_raise(eWikitextParserError, "failed before finding a token");
    else if (out->type == NO_TOKEN)
        rb_raise(eWikitextParserError, "failed to produce a token");
}
