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
static const int wikitext_start = 80;
static const int wikitext_first_final = 80;
static const int wikitext_error = 0;

static const int wikitext_en_main = 80;

#line 418 "wikitext_ragel.rl"


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
#line 460 "wikitext_ragel.rl"
    
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
#line 409 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(DEFAULT);
            out->column_stop = out->column_start + 1;
            {p++; cs = 80; goto _out;}
        }}
	goto st80;
tr3:
#line 52 "wikitext_ragel.rl"
	{
        out->code_point = ((uint32_t)(*(p - 2)) & 0x0f) << 12 |
            ((uint32_t)(*(p - 1)) & 0x3f) << 6 |
            (*p & 0x3f);
    }
#line 409 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(DEFAULT);
            out->column_stop = out->column_start + 1;
            {p++; cs = 80; goto _out;}
        }}
	goto st80;
tr6:
#line 59 "wikitext_ragel.rl"
	{
        out->code_point = ((uint32_t)(*(p - 3)) & 0x07) << 18 |
            ((uint32_t)(*(p - 2)) & 0x3f) << 12 |
            ((uint32_t)(*(p - 1)) & 0x3f) << 6 |
            (*p & 0x3f);
    }
#line 409 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(DEFAULT);
            out->column_stop = out->column_start + 1;
            {p++; cs = 80; goto _out;}
        }}
	goto st80;
tr7:
#line 359 "wikitext_ragel.rl"
	{{p = ((te))-1;}{
            EMIT(AMP);
            {p++; cs = 80; goto _out;}
        }}
	goto st80;
tr10:
#line 347 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(DECIMAL_ENTITY);
            {p++; cs = 80; goto _out;}
        }}
	goto st80;
tr12:
#line 341 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(HEX_ENTITY);
            {p++; cs = 80; goto _out;}
        }}
	goto st80;
tr14:
#line 335 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(NAMED_ENTITY);
            {p++; cs = 80; goto _out;}
        }}
	goto st80;
tr18:
#line 329 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(AMP_ENTITY);
            {p++; cs = 80; goto _out;}
        }}
	goto st80;
tr22:
#line 323 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(QUOT_ENTITY);
            {p++; cs = 80; goto _out;}
        }}
	goto st80;
tr23:
#line 365 "wikitext_ragel.rl"
	{{p = ((te))-1;}{
            EMIT(LESS);
            {p++; cs = 80; goto _out;}
        }}
	goto st80;
tr39:
#line 110 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(BLOCKQUOTE_END);
            {p++; cs = 80; goto _out;}
        }}
	goto st80;
tr41:
#line 152 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(EM_END);
            {p++; cs = 80; goto _out;}
        }}
	goto st80;
tr47:
#line 86 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(NO_WIKI_END);
            {p++; cs = 80; goto _out;}
        }}
	goto st80;
tr50:
#line 98 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(PRE_END);
            {p++; cs = 80; goto _out;}
        }}
	goto st80;
tr56:
#line 140 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(STRONG_END);
            {p++; cs = 80; goto _out;}
        }}
	goto st80;
tr58:
#line 170 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(TT_END);
            {p++; cs = 80; goto _out;}
        }}
	goto st80;
tr68:
#line 104 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(BLOCKQUOTE_START);
            {p++; cs = 80; goto _out;}
        }}
	goto st80;
tr70:
#line 146 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(EM_START);
            {p++; cs = 80; goto _out;}
        }}
	goto st80;
tr76:
#line 80 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(NO_WIKI_START);
            {p++; cs = 80; goto _out;}
        }}
	goto st80;
tr79:
#line 92 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(PRE_START);
            {p++; cs = 80; goto _out;}
        }}
	goto st80;
tr85:
#line 134 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(STRONG_START);
            {p++; cs = 80; goto _out;}
        }}
	goto st80;
tr87:
#line 164 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(TT_START);
            {p++; cs = 80; goto _out;}
        }}
	goto st80;
tr88:
#line 281 "wikitext_ragel.rl"
	{{p = ((te))-1;}{
            EMIT(URI);
            {p++; cs = 80; goto _out;}
        }}
	goto st80;
tr94:
#line 41 "wikitext_ragel.rl"
	{
        out->code_point = *p & 0x7f;
    }
#line 409 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(DEFAULT);
            out->column_stop = out->column_start + 1;
            {p++; cs = 80; goto _out;}
        }}
	goto st80;
tr95:
#line 377 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(CRLF);
            out->column_stop = 1;
            out->line_stop++;
            {p++; cs = 80; goto _out;}
        }}
#line 41 "wikitext_ragel.rl"
	{
        out->code_point = *p & 0x7f;
    }
	goto st80;
tr99:
#line 353 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(QUOT);
            {p++; cs = 80; goto _out;}
        }}
	goto st80;
tr100:
#line 202 "wikitext_ragel.rl"
	{te = p+1;{
            if (out->column_start == 1              ||
                last_token_type == OL               ||
                last_token_type == UL               ||
                last_token_type == BLOCKQUOTE       ||
                last_token_type == BLOCKQUOTE_START)
                EMIT(OL);
            else
                EMIT(PRINTABLE);
            {p++; cs = 80; goto _out;}
        }}
	goto st80;
tr103:
#line 215 "wikitext_ragel.rl"
	{te = p+1;{
            if (out->column_start == 1              ||
                last_token_type == OL               ||
                last_token_type == UL               ||
                last_token_type == BLOCKQUOTE       ||
                last_token_type == BLOCKQUOTE_START)
                EMIT(UL);
            else
                EMIT(PRINTABLE);
            {p++; cs = 80; goto _out;}
        }}
	goto st80;
tr114:
#line 158 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(TT);
            {p++; cs = 80; goto _out;}
        }}
	goto st80;
tr115:
#line 305 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(SEPARATOR);
            {p++; cs = 80; goto _out;}
        }}
	goto st80;
tr116:
#line 377 "wikitext_ragel.rl"
	{te = p;p--;{
            EMIT(CRLF);
            out->column_stop = 1;
            out->line_stop++;
            {p++; cs = 80; goto _out;}
        }}
	goto st80;
tr117:
#line 377 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(CRLF);
            out->column_stop = 1;
            out->line_stop++;
            {p++; cs = 80; goto _out;}
        }}
	goto st80;
tr118:
#line 190 "wikitext_ragel.rl"
	{te = p;p--;{
            if (out->column_start == 1 || last_token_type == BLOCKQUOTE)
            {
                REWIND();
                EMIT(PRE);
            }
            else
                EMIT(SPACE);
            {p++; cs = 80; goto _out;}
        }}
	goto st80;
tr120:
#line 389 "wikitext_ragel.rl"
	{te = p;p--;{
            EMIT(PRINTABLE);
            {p++; cs = 80; goto _out;}
        }}
	goto st80;
tr121:
#line 359 "wikitext_ragel.rl"
	{te = p;p--;{
            EMIT(AMP);
            {p++; cs = 80; goto _out;}
        }}
	goto st80;
tr125:
#line 116 "wikitext_ragel.rl"
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
            {p++; cs = 80; goto _out;}
        }}
	goto st80;
tr129:
#line 116 "wikitext_ragel.rl"
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
            {p++; cs = 80; goto _out;}
        }}
	goto st80;
tr132:
#line 1 "wikitext_ragel.rl"
	{	switch( act ) {
	case 20:
	{{p = ((te))-1;}
            EMIT(URI);
            {p++; cs = 80; goto _out;}
        }
	break;
	case 21:
	{{p = ((te))-1;}
            EMIT(MAIL);
            {p++; cs = 80; goto _out;}
        }
	break;
	case 37:
	{{p = ((te))-1;}
            EMIT(PRINTABLE);
            {p++; cs = 80; goto _out;}
        }
	break;
	default: break;
	}
	}
	goto st80;
tr136:
#line 287 "wikitext_ragel.rl"
	{te = p;p--;{
            EMIT(MAIL);
            {p++; cs = 80; goto _out;}
        }}
	goto st80;
tr140:
#line 365 "wikitext_ragel.rl"
	{te = p;p--;{
            EMIT(LESS);
            {p++; cs = 80; goto _out;}
        }}
	goto st80;
tr148:
#line 228 "wikitext_ragel.rl"
	{te = p;p--;{
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
            {p++; cs = 80; goto _out;}
        }}
	goto st80;
tr150:
#line 177 "wikitext_ragel.rl"
	{te = p;p--;{
            if (out->column_start == 1 || last_token_type == BLOCKQUOTE)
                EMIT(BLOCKQUOTE);
            else
            {
                REWIND();
                EMIT(GREATER);
            }
            {p++; cs = 80; goto _out;}
        }}
	goto st80;
tr151:
#line 177 "wikitext_ragel.rl"
	{te = p+1;{
            if (out->column_start == 1 || last_token_type == BLOCKQUOTE)
                EMIT(BLOCKQUOTE);
            else
            {
                REWIND();
                EMIT(GREATER);
            }
            {p++; cs = 80; goto _out;}
        }}
	goto st80;
tr158:
#line 281 "wikitext_ragel.rl"
	{te = p;p--;{
            EMIT(URI);
            {p++; cs = 80; goto _out;}
        }}
	goto st80;
tr179:
#line 311 "wikitext_ragel.rl"
	{te = p;p--;{
            EMIT(EXT_LINK_START);
            {p++; cs = 80; goto _out;}
        }}
	goto st80;
tr180:
#line 293 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(LINK_START);
            {p++; cs = 80; goto _out;}
        }}
	goto st80;
tr181:
#line 317 "wikitext_ragel.rl"
	{te = p;p--;{
            EMIT(EXT_LINK_END);
            {p++; cs = 80; goto _out;}
        }}
	goto st80;
tr182:
#line 299 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(LINK_END);
            {p++; cs = 80; goto _out;}
        }}
	goto st80;
st80:
#line 1 "wikitext_ragel.rl"
	{ts = 0;}
	if ( ++p == pe )
		goto _test_eof80;
case 80:
#line 1 "wikitext_ragel.rl"
	{ts = p;}
#line 601 "wikitext_ragel.c"
	switch( (*p) ) {
		case 10: goto tr95;
		case 13: goto tr96;
		case 32: goto tr97;
		case 34: goto tr99;
		case 35: goto tr100;
		case 38: goto tr101;
		case 39: goto st85;
		case 42: goto tr103;
		case 47: goto st83;
		case 60: goto tr105;
		case 61: goto tr106;
		case 62: goto tr107;
		case 70: goto st101;
		case 72: goto st109;
		case 77: goto st113;
		case 83: goto st128;
		case 91: goto st130;
		case 92: goto st83;
		case 93: goto st131;
		case 94: goto st83;
		case 96: goto tr114;
		case 102: goto st101;
		case 104: goto st109;
		case 109: goto st113;
		case 115: goto st128;
		case 124: goto tr115;
		case 127: goto tr94;
	}
	if ( (*p) < 33 ) {
		if ( (*p) < -32 ) {
			if ( -62 <= (*p) && (*p) <= -33 )
				goto st1;
		} else if ( (*p) > -17 ) {
			if ( (*p) > -12 ) {
				if ( 1 <= (*p) && (*p) <= 31 )
					goto tr94;
			} else if ( (*p) >= -16 )
				goto st4;
		} else
			goto st2;
	} else if ( (*p) > 44 ) {
		if ( (*p) < 58 ) {
			if ( 45 <= (*p) && (*p) <= 57 )
				goto st89;
		} else if ( (*p) > 64 ) {
			if ( (*p) > 122 ) {
				if ( 123 <= (*p) && (*p) <= 126 )
					goto st83;
			} else if ( (*p) >= 65 )
				goto st89;
		} else
			goto st83;
	} else
		goto st83;
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
tr96:
#line 41 "wikitext_ragel.rl"
	{
        out->code_point = *p & 0x7f;
    }
	goto st81;
st81:
	if ( ++p == pe )
		goto _test_eof81;
case 81:
#line 713 "wikitext_ragel.c"
	if ( (*p) == 10 )
		goto tr117;
	goto tr116;
tr97:
#line 36 "wikitext_ragel.rl"
	{
        MARK();
    }
	goto st82;
st82:
	if ( ++p == pe )
		goto _test_eof82;
case 82:
#line 727 "wikitext_ragel.c"
	if ( (*p) == 32 )
		goto st82;
	goto tr118;
st83:
	if ( ++p == pe )
		goto _test_eof83;
case 83:
	switch( (*p) ) {
		case 33: goto st83;
		case 92: goto st83;
	}
	if ( (*p) < 63 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st83;
		} else if ( (*p) > 41 ) {
			if ( 43 <= (*p) && (*p) <= 59 )
				goto st83;
		} else
			goto st83;
	} else if ( (*p) > 90 ) {
		if ( (*p) < 97 ) {
			if ( 94 <= (*p) && (*p) <= 95 )
				goto st83;
		} else if ( (*p) > 123 ) {
			if ( 125 <= (*p) && (*p) <= 126 )
				goto st83;
		} else
			goto st83;
	} else
		goto st83;
	goto tr120;
tr101:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
	goto st84;
st84:
	if ( ++p == pe )
		goto _test_eof84;
case 84:
#line 768 "wikitext_ragel.c"
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
	goto tr121;
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
st85:
	if ( ++p == pe )
		goto _test_eof85;
case 85:
	if ( (*p) == 39 )
		goto st86;
	goto tr125;
st86:
	if ( ++p == pe )
		goto _test_eof86;
case 86:
	if ( (*p) == 39 )
		goto st87;
	goto tr125;
st87:
	if ( ++p == pe )
		goto _test_eof87;
case 87:
	if ( (*p) == 39 )
		goto st88;
	goto tr125;
st88:
	if ( ++p == pe )
		goto _test_eof88;
case 88:
	if ( (*p) == 39 )
		goto tr129;
	goto tr125;
st89:
	if ( ++p == pe )
		goto _test_eof89;
case 89:
	switch( (*p) ) {
		case 33: goto st83;
		case 47: goto st83;
		case 63: goto st83;
		case 64: goto st90;
		case 92: goto st83;
		case 94: goto st83;
		case 95: goto st89;
		case 123: goto st83;
	}
	if ( (*p) < 45 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st83;
		} else if ( (*p) > 41 ) {
			if ( 43 <= (*p) && (*p) <= 44 )
				goto st83;
		} else
			goto st83;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 65 ) {
			if ( 58 <= (*p) && (*p) <= 59 )
				goto st83;
		} else if ( (*p) > 90 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st83;
			} else if ( (*p) >= 97 )
				goto st89;
		} else
			goto st89;
	} else
		goto st89;
	goto tr120;
st90:
	if ( ++p == pe )
		goto _test_eof90;
case 90:
	switch( (*p) ) {
		case 33: goto st83;
		case 92: goto st83;
		case 123: goto st83;
	}
	if ( (*p) < 58 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st83;
		} else if ( (*p) > 41 ) {
			if ( (*p) > 47 ) {
				if ( 48 <= (*p) && (*p) <= 57 )
					goto tr131;
			} else if ( (*p) >= 43 )
				goto st83;
		} else
			goto st83;
	} else if ( (*p) > 59 ) {
		if ( (*p) < 94 ) {
			if ( (*p) > 64 ) {
				if ( 65 <= (*p) && (*p) <= 90 )
					goto tr131;
			} else if ( (*p) >= 63 )
				goto st83;
		} else if ( (*p) > 95 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st83;
			} else if ( (*p) >= 97 )
				goto tr131;
		} else
			goto st83;
	} else
		goto st83;
	goto tr120;
tr131:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
#line 389 "wikitext_ragel.rl"
	{act = 37;}
	goto st91;
tr139:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
#line 287 "wikitext_ragel.rl"
	{act = 21;}
	goto st91;
st91:
	if ( ++p == pe )
		goto _test_eof91;
case 91:
#line 1088 "wikitext_ragel.c"
	switch( (*p) ) {
		case 33: goto st83;
		case 46: goto st92;
		case 92: goto st83;
		case 123: goto st83;
	}
	if ( (*p) < 58 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st83;
		} else if ( (*p) > 41 ) {
			if ( (*p) > 47 ) {
				if ( 48 <= (*p) && (*p) <= 57 )
					goto tr131;
			} else if ( (*p) >= 43 )
				goto st83;
		} else
			goto st83;
	} else if ( (*p) > 59 ) {
		if ( (*p) < 94 ) {
			if ( (*p) > 64 ) {
				if ( 65 <= (*p) && (*p) <= 90 )
					goto tr131;
			} else if ( (*p) >= 63 )
				goto st83;
		} else if ( (*p) > 95 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st83;
			} else if ( (*p) >= 97 )
				goto tr131;
		} else
			goto st83;
	} else
		goto st83;
	goto tr132;
st92:
	if ( ++p == pe )
		goto _test_eof92;
case 92:
	switch( (*p) ) {
		case 33: goto st83;
		case 92: goto st83;
		case 123: goto st83;
	}
	if ( (*p) < 58 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st83;
		} else if ( (*p) > 41 ) {
			if ( (*p) > 47 ) {
				if ( 48 <= (*p) && (*p) <= 57 )
					goto tr131;
			} else if ( (*p) >= 43 )
				goto st83;
		} else
			goto st83;
	} else if ( (*p) > 59 ) {
		if ( (*p) < 94 ) {
			if ( (*p) > 64 ) {
				if ( 65 <= (*p) && (*p) <= 90 )
					goto st93;
			} else if ( (*p) >= 63 )
				goto st83;
		} else if ( (*p) > 95 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st83;
			} else if ( (*p) >= 97 )
				goto st93;
		} else
			goto st83;
	} else
		goto st83;
	goto tr120;
st93:
	if ( ++p == pe )
		goto _test_eof93;
case 93:
	switch( (*p) ) {
		case 33: goto st83;
		case 46: goto st92;
		case 92: goto st83;
		case 123: goto st83;
	}
	if ( (*p) < 58 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st83;
		} else if ( (*p) > 41 ) {
			if ( (*p) > 47 ) {
				if ( 48 <= (*p) && (*p) <= 57 )
					goto tr131;
			} else if ( (*p) >= 43 )
				goto st83;
		} else
			goto st83;
	} else if ( (*p) > 59 ) {
		if ( (*p) < 94 ) {
			if ( (*p) > 64 ) {
				if ( 65 <= (*p) && (*p) <= 90 )
					goto st94;
			} else if ( (*p) >= 63 )
				goto st83;
		} else if ( (*p) > 95 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st83;
			} else if ( (*p) >= 97 )
				goto st94;
		} else
			goto st83;
	} else
		goto st83;
	goto tr120;
st94:
	if ( ++p == pe )
		goto _test_eof94;
case 94:
	switch( (*p) ) {
		case 33: goto st83;
		case 46: goto st92;
		case 92: goto st83;
		case 123: goto st83;
	}
	if ( (*p) < 58 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st83;
		} else if ( (*p) > 41 ) {
			if ( (*p) > 47 ) {
				if ( 48 <= (*p) && (*p) <= 57 )
					goto tr131;
			} else if ( (*p) >= 43 )
				goto st83;
		} else
			goto st83;
	} else if ( (*p) > 59 ) {
		if ( (*p) < 94 ) {
			if ( (*p) > 64 ) {
				if ( 65 <= (*p) && (*p) <= 90 )
					goto st95;
			} else if ( (*p) >= 63 )
				goto st83;
		} else if ( (*p) > 95 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st83;
			} else if ( (*p) >= 97 )
				goto st95;
		} else
			goto st83;
	} else
		goto st83;
	goto tr136;
st95:
	if ( ++p == pe )
		goto _test_eof95;
case 95:
	switch( (*p) ) {
		case 33: goto st83;
		case 46: goto st92;
		case 92: goto st83;
		case 123: goto st83;
	}
	if ( (*p) < 58 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st83;
		} else if ( (*p) > 41 ) {
			if ( (*p) > 47 ) {
				if ( 48 <= (*p) && (*p) <= 57 )
					goto tr131;
			} else if ( (*p) >= 43 )
				goto st83;
		} else
			goto st83;
	} else if ( (*p) > 59 ) {
		if ( (*p) < 94 ) {
			if ( (*p) > 64 ) {
				if ( 65 <= (*p) && (*p) <= 90 )
					goto st96;
			} else if ( (*p) >= 63 )
				goto st83;
		} else if ( (*p) > 95 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st83;
			} else if ( (*p) >= 97 )
				goto st96;
		} else
			goto st83;
	} else
		goto st83;
	goto tr136;
st96:
	if ( ++p == pe )
		goto _test_eof96;
case 96:
	switch( (*p) ) {
		case 33: goto st83;
		case 46: goto st92;
		case 92: goto st83;
		case 123: goto st83;
	}
	if ( (*p) < 58 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st83;
		} else if ( (*p) > 41 ) {
			if ( (*p) > 47 ) {
				if ( 48 <= (*p) && (*p) <= 57 )
					goto tr131;
			} else if ( (*p) >= 43 )
				goto st83;
		} else
			goto st83;
	} else if ( (*p) > 59 ) {
		if ( (*p) < 94 ) {
			if ( (*p) > 64 ) {
				if ( 65 <= (*p) && (*p) <= 90 )
					goto tr139;
			} else if ( (*p) >= 63 )
				goto st83;
		} else if ( (*p) > 95 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st83;
			} else if ( (*p) >= 97 )
				goto tr139;
		} else
			goto st83;
	} else
		goto st83;
	goto tr136;
tr105:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
	goto st97;
st97:
	if ( ++p == pe )
		goto _test_eof97;
case 97:
#line 1332 "wikitext_ragel.c"
	switch( (*p) ) {
		case 47: goto st20;
		case 66: goto st50;
		case 69: goto st60;
		case 78: goto st62;
		case 80: goto st68;
		case 83: goto st71;
		case 84: goto st77;
		case 98: goto st50;
		case 101: goto st60;
		case 110: goto st62;
		case 112: goto st68;
		case 115: goto st71;
		case 116: goto st77;
	}
	goto tr140;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
	switch( (*p) ) {
		case 66: goto st21;
		case 69: goto st31;
		case 78: goto st33;
		case 80: goto st39;
		case 83: goto st42;
		case 84: goto st48;
		case 98: goto st21;
		case 101: goto st31;
		case 110: goto st33;
		case 112: goto st39;
		case 115: goto st42;
		case 116: goto st48;
	}
	goto tr23;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
	switch( (*p) ) {
		case 76: goto st22;
		case 108: goto st22;
	}
	goto tr23;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
	switch( (*p) ) {
		case 79: goto st23;
		case 111: goto st23;
	}
	goto tr23;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
	switch( (*p) ) {
		case 67: goto st24;
		case 99: goto st24;
	}
	goto tr23;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
	switch( (*p) ) {
		case 75: goto st25;
		case 107: goto st25;
	}
	goto tr23;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
	switch( (*p) ) {
		case 81: goto st26;
		case 113: goto st26;
	}
	goto tr23;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
	switch( (*p) ) {
		case 85: goto st27;
		case 117: goto st27;
	}
	goto tr23;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
	switch( (*p) ) {
		case 79: goto st28;
		case 111: goto st28;
	}
	goto tr23;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
	switch( (*p) ) {
		case 84: goto st29;
		case 116: goto st29;
	}
	goto tr23;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
	switch( (*p) ) {
		case 69: goto st30;
		case 101: goto st30;
	}
	goto tr23;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
	if ( (*p) == 62 )
		goto tr39;
	goto tr23;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
	switch( (*p) ) {
		case 77: goto st32;
		case 109: goto st32;
	}
	goto tr23;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
	if ( (*p) == 62 )
		goto tr41;
	goto tr23;
st33:
	if ( ++p == pe )
		goto _test_eof33;
case 33:
	switch( (*p) ) {
		case 79: goto st34;
		case 111: goto st34;
	}
	goto tr23;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
	switch( (*p) ) {
		case 87: goto st35;
		case 119: goto st35;
	}
	goto tr23;
st35:
	if ( ++p == pe )
		goto _test_eof35;
case 35:
	switch( (*p) ) {
		case 73: goto st36;
		case 105: goto st36;
	}
	goto tr23;
st36:
	if ( ++p == pe )
		goto _test_eof36;
case 36:
	switch( (*p) ) {
		case 75: goto st37;
		case 107: goto st37;
	}
	goto tr23;
st37:
	if ( ++p == pe )
		goto _test_eof37;
case 37:
	switch( (*p) ) {
		case 73: goto st38;
		case 105: goto st38;
	}
	goto tr23;
st38:
	if ( ++p == pe )
		goto _test_eof38;
case 38:
	if ( (*p) == 62 )
		goto tr47;
	goto tr23;
st39:
	if ( ++p == pe )
		goto _test_eof39;
case 39:
	switch( (*p) ) {
		case 82: goto st40;
		case 114: goto st40;
	}
	goto tr23;
st40:
	if ( ++p == pe )
		goto _test_eof40;
case 40:
	switch( (*p) ) {
		case 69: goto st41;
		case 101: goto st41;
	}
	goto tr23;
st41:
	if ( ++p == pe )
		goto _test_eof41;
case 41:
	if ( (*p) == 62 )
		goto tr50;
	goto tr23;
st42:
	if ( ++p == pe )
		goto _test_eof42;
case 42:
	switch( (*p) ) {
		case 84: goto st43;
		case 116: goto st43;
	}
	goto tr23;
st43:
	if ( ++p == pe )
		goto _test_eof43;
case 43:
	switch( (*p) ) {
		case 82: goto st44;
		case 114: goto st44;
	}
	goto tr23;
st44:
	if ( ++p == pe )
		goto _test_eof44;
case 44:
	switch( (*p) ) {
		case 79: goto st45;
		case 111: goto st45;
	}
	goto tr23;
st45:
	if ( ++p == pe )
		goto _test_eof45;
case 45:
	switch( (*p) ) {
		case 78: goto st46;
		case 110: goto st46;
	}
	goto tr23;
st46:
	if ( ++p == pe )
		goto _test_eof46;
case 46:
	switch( (*p) ) {
		case 71: goto st47;
		case 103: goto st47;
	}
	goto tr23;
st47:
	if ( ++p == pe )
		goto _test_eof47;
case 47:
	if ( (*p) == 62 )
		goto tr56;
	goto tr23;
st48:
	if ( ++p == pe )
		goto _test_eof48;
case 48:
	switch( (*p) ) {
		case 84: goto st49;
		case 116: goto st49;
	}
	goto tr23;
st49:
	if ( ++p == pe )
		goto _test_eof49;
case 49:
	if ( (*p) == 62 )
		goto tr58;
	goto tr23;
st50:
	if ( ++p == pe )
		goto _test_eof50;
case 50:
	switch( (*p) ) {
		case 76: goto st51;
		case 108: goto st51;
	}
	goto tr23;
st51:
	if ( ++p == pe )
		goto _test_eof51;
case 51:
	switch( (*p) ) {
		case 79: goto st52;
		case 111: goto st52;
	}
	goto tr23;
st52:
	if ( ++p == pe )
		goto _test_eof52;
case 52:
	switch( (*p) ) {
		case 67: goto st53;
		case 99: goto st53;
	}
	goto tr23;
st53:
	if ( ++p == pe )
		goto _test_eof53;
case 53:
	switch( (*p) ) {
		case 75: goto st54;
		case 107: goto st54;
	}
	goto tr23;
st54:
	if ( ++p == pe )
		goto _test_eof54;
case 54:
	switch( (*p) ) {
		case 81: goto st55;
		case 113: goto st55;
	}
	goto tr23;
st55:
	if ( ++p == pe )
		goto _test_eof55;
case 55:
	switch( (*p) ) {
		case 85: goto st56;
		case 117: goto st56;
	}
	goto tr23;
st56:
	if ( ++p == pe )
		goto _test_eof56;
case 56:
	switch( (*p) ) {
		case 79: goto st57;
		case 111: goto st57;
	}
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
	switch( (*p) ) {
		case 69: goto st59;
		case 101: goto st59;
	}
	goto tr23;
st59:
	if ( ++p == pe )
		goto _test_eof59;
case 59:
	if ( (*p) == 62 )
		goto tr68;
	goto tr23;
st60:
	if ( ++p == pe )
		goto _test_eof60;
case 60:
	switch( (*p) ) {
		case 77: goto st61;
		case 109: goto st61;
	}
	goto tr23;
st61:
	if ( ++p == pe )
		goto _test_eof61;
case 61:
	if ( (*p) == 62 )
		goto tr70;
	goto tr23;
st62:
	if ( ++p == pe )
		goto _test_eof62;
case 62:
	switch( (*p) ) {
		case 79: goto st63;
		case 111: goto st63;
	}
	goto tr23;
st63:
	if ( ++p == pe )
		goto _test_eof63;
case 63:
	switch( (*p) ) {
		case 87: goto st64;
		case 119: goto st64;
	}
	goto tr23;
st64:
	if ( ++p == pe )
		goto _test_eof64;
case 64:
	switch( (*p) ) {
		case 73: goto st65;
		case 105: goto st65;
	}
	goto tr23;
st65:
	if ( ++p == pe )
		goto _test_eof65;
case 65:
	switch( (*p) ) {
		case 75: goto st66;
		case 107: goto st66;
	}
	goto tr23;
st66:
	if ( ++p == pe )
		goto _test_eof66;
case 66:
	switch( (*p) ) {
		case 73: goto st67;
		case 105: goto st67;
	}
	goto tr23;
st67:
	if ( ++p == pe )
		goto _test_eof67;
case 67:
	if ( (*p) == 62 )
		goto tr76;
	goto tr23;
st68:
	if ( ++p == pe )
		goto _test_eof68;
case 68:
	switch( (*p) ) {
		case 82: goto st69;
		case 114: goto st69;
	}
	goto tr23;
st69:
	if ( ++p == pe )
		goto _test_eof69;
case 69:
	switch( (*p) ) {
		case 69: goto st70;
		case 101: goto st70;
	}
	goto tr23;
st70:
	if ( ++p == pe )
		goto _test_eof70;
case 70:
	if ( (*p) == 62 )
		goto tr79;
	goto tr23;
st71:
	if ( ++p == pe )
		goto _test_eof71;
case 71:
	switch( (*p) ) {
		case 84: goto st72;
		case 116: goto st72;
	}
	goto tr23;
st72:
	if ( ++p == pe )
		goto _test_eof72;
case 72:
	switch( (*p) ) {
		case 82: goto st73;
		case 114: goto st73;
	}
	goto tr23;
st73:
	if ( ++p == pe )
		goto _test_eof73;
case 73:
	switch( (*p) ) {
		case 79: goto st74;
		case 111: goto st74;
	}
	goto tr23;
st74:
	if ( ++p == pe )
		goto _test_eof74;
case 74:
	switch( (*p) ) {
		case 78: goto st75;
		case 110: goto st75;
	}
	goto tr23;
st75:
	if ( ++p == pe )
		goto _test_eof75;
case 75:
	switch( (*p) ) {
		case 71: goto st76;
		case 103: goto st76;
	}
	goto tr23;
st76:
	if ( ++p == pe )
		goto _test_eof76;
case 76:
	if ( (*p) == 62 )
		goto tr85;
	goto tr23;
st77:
	if ( ++p == pe )
		goto _test_eof77;
case 77:
	switch( (*p) ) {
		case 84: goto st78;
		case 116: goto st78;
	}
	goto tr23;
st78:
	if ( ++p == pe )
		goto _test_eof78;
case 78:
	if ( (*p) == 62 )
		goto tr87;
	goto tr23;
tr106:
#line 36 "wikitext_ragel.rl"
	{
        MARK();
    }
	goto st98;
st98:
	if ( ++p == pe )
		goto _test_eof98;
case 98:
#line 1876 "wikitext_ragel.c"
	switch( (*p) ) {
		case 32: goto st99;
		case 61: goto tr106;
	}
	goto tr148;
st99:
	if ( ++p == pe )
		goto _test_eof99;
case 99:
	if ( (*p) == 32 )
		goto st99;
	goto tr148;
tr107:
#line 36 "wikitext_ragel.rl"
	{
        MARK();
    }
	goto st100;
st100:
	if ( ++p == pe )
		goto _test_eof100;
case 100:
#line 1899 "wikitext_ragel.c"
	if ( (*p) == 32 )
		goto tr151;
	goto tr150;
st101:
	if ( ++p == pe )
		goto _test_eof101;
case 101:
	switch( (*p) ) {
		case 33: goto st83;
		case 47: goto st83;
		case 63: goto st83;
		case 64: goto st90;
		case 84: goto st102;
		case 92: goto st83;
		case 94: goto st83;
		case 95: goto st89;
		case 116: goto st102;
		case 123: goto st83;
	}
	if ( (*p) < 45 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st83;
		} else if ( (*p) > 41 ) {
			if ( 43 <= (*p) && (*p) <= 44 )
				goto st83;
		} else
			goto st83;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 65 ) {
			if ( 58 <= (*p) && (*p) <= 59 )
				goto st83;
		} else if ( (*p) > 90 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st83;
			} else if ( (*p) >= 97 )
				goto st89;
		} else
			goto st89;
	} else
		goto st89;
	goto tr120;
st102:
	if ( ++p == pe )
		goto _test_eof102;
case 102:
	switch( (*p) ) {
		case 33: goto st83;
		case 47: goto st83;
		case 63: goto st83;
		case 64: goto st90;
		case 80: goto st103;
		case 92: goto st83;
		case 94: goto st83;
		case 95: goto st89;
		case 112: goto st103;
		case 123: goto st83;
	}
	if ( (*p) < 45 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st83;
		} else if ( (*p) > 41 ) {
			if ( 43 <= (*p) && (*p) <= 44 )
				goto st83;
		} else
			goto st83;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 65 ) {
			if ( 58 <= (*p) && (*p) <= 59 )
				goto st83;
		} else if ( (*p) > 90 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st83;
			} else if ( (*p) >= 97 )
				goto st89;
		} else
			goto st89;
	} else
		goto st89;
	goto tr120;
st103:
	if ( ++p == pe )
		goto _test_eof103;
case 103:
	switch( (*p) ) {
		case 33: goto st83;
		case 47: goto st83;
		case 58: goto st104;
		case 59: goto st83;
		case 63: goto st83;
		case 64: goto st90;
		case 92: goto st83;
		case 94: goto st83;
		case 95: goto st89;
		case 123: goto st83;
	}
	if ( (*p) < 45 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st83;
		} else if ( (*p) > 41 ) {
			if ( 43 <= (*p) && (*p) <= 44 )
				goto st83;
		} else
			goto st83;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 97 ) {
			if ( 65 <= (*p) && (*p) <= 90 )
				goto st89;
		} else if ( (*p) > 122 ) {
			if ( 125 <= (*p) && (*p) <= 126 )
				goto st83;
		} else
			goto st89;
	} else
		goto st89;
	goto tr120;
st104:
	if ( ++p == pe )
		goto _test_eof104;
case 104:
	switch( (*p) ) {
		case 33: goto st83;
		case 47: goto st105;
		case 92: goto st83;
	}
	if ( (*p) < 63 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st83;
		} else if ( (*p) > 41 ) {
			if ( 43 <= (*p) && (*p) <= 59 )
				goto st83;
		} else
			goto st83;
	} else if ( (*p) > 90 ) {
		if ( (*p) < 97 ) {
			if ( 94 <= (*p) && (*p) <= 95 )
				goto st83;
		} else if ( (*p) > 123 ) {
			if ( 125 <= (*p) && (*p) <= 126 )
				goto st83;
		} else
			goto st83;
	} else
		goto st83;
	goto tr120;
st105:
	if ( ++p == pe )
		goto _test_eof105;
case 105:
	switch( (*p) ) {
		case 33: goto st83;
		case 47: goto st106;
		case 92: goto st83;
	}
	if ( (*p) < 63 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st83;
		} else if ( (*p) > 41 ) {
			if ( 43 <= (*p) && (*p) <= 59 )
				goto st83;
		} else
			goto st83;
	} else if ( (*p) > 90 ) {
		if ( (*p) < 97 ) {
			if ( 94 <= (*p) && (*p) <= 95 )
				goto st83;
		} else if ( (*p) > 123 ) {
			if ( 125 <= (*p) && (*p) <= 126 )
				goto st83;
		} else
			goto st83;
	} else
		goto st83;
	goto tr120;
st106:
	if ( ++p == pe )
		goto _test_eof106;
case 106:
	switch( (*p) ) {
		case 33: goto st83;
		case 35: goto tr90;
		case 40: goto tr157;
		case 41: goto st83;
		case 44: goto st83;
		case 46: goto st83;
		case 61: goto tr90;
		case 63: goto st83;
		case 92: goto st83;
		case 94: goto st83;
		case 95: goto tr157;
		case 123: goto st83;
		case 125: goto st83;
		case 126: goto tr157;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 37 ) {
			if ( 38 <= (*p) && (*p) <= 42 )
				goto tr90;
		} else if ( (*p) >= 36 )
			goto tr157;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 64 ) {
			if ( 58 <= (*p) && (*p) <= 59 )
				goto st83;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr157;
		} else
			goto tr157;
	} else
		goto tr157;
	goto tr120;
tr90:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
	goto st107;
st107:
	if ( ++p == pe )
		goto _test_eof107;
case 107:
#line 2126 "wikitext_ragel.c"
	switch( (*p) ) {
		case 33: goto st79;
		case 41: goto st79;
		case 44: goto st79;
		case 46: goto st79;
		case 61: goto tr90;
		case 63: goto st79;
		case 95: goto tr90;
		case 126: goto tr90;
	}
	if ( (*p) < 58 ) {
		if ( 35 <= (*p) && (*p) <= 57 )
			goto tr90;
	} else if ( (*p) > 59 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr90;
		} else if ( (*p) >= 64 )
			goto tr90;
	} else
		goto st79;
	goto tr158;
st79:
	if ( ++p == pe )
		goto _test_eof79;
case 79:
	switch( (*p) ) {
		case 33: goto st79;
		case 41: goto st79;
		case 44: goto st79;
		case 46: goto st79;
		case 61: goto tr90;
		case 63: goto st79;
		case 95: goto tr90;
		case 126: goto tr90;
	}
	if ( (*p) < 58 ) {
		if ( 35 <= (*p) && (*p) <= 57 )
			goto tr90;
	} else if ( (*p) > 59 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr90;
		} else if ( (*p) >= 64 )
			goto tr90;
	} else
		goto st79;
	goto tr88;
tr159:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
#line 389 "wikitext_ragel.rl"
	{act = 37;}
	goto st108;
tr157:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
#line 281 "wikitext_ragel.rl"
	{act = 20;}
	goto st108;
st108:
	if ( ++p == pe )
		goto _test_eof108;
case 108:
#line 2191 "wikitext_ragel.c"
	switch( (*p) ) {
		case 33: goto tr159;
		case 35: goto tr90;
		case 40: goto tr157;
		case 41: goto tr159;
		case 44: goto tr159;
		case 46: goto tr159;
		case 61: goto tr90;
		case 63: goto tr159;
		case 92: goto st83;
		case 94: goto st83;
		case 95: goto tr157;
		case 123: goto st83;
		case 125: goto st83;
		case 126: goto tr157;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 37 ) {
			if ( 38 <= (*p) && (*p) <= 42 )
				goto tr90;
		} else if ( (*p) >= 36 )
			goto tr157;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 64 ) {
			if ( 58 <= (*p) && (*p) <= 59 )
				goto tr159;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr157;
		} else
			goto tr157;
	} else
		goto tr157;
	goto tr132;
st109:
	if ( ++p == pe )
		goto _test_eof109;
case 109:
	switch( (*p) ) {
		case 33: goto st83;
		case 47: goto st83;
		case 63: goto st83;
		case 64: goto st90;
		case 84: goto st110;
		case 92: goto st83;
		case 94: goto st83;
		case 95: goto st89;
		case 116: goto st110;
		case 123: goto st83;
	}
	if ( (*p) < 45 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st83;
		} else if ( (*p) > 41 ) {
			if ( 43 <= (*p) && (*p) <= 44 )
				goto st83;
		} else
			goto st83;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 65 ) {
			if ( 58 <= (*p) && (*p) <= 59 )
				goto st83;
		} else if ( (*p) > 90 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st83;
			} else if ( (*p) >= 97 )
				goto st89;
		} else
			goto st89;
	} else
		goto st89;
	goto tr120;
st110:
	if ( ++p == pe )
		goto _test_eof110;
case 110:
	switch( (*p) ) {
		case 33: goto st83;
		case 47: goto st83;
		case 63: goto st83;
		case 64: goto st90;
		case 84: goto st111;
		case 92: goto st83;
		case 94: goto st83;
		case 95: goto st89;
		case 116: goto st111;
		case 123: goto st83;
	}
	if ( (*p) < 45 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st83;
		} else if ( (*p) > 41 ) {
			if ( 43 <= (*p) && (*p) <= 44 )
				goto st83;
		} else
			goto st83;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 65 ) {
			if ( 58 <= (*p) && (*p) <= 59 )
				goto st83;
		} else if ( (*p) > 90 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st83;
			} else if ( (*p) >= 97 )
				goto st89;
		} else
			goto st89;
	} else
		goto st89;
	goto tr120;
st111:
	if ( ++p == pe )
		goto _test_eof111;
case 111:
	switch( (*p) ) {
		case 33: goto st83;
		case 47: goto st83;
		case 63: goto st83;
		case 64: goto st90;
		case 80: goto st112;
		case 92: goto st83;
		case 94: goto st83;
		case 95: goto st89;
		case 112: goto st112;
		case 123: goto st83;
	}
	if ( (*p) < 45 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st83;
		} else if ( (*p) > 41 ) {
			if ( 43 <= (*p) && (*p) <= 44 )
				goto st83;
		} else
			goto st83;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 65 ) {
			if ( 58 <= (*p) && (*p) <= 59 )
				goto st83;
		} else if ( (*p) > 90 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st83;
			} else if ( (*p) >= 97 )
				goto st89;
		} else
			goto st89;
	} else
		goto st89;
	goto tr120;
st112:
	if ( ++p == pe )
		goto _test_eof112;
case 112:
	switch( (*p) ) {
		case 33: goto st83;
		case 47: goto st83;
		case 58: goto st104;
		case 59: goto st83;
		case 63: goto st83;
		case 64: goto st90;
		case 83: goto st103;
		case 92: goto st83;
		case 94: goto st83;
		case 95: goto st89;
		case 115: goto st103;
		case 123: goto st83;
	}
	if ( (*p) < 45 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st83;
		} else if ( (*p) > 41 ) {
			if ( 43 <= (*p) && (*p) <= 44 )
				goto st83;
		} else
			goto st83;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 97 ) {
			if ( 65 <= (*p) && (*p) <= 90 )
				goto st89;
		} else if ( (*p) > 122 ) {
			if ( 125 <= (*p) && (*p) <= 126 )
				goto st83;
		} else
			goto st89;
	} else
		goto st89;
	goto tr120;
st113:
	if ( ++p == pe )
		goto _test_eof113;
case 113:
	switch( (*p) ) {
		case 33: goto st83;
		case 47: goto st83;
		case 63: goto st83;
		case 64: goto st90;
		case 65: goto st114;
		case 92: goto st83;
		case 94: goto st83;
		case 95: goto st89;
		case 97: goto st114;
		case 123: goto st83;
	}
	if ( (*p) < 45 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st83;
		} else if ( (*p) > 41 ) {
			if ( 43 <= (*p) && (*p) <= 44 )
				goto st83;
		} else
			goto st83;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 66 ) {
			if ( 58 <= (*p) && (*p) <= 59 )
				goto st83;
		} else if ( (*p) > 90 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st83;
			} else if ( (*p) >= 98 )
				goto st89;
		} else
			goto st89;
	} else
		goto st89;
	goto tr120;
st114:
	if ( ++p == pe )
		goto _test_eof114;
case 114:
	switch( (*p) ) {
		case 33: goto st83;
		case 47: goto st83;
		case 63: goto st83;
		case 64: goto st90;
		case 73: goto st115;
		case 92: goto st83;
		case 94: goto st83;
		case 95: goto st89;
		case 105: goto st115;
		case 123: goto st83;
	}
	if ( (*p) < 45 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st83;
		} else if ( (*p) > 41 ) {
			if ( 43 <= (*p) && (*p) <= 44 )
				goto st83;
		} else
			goto st83;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 65 ) {
			if ( 58 <= (*p) && (*p) <= 59 )
				goto st83;
		} else if ( (*p) > 90 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st83;
			} else if ( (*p) >= 97 )
				goto st89;
		} else
			goto st89;
	} else
		goto st89;
	goto tr120;
st115:
	if ( ++p == pe )
		goto _test_eof115;
case 115:
	switch( (*p) ) {
		case 33: goto st83;
		case 47: goto st83;
		case 63: goto st83;
		case 64: goto st90;
		case 76: goto st116;
		case 92: goto st83;
		case 94: goto st83;
		case 95: goto st89;
		case 108: goto st116;
		case 123: goto st83;
	}
	if ( (*p) < 45 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st83;
		} else if ( (*p) > 41 ) {
			if ( 43 <= (*p) && (*p) <= 44 )
				goto st83;
		} else
			goto st83;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 65 ) {
			if ( 58 <= (*p) && (*p) <= 59 )
				goto st83;
		} else if ( (*p) > 90 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st83;
			} else if ( (*p) >= 97 )
				goto st89;
		} else
			goto st89;
	} else
		goto st89;
	goto tr120;
st116:
	if ( ++p == pe )
		goto _test_eof116;
case 116:
	switch( (*p) ) {
		case 33: goto st83;
		case 47: goto st83;
		case 63: goto st83;
		case 64: goto st90;
		case 84: goto st117;
		case 92: goto st83;
		case 94: goto st83;
		case 95: goto st89;
		case 116: goto st117;
		case 123: goto st83;
	}
	if ( (*p) < 45 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st83;
		} else if ( (*p) > 41 ) {
			if ( 43 <= (*p) && (*p) <= 44 )
				goto st83;
		} else
			goto st83;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 65 ) {
			if ( 58 <= (*p) && (*p) <= 59 )
				goto st83;
		} else if ( (*p) > 90 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st83;
			} else if ( (*p) >= 97 )
				goto st89;
		} else
			goto st89;
	} else
		goto st89;
	goto tr120;
st117:
	if ( ++p == pe )
		goto _test_eof117;
case 117:
	switch( (*p) ) {
		case 33: goto st83;
		case 47: goto st83;
		case 63: goto st83;
		case 64: goto st90;
		case 79: goto st118;
		case 92: goto st83;
		case 94: goto st83;
		case 95: goto st89;
		case 111: goto st118;
		case 123: goto st83;
	}
	if ( (*p) < 45 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st83;
		} else if ( (*p) > 41 ) {
			if ( 43 <= (*p) && (*p) <= 44 )
				goto st83;
		} else
			goto st83;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 65 ) {
			if ( 58 <= (*p) && (*p) <= 59 )
				goto st83;
		} else if ( (*p) > 90 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st83;
			} else if ( (*p) >= 97 )
				goto st89;
		} else
			goto st89;
	} else
		goto st89;
	goto tr120;
st118:
	if ( ++p == pe )
		goto _test_eof118;
case 118:
	switch( (*p) ) {
		case 33: goto st83;
		case 47: goto st83;
		case 58: goto st119;
		case 59: goto st83;
		case 63: goto st83;
		case 64: goto st90;
		case 92: goto st83;
		case 94: goto st83;
		case 95: goto st89;
		case 123: goto st83;
	}
	if ( (*p) < 45 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st83;
		} else if ( (*p) > 41 ) {
			if ( 43 <= (*p) && (*p) <= 44 )
				goto st83;
		} else
			goto st83;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 97 ) {
			if ( 65 <= (*p) && (*p) <= 90 )
				goto st89;
		} else if ( (*p) > 122 ) {
			if ( 125 <= (*p) && (*p) <= 126 )
				goto st83;
		} else
			goto st89;
	} else
		goto st89;
	goto tr120;
st119:
	if ( ++p == pe )
		goto _test_eof119;
case 119:
	switch( (*p) ) {
		case 33: goto st83;
		case 47: goto st83;
		case 92: goto st83;
		case 94: goto st83;
		case 95: goto st120;
		case 123: goto st83;
	}
	if ( (*p) < 58 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st83;
		} else if ( (*p) > 41 ) {
			if ( (*p) > 44 ) {
				if ( 45 <= (*p) && (*p) <= 57 )
					goto st120;
			} else if ( (*p) >= 43 )
				goto st83;
		} else
			goto st83;
	} else if ( (*p) > 59 ) {
		if ( (*p) < 65 ) {
			if ( 63 <= (*p) && (*p) <= 64 )
				goto st83;
		} else if ( (*p) > 90 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st83;
			} else if ( (*p) >= 97 )
				goto st120;
		} else
			goto st120;
	} else
		goto st83;
	goto tr120;
st120:
	if ( ++p == pe )
		goto _test_eof120;
case 120:
	switch( (*p) ) {
		case 33: goto st83;
		case 47: goto st83;
		case 63: goto st83;
		case 64: goto st121;
		case 92: goto st83;
		case 94: goto st83;
		case 95: goto st120;
		case 123: goto st83;
	}
	if ( (*p) < 45 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st83;
		} else if ( (*p) > 41 ) {
			if ( 43 <= (*p) && (*p) <= 44 )
				goto st83;
		} else
			goto st83;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 65 ) {
			if ( 58 <= (*p) && (*p) <= 59 )
				goto st83;
		} else if ( (*p) > 90 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st83;
			} else if ( (*p) >= 97 )
				goto st120;
		} else
			goto st120;
	} else
		goto st120;
	goto tr120;
st121:
	if ( ++p == pe )
		goto _test_eof121;
case 121:
	switch( (*p) ) {
		case 33: goto st83;
		case 92: goto st83;
		case 123: goto st83;
	}
	if ( (*p) < 58 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st83;
		} else if ( (*p) > 41 ) {
			if ( (*p) > 47 ) {
				if ( 48 <= (*p) && (*p) <= 57 )
					goto tr171;
			} else if ( (*p) >= 43 )
				goto st83;
		} else
			goto st83;
	} else if ( (*p) > 59 ) {
		if ( (*p) < 94 ) {
			if ( (*p) > 64 ) {
				if ( 65 <= (*p) && (*p) <= 90 )
					goto tr171;
			} else if ( (*p) >= 63 )
				goto st83;
		} else if ( (*p) > 95 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st83;
			} else if ( (*p) >= 97 )
				goto tr171;
		} else
			goto st83;
	} else
		goto st83;
	goto tr120;
tr171:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
#line 389 "wikitext_ragel.rl"
	{act = 37;}
	goto st122;
tr177:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
#line 281 "wikitext_ragel.rl"
	{act = 20;}
	goto st122;
st122:
	if ( ++p == pe )
		goto _test_eof122;
case 122:
#line 2754 "wikitext_ragel.c"
	switch( (*p) ) {
		case 33: goto st83;
		case 46: goto st123;
		case 92: goto st83;
		case 123: goto st83;
	}
	if ( (*p) < 58 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st83;
		} else if ( (*p) > 41 ) {
			if ( (*p) > 47 ) {
				if ( 48 <= (*p) && (*p) <= 57 )
					goto tr171;
			} else if ( (*p) >= 43 )
				goto st83;
		} else
			goto st83;
	} else if ( (*p) > 59 ) {
		if ( (*p) < 94 ) {
			if ( (*p) > 64 ) {
				if ( 65 <= (*p) && (*p) <= 90 )
					goto tr171;
			} else if ( (*p) >= 63 )
				goto st83;
		} else if ( (*p) > 95 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st83;
			} else if ( (*p) >= 97 )
				goto tr171;
		} else
			goto st83;
	} else
		goto st83;
	goto tr132;
st123:
	if ( ++p == pe )
		goto _test_eof123;
case 123:
	switch( (*p) ) {
		case 33: goto st83;
		case 92: goto st83;
		case 123: goto st83;
	}
	if ( (*p) < 58 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st83;
		} else if ( (*p) > 41 ) {
			if ( (*p) > 47 ) {
				if ( 48 <= (*p) && (*p) <= 57 )
					goto tr171;
			} else if ( (*p) >= 43 )
				goto st83;
		} else
			goto st83;
	} else if ( (*p) > 59 ) {
		if ( (*p) < 94 ) {
			if ( (*p) > 64 ) {
				if ( 65 <= (*p) && (*p) <= 90 )
					goto st124;
			} else if ( (*p) >= 63 )
				goto st83;
		} else if ( (*p) > 95 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st83;
			} else if ( (*p) >= 97 )
				goto st124;
		} else
			goto st83;
	} else
		goto st83;
	goto tr120;
st124:
	if ( ++p == pe )
		goto _test_eof124;
case 124:
	switch( (*p) ) {
		case 33: goto st83;
		case 46: goto st123;
		case 92: goto st83;
		case 123: goto st83;
	}
	if ( (*p) < 58 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st83;
		} else if ( (*p) > 41 ) {
			if ( (*p) > 47 ) {
				if ( 48 <= (*p) && (*p) <= 57 )
					goto tr171;
			} else if ( (*p) >= 43 )
				goto st83;
		} else
			goto st83;
	} else if ( (*p) > 59 ) {
		if ( (*p) < 94 ) {
			if ( (*p) > 64 ) {
				if ( 65 <= (*p) && (*p) <= 90 )
					goto st125;
			} else if ( (*p) >= 63 )
				goto st83;
		} else if ( (*p) > 95 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st83;
			} else if ( (*p) >= 97 )
				goto st125;
		} else
			goto st83;
	} else
		goto st83;
	goto tr120;
st125:
	if ( ++p == pe )
		goto _test_eof125;
case 125:
	switch( (*p) ) {
		case 33: goto st83;
		case 46: goto st123;
		case 92: goto st83;
		case 123: goto st83;
	}
	if ( (*p) < 58 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st83;
		} else if ( (*p) > 41 ) {
			if ( (*p) > 47 ) {
				if ( 48 <= (*p) && (*p) <= 57 )
					goto tr171;
			} else if ( (*p) >= 43 )
				goto st83;
		} else
			goto st83;
	} else if ( (*p) > 59 ) {
		if ( (*p) < 94 ) {
			if ( (*p) > 64 ) {
				if ( 65 <= (*p) && (*p) <= 90 )
					goto st126;
			} else if ( (*p) >= 63 )
				goto st83;
		} else if ( (*p) > 95 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st83;
			} else if ( (*p) >= 97 )
				goto st126;
		} else
			goto st83;
	} else
		goto st83;
	goto tr158;
st126:
	if ( ++p == pe )
		goto _test_eof126;
case 126:
	switch( (*p) ) {
		case 33: goto st83;
		case 46: goto st123;
		case 92: goto st83;
		case 123: goto st83;
	}
	if ( (*p) < 58 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st83;
		} else if ( (*p) > 41 ) {
			if ( (*p) > 47 ) {
				if ( 48 <= (*p) && (*p) <= 57 )
					goto tr171;
			} else if ( (*p) >= 43 )
				goto st83;
		} else
			goto st83;
	} else if ( (*p) > 59 ) {
		if ( (*p) < 94 ) {
			if ( (*p) > 64 ) {
				if ( 65 <= (*p) && (*p) <= 90 )
					goto st127;
			} else if ( (*p) >= 63 )
				goto st83;
		} else if ( (*p) > 95 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st83;
			} else if ( (*p) >= 97 )
				goto st127;
		} else
			goto st83;
	} else
		goto st83;
	goto tr158;
st127:
	if ( ++p == pe )
		goto _test_eof127;
case 127:
	switch( (*p) ) {
		case 33: goto st83;
		case 46: goto st123;
		case 92: goto st83;
		case 123: goto st83;
	}
	if ( (*p) < 58 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st83;
		} else if ( (*p) > 41 ) {
			if ( (*p) > 47 ) {
				if ( 48 <= (*p) && (*p) <= 57 )
					goto tr171;
			} else if ( (*p) >= 43 )
				goto st83;
		} else
			goto st83;
	} else if ( (*p) > 59 ) {
		if ( (*p) < 94 ) {
			if ( (*p) > 64 ) {
				if ( 65 <= (*p) && (*p) <= 90 )
					goto tr177;
			} else if ( (*p) >= 63 )
				goto st83;
		} else if ( (*p) > 95 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st83;
			} else if ( (*p) >= 97 )
				goto tr177;
		} else
			goto st83;
	} else
		goto st83;
	goto tr158;
st128:
	if ( ++p == pe )
		goto _test_eof128;
case 128:
	switch( (*p) ) {
		case 33: goto st83;
		case 47: goto st83;
		case 63: goto st83;
		case 64: goto st90;
		case 86: goto st129;
		case 92: goto st83;
		case 94: goto st83;
		case 95: goto st89;
		case 118: goto st129;
		case 123: goto st83;
	}
	if ( (*p) < 45 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st83;
		} else if ( (*p) > 41 ) {
			if ( 43 <= (*p) && (*p) <= 44 )
				goto st83;
		} else
			goto st83;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 65 ) {
			if ( 58 <= (*p) && (*p) <= 59 )
				goto st83;
		} else if ( (*p) > 90 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st83;
			} else if ( (*p) >= 97 )
				goto st89;
		} else
			goto st89;
	} else
		goto st89;
	goto tr120;
st129:
	if ( ++p == pe )
		goto _test_eof129;
case 129:
	switch( (*p) ) {
		case 33: goto st83;
		case 47: goto st83;
		case 63: goto st83;
		case 64: goto st90;
		case 78: goto st103;
		case 92: goto st83;
		case 94: goto st83;
		case 95: goto st89;
		case 110: goto st103;
		case 123: goto st83;
	}
	if ( (*p) < 45 ) {
		if ( (*p) < 40 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto st83;
		} else if ( (*p) > 41 ) {
			if ( 43 <= (*p) && (*p) <= 44 )
				goto st83;
		} else
			goto st83;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 65 ) {
			if ( 58 <= (*p) && (*p) <= 59 )
				goto st83;
		} else if ( (*p) > 90 ) {
			if ( (*p) > 122 ) {
				if ( 125 <= (*p) && (*p) <= 126 )
					goto st83;
			} else if ( (*p) >= 97 )
				goto st89;
		} else
			goto st89;
	} else
		goto st89;
	goto tr120;
st130:
	if ( ++p == pe )
		goto _test_eof130;
case 130:
	if ( (*p) == 91 )
		goto tr180;
	goto tr179;
st131:
	if ( ++p == pe )
		goto _test_eof131;
case 131:
	if ( (*p) == 93 )
		goto tr182;
	goto tr181;
	}
	_test_eof80: cs = 80; goto _test_eof; 
	_test_eof1: cs = 1; goto _test_eof; 
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof81: cs = 81; goto _test_eof; 
	_test_eof82: cs = 82; goto _test_eof; 
	_test_eof83: cs = 83; goto _test_eof; 
	_test_eof84: cs = 84; goto _test_eof; 
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
	_test_eof85: cs = 85; goto _test_eof; 
	_test_eof86: cs = 86; goto _test_eof; 
	_test_eof87: cs = 87; goto _test_eof; 
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
	_test_eof72: cs = 72; goto _test_eof; 
	_test_eof73: cs = 73; goto _test_eof; 
	_test_eof74: cs = 74; goto _test_eof; 
	_test_eof75: cs = 75; goto _test_eof; 
	_test_eof76: cs = 76; goto _test_eof; 
	_test_eof77: cs = 77; goto _test_eof; 
	_test_eof78: cs = 78; goto _test_eof; 
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
	_test_eof79: cs = 79; goto _test_eof; 
	_test_eof108: cs = 108; goto _test_eof; 
	_test_eof109: cs = 109; goto _test_eof; 
	_test_eof110: cs = 110; goto _test_eof; 
	_test_eof111: cs = 111; goto _test_eof; 
	_test_eof112: cs = 112; goto _test_eof; 
	_test_eof113: cs = 113; goto _test_eof; 
	_test_eof114: cs = 114; goto _test_eof; 
	_test_eof115: cs = 115; goto _test_eof; 
	_test_eof116: cs = 116; goto _test_eof; 
	_test_eof117: cs = 117; goto _test_eof; 
	_test_eof118: cs = 118; goto _test_eof; 
	_test_eof119: cs = 119; goto _test_eof; 
	_test_eof120: cs = 120; goto _test_eof; 
	_test_eof121: cs = 121; goto _test_eof; 
	_test_eof122: cs = 122; goto _test_eof; 
	_test_eof123: cs = 123; goto _test_eof; 
	_test_eof124: cs = 124; goto _test_eof; 
	_test_eof125: cs = 125; goto _test_eof; 
	_test_eof126: cs = 126; goto _test_eof; 
	_test_eof127: cs = 127; goto _test_eof; 
	_test_eof128: cs = 128; goto _test_eof; 
	_test_eof129: cs = 129; goto _test_eof; 
	_test_eof130: cs = 130; goto _test_eof; 
	_test_eof131: cs = 131; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 81: goto tr116;
	case 82: goto tr118;
	case 83: goto tr120;
	case 84: goto tr121;
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
	case 85: goto tr125;
	case 86: goto tr125;
	case 87: goto tr125;
	case 88: goto tr125;
	case 89: goto tr120;
	case 90: goto tr120;
	case 91: goto tr132;
	case 92: goto tr120;
	case 93: goto tr120;
	case 94: goto tr136;
	case 95: goto tr136;
	case 96: goto tr136;
	case 97: goto tr140;
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
	case 59: goto tr23;
	case 60: goto tr23;
	case 61: goto tr23;
	case 62: goto tr23;
	case 63: goto tr23;
	case 64: goto tr23;
	case 65: goto tr23;
	case 66: goto tr23;
	case 67: goto tr23;
	case 68: goto tr23;
	case 69: goto tr23;
	case 70: goto tr23;
	case 71: goto tr23;
	case 72: goto tr23;
	case 73: goto tr23;
	case 74: goto tr23;
	case 75: goto tr23;
	case 76: goto tr23;
	case 77: goto tr23;
	case 78: goto tr23;
	case 98: goto tr148;
	case 99: goto tr148;
	case 100: goto tr150;
	case 101: goto tr120;
	case 102: goto tr120;
	case 103: goto tr120;
	case 104: goto tr120;
	case 105: goto tr120;
	case 106: goto tr120;
	case 107: goto tr158;
	case 79: goto tr88;
	case 108: goto tr132;
	case 109: goto tr120;
	case 110: goto tr120;
	case 111: goto tr120;
	case 112: goto tr120;
	case 113: goto tr120;
	case 114: goto tr120;
	case 115: goto tr120;
	case 116: goto tr120;
	case 117: goto tr120;
	case 118: goto tr120;
	case 119: goto tr120;
	case 120: goto tr120;
	case 121: goto tr120;
	case 122: goto tr132;
	case 123: goto tr120;
	case 124: goto tr120;
	case 125: goto tr158;
	case 126: goto tr158;
	case 127: goto tr158;
	case 128: goto tr120;
	case 129: goto tr120;
	case 130: goto tr179;
	case 131: goto tr181;
	}
	}

	_out: {}
	}
#line 461 "wikitext_ragel.rl"
    if (cs == wikitext_error)
        rb_raise(eWikitextParserError, "failed before finding a token");
    else if (out->type == NO_TOKEN)
        rb_raise(eWikitextParserError, "failed to produce a token");
}
