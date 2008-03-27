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
static const int wikitext_start = 94;
static const int wikitext_first_final = 94;
static const int wikitext_error = 0;

static const int wikitext_en_main = 94;

#line 455 "wikitext_ragel.rl"


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
#line 497 "wikitext_ragel.rl"
    
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
#line 446 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(DEFAULT);
            out->column_stop = out->column_start + 1;
            {p++; cs = 94; goto _out;}
        }}
	goto st94;
tr3:
#line 52 "wikitext_ragel.rl"
	{
        out->code_point = ((uint32_t)(*(p - 2)) & 0x0f) << 12 |
            ((uint32_t)(*(p - 1)) & 0x3f) << 6 |
            (*p & 0x3f);
    }
#line 446 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(DEFAULT);
            out->column_stop = out->column_start + 1;
            {p++; cs = 94; goto _out;}
        }}
	goto st94;
tr6:
#line 59 "wikitext_ragel.rl"
	{
        out->code_point = ((uint32_t)(*(p - 3)) & 0x07) << 18 |
            ((uint32_t)(*(p - 2)) & 0x3f) << 12 |
            ((uint32_t)(*(p - 1)) & 0x3f) << 6 |
            (*p & 0x3f);
    }
#line 446 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(DEFAULT);
            out->column_stop = out->column_start + 1;
            {p++; cs = 94; goto _out;}
        }}
	goto st94;
tr7:
#line 359 "wikitext_ragel.rl"
	{{p = ((te))-1;}{
            EMIT(AMP);
            {p++; cs = 94; goto _out;}
        }}
	goto st94;
tr10:
#line 347 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(DECIMAL_ENTITY);
            {p++; cs = 94; goto _out;}
        }}
	goto st94;
tr12:
#line 341 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(HEX_ENTITY);
            {p++; cs = 94; goto _out;}
        }}
	goto st94;
tr14:
#line 335 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(NAMED_ENTITY);
            {p++; cs = 94; goto _out;}
        }}
	goto st94;
tr18:
#line 329 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(AMP_ENTITY);
            {p++; cs = 94; goto _out;}
        }}
	goto st94;
tr22:
#line 323 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(QUOT_ENTITY);
            {p++; cs = 94; goto _out;}
        }}
	goto st94;
tr23:
#line 1 "wikitext_ragel.rl"
	{	switch( act ) {
	case 20:
	{{p = ((te))-1;}
            EMIT(URI);
            {p++; cs = 94; goto _out;}
        }
	break;
	case 21:
	{{p = ((te))-1;}
            EMIT(MAIL);
            {p++; cs = 94; goto _out;}
        }
	break;
	case 41:
	{{p = ((te))-1;}
            EMIT(SPECIAL_URI_CHARS);
            {p++; cs = 94; goto _out;}
        }
	break;
	case 42:
	{{p = ((te))-1;}
            EMIT(PRINTABLE);
            {p++; cs = 94; goto _out;}
        }
	break;
	default: break;
	}
	}
	goto st94;
tr30:
#line 365 "wikitext_ragel.rl"
	{{p = ((te))-1;}{
            EMIT(LESS);
            {p++; cs = 94; goto _out;}
        }}
	goto st94;
tr46:
#line 110 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(BLOCKQUOTE_END);
            {p++; cs = 94; goto _out;}
        }}
	goto st94;
tr48:
#line 152 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(EM_END);
            {p++; cs = 94; goto _out;}
        }}
	goto st94;
tr54:
#line 86 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(NO_WIKI_END);
            {p++; cs = 94; goto _out;}
        }}
	goto st94;
tr57:
#line 98 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(PRE_END);
            {p++; cs = 94; goto _out;}
        }}
	goto st94;
tr63:
#line 140 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(STRONG_END);
            {p++; cs = 94; goto _out;}
        }}
	goto st94;
tr65:
#line 170 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(TT_END);
            {p++; cs = 94; goto _out;}
        }}
	goto st94;
tr75:
#line 104 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(BLOCKQUOTE_START);
            {p++; cs = 94; goto _out;}
        }}
	goto st94;
tr77:
#line 146 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(EM_START);
            {p++; cs = 94; goto _out;}
        }}
	goto st94;
tr83:
#line 80 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(NO_WIKI_START);
            {p++; cs = 94; goto _out;}
        }}
	goto st94;
tr86:
#line 92 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(PRE_START);
            {p++; cs = 94; goto _out;}
        }}
	goto st94;
tr92:
#line 134 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(STRONG_START);
            {p++; cs = 94; goto _out;}
        }}
	goto st94;
tr94:
#line 164 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(TT_START);
            {p++; cs = 94; goto _out;}
        }}
	goto st94;
tr95:
#line 426 "wikitext_ragel.rl"
	{{p = ((te))-1;}{
            EMIT(PRINTABLE);
            {p++; cs = 94; goto _out;}
        }}
	goto st94;
tr99:
#line 281 "wikitext_ragel.rl"
	{{p = ((te))-1;}{
            EMIT(URI);
            {p++; cs = 94; goto _out;}
        }}
	goto st94;
tr110:
#line 41 "wikitext_ragel.rl"
	{
        out->code_point = *p & 0x7f;
    }
#line 446 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(DEFAULT);
            out->column_stop = out->column_start + 1;
            {p++; cs = 94; goto _out;}
        }}
	goto st94;
tr111:
#line 401 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(CRLF);
            out->column_stop = 1;
            out->line_stop++;
            {p++; cs = 94; goto _out;}
        }}
#line 41 "wikitext_ragel.rl"
	{
        out->code_point = *p & 0x7f;
    }
	goto st94;
tr115:
#line 353 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(QUOT);
            {p++; cs = 94; goto _out;}
        }}
	goto st94;
tr116:
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
            {p++; cs = 94; goto _out;}
        }}
	goto st94;
tr120:
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
            {p++; cs = 94; goto _out;}
        }}
	goto st94;
tr132:
#line 158 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(TT);
            {p++; cs = 94; goto _out;}
        }}
	goto st94;
tr134:
#line 305 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(SEPARATOR);
            {p++; cs = 94; goto _out;}
        }}
	goto st94;
tr136:
#line 401 "wikitext_ragel.rl"
	{te = p;p--;{
            EMIT(CRLF);
            out->column_stop = 1;
            out->line_stop++;
            {p++; cs = 94; goto _out;}
        }}
	goto st94;
tr137:
#line 401 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(CRLF);
            out->column_stop = 1;
            out->line_stop++;
            {p++; cs = 94; goto _out;}
        }}
	goto st94;
tr138:
#line 190 "wikitext_ragel.rl"
	{te = p;p--;{
            if (out->column_start == 1 || last_token_type == BLOCKQUOTE)
            {
                REWIND();
                EMIT(PRE);
            }
            else
                EMIT(SPACE);
            {p++; cs = 94; goto _out;}
        }}
	goto st94;
tr140:
#line 414 "wikitext_ragel.rl"
	{te = p;p--;{
            EMIT(SPECIAL_URI_CHARS);
            {p++; cs = 94; goto _out;}
        }}
	goto st94;
tr141:
#line 426 "wikitext_ragel.rl"
	{te = p;p--;{
            EMIT(PRINTABLE);
            {p++; cs = 94; goto _out;}
        }}
	goto st94;
tr142:
#line 359 "wikitext_ragel.rl"
	{te = p;p--;{
            EMIT(AMP);
            {p++; cs = 94; goto _out;}
        }}
	goto st94;
tr146:
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
            {p++; cs = 94; goto _out;}
        }}
	goto st94;
tr150:
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
            {p++; cs = 94; goto _out;}
        }}
	goto st94;
tr152:
#line 287 "wikitext_ragel.rl"
	{te = p;p--;{
            EMIT(MAIL);
            {p++; cs = 94; goto _out;}
        }}
	goto st94;
tr157:
#line 365 "wikitext_ragel.rl"
	{te = p;p--;{
            EMIT(LESS);
            {p++; cs = 94; goto _out;}
        }}
	goto st94;
tr165:
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
            {p++; cs = 94; goto _out;}
        }}
	goto st94;
tr167:
#line 177 "wikitext_ragel.rl"
	{te = p;p--;{
            if (out->column_start == 1 || last_token_type == BLOCKQUOTE)
                EMIT(BLOCKQUOTE);
            else
            {
                REWIND();
                EMIT(GREATER);
            }
            {p++; cs = 94; goto _out;}
        }}
	goto st94;
tr168:
#line 177 "wikitext_ragel.rl"
	{te = p+1;{
            if (out->column_start == 1 || last_token_type == BLOCKQUOTE)
                EMIT(BLOCKQUOTE);
            else
            {
                REWIND();
                EMIT(GREATER);
            }
            {p++; cs = 94; goto _out;}
        }}
	goto st94;
tr172:
#line 281 "wikitext_ragel.rl"
	{te = p;p--;{
            EMIT(URI);
            {p++; cs = 94; goto _out;}
        }}
	goto st94;
tr186:
#line 311 "wikitext_ragel.rl"
	{te = p;p--;{
            EMIT(EXT_LINK_START);
            {p++; cs = 94; goto _out;}
        }}
	goto st94;
tr187:
#line 293 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(LINK_START);
            {p++; cs = 94; goto _out;}
        }}
	goto st94;
tr188:
#line 317 "wikitext_ragel.rl"
	{te = p;p--;{
            EMIT(EXT_LINK_END);
            {p++; cs = 94; goto _out;}
        }}
	goto st94;
tr189:
#line 299 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(LINK_END);
            {p++; cs = 94; goto _out;}
        }}
	goto st94;
tr190:
#line 389 "wikitext_ragel.rl"
	{te = p;p--;{
            EMIT(LEFT_CURLY);
            {p++; cs = 94; goto _out;}
        }}
	goto st94;
tr191:
#line 377 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(IMG_START);
            {p++; cs = 94; goto _out;}
        }}
	goto st94;
tr192:
#line 395 "wikitext_ragel.rl"
	{te = p;p--;{
            EMIT(RIGHT_CURLY);
            {p++; cs = 94; goto _out;}
        }}
	goto st94;
tr193:
#line 383 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(IMG_END);
            {p++; cs = 94; goto _out;}
        }}
	goto st94;
st94:
#line 1 "wikitext_ragel.rl"
	{ts = 0;}
	if ( ++p == pe )
		goto _test_eof94;
case 94:
#line 1 "wikitext_ragel.rl"
	{ts = p;}
#line 649 "wikitext_ragel.c"
	switch( (*p) ) {
		case 10: goto tr111;
		case 13: goto tr112;
		case 32: goto tr113;
		case 33: goto st97;
		case 34: goto tr115;
		case 35: goto tr116;
		case 38: goto tr118;
		case 39: goto st100;
		case 42: goto tr120;
		case 43: goto st98;
		case 46: goto tr122;
		case 47: goto st98;
		case 60: goto tr123;
		case 61: goto tr124;
		case 62: goto tr125;
		case 64: goto st98;
		case 70: goto tr126;
		case 72: goto tr127;
		case 77: goto tr128;
		case 83: goto tr129;
		case 91: goto st136;
		case 92: goto st98;
		case 93: goto st137;
		case 94: goto st98;
		case 96: goto tr132;
		case 102: goto tr126;
		case 104: goto tr127;
		case 109: goto tr128;
		case 115: goto tr129;
		case 123: goto st138;
		case 124: goto tr134;
		case 125: goto st139;
		case 126: goto st98;
		case 127: goto tr110;
	}
	if ( (*p) < 36 ) {
		if ( (*p) < -32 ) {
			if ( -62 <= (*p) && (*p) <= -33 )
				goto st1;
		} else if ( (*p) > -17 ) {
			if ( (*p) > -12 ) {
				if ( 1 <= (*p) && (*p) <= 31 )
					goto tr110;
			} else if ( (*p) >= -16 )
				goto st4;
		} else
			goto st2;
	} else if ( (*p) > 37 ) {
		if ( (*p) < 45 ) {
			if ( 40 <= (*p) && (*p) <= 44 )
				goto st97;
		} else if ( (*p) > 57 ) {
			if ( (*p) > 63 ) {
				if ( 65 <= (*p) && (*p) <= 122 )
					goto tr121;
			} else if ( (*p) >= 58 )
				goto st97;
		} else
			goto tr121;
	} else
		goto st98;
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
tr112:
#line 41 "wikitext_ragel.rl"
	{
        out->code_point = *p & 0x7f;
    }
	goto st95;
st95:
	if ( ++p == pe )
		goto _test_eof95;
case 95:
#line 768 "wikitext_ragel.c"
	if ( (*p) == 10 )
		goto tr137;
	goto tr136;
tr113:
#line 36 "wikitext_ragel.rl"
	{
        MARK();
    }
	goto st96;
st96:
	if ( ++p == pe )
		goto _test_eof96;
case 96:
#line 782 "wikitext_ragel.c"
	if ( (*p) == 32 )
		goto st96;
	goto tr138;
st97:
	if ( ++p == pe )
		goto _test_eof97;
case 97:
	switch( (*p) ) {
		case 33: goto st97;
		case 44: goto st97;
		case 46: goto st97;
		case 63: goto st97;
	}
	if ( (*p) > 41 ) {
		if ( 58 <= (*p) && (*p) <= 59 )
			goto st97;
	} else if ( (*p) >= 40 )
		goto st97;
	goto tr140;
st98:
	if ( ++p == pe )
		goto _test_eof98;
case 98:
	switch( (*p) ) {
		case 43: goto st98;
		case 45: goto st98;
		case 92: goto st98;
		case 126: goto st98;
	}
	if ( (*p) < 64 ) {
		if ( (*p) > 37 ) {
			if ( 47 <= (*p) && (*p) <= 57 )
				goto st98;
		} else if ( (*p) >= 36 )
			goto st98;
	} else if ( (*p) > 90 ) {
		if ( (*p) > 95 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st98;
		} else if ( (*p) >= 94 )
			goto st98;
	} else
		goto st98;
	goto tr141;
tr118:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
	goto st99;
st99:
	if ( ++p == pe )
		goto _test_eof99;
case 99:
#line 835 "wikitext_ragel.c"
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
	goto tr142;
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
st100:
	if ( ++p == pe )
		goto _test_eof100;
case 100:
	if ( (*p) == 39 )
		goto st101;
	goto tr146;
st101:
	if ( ++p == pe )
		goto _test_eof101;
case 101:
	if ( (*p) == 39 )
		goto st102;
	goto tr146;
st102:
	if ( ++p == pe )
		goto _test_eof102;
case 102:
	if ( (*p) == 39 )
		goto st103;
	goto tr146;
st103:
	if ( ++p == pe )
		goto _test_eof103;
case 103:
	if ( (*p) == 39 )
		goto tr150;
	goto tr146;
tr121:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
#line 426 "wikitext_ragel.rl"
	{act = 42;}
	goto st104;
st104:
	if ( ++p == pe )
		goto _test_eof104;
case 104:
#line 1072 "wikitext_ragel.c"
	switch( (*p) ) {
		case 43: goto st98;
		case 46: goto st20;
		case 47: goto st98;
		case 64: goto st109;
		case 92: goto st98;
		case 94: goto st98;
		case 95: goto tr121;
		case 126: goto st98;
	}
	if ( (*p) < 45 ) {
		if ( 36 <= (*p) && (*p) <= 37 )
			goto st98;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr121;
		} else if ( (*p) >= 65 )
			goto tr121;
	} else
		goto tr121;
	goto tr141;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
	switch( (*p) ) {
		case 64: goto st21;
		case 95: goto st20;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st20;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st20;
		} else if ( (*p) >= 65 )
			goto st20;
	} else
		goto st20;
	goto tr23;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st22;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st22;
	} else
		goto st22;
	goto tr23;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
	if ( (*p) == 46 )
		goto st23;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st22;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st22;
	} else
		goto st22;
	goto tr23;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st22;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st24;
	} else
		goto st24;
	goto tr23;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
	if ( (*p) == 46 )
		goto st23;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st22;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr29;
	} else
		goto tr29;
	goto tr23;
tr29:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
#line 287 "wikitext_ragel.rl"
	{act = 21;}
	goto st105;
st105:
	if ( ++p == pe )
		goto _test_eof105;
case 105:
#line 1181 "wikitext_ragel.c"
	if ( (*p) == 46 )
		goto st23;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st22;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr153;
	} else
		goto tr153;
	goto tr152;
tr153:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
#line 287 "wikitext_ragel.rl"
	{act = 21;}
	goto st106;
st106:
	if ( ++p == pe )
		goto _test_eof106;
case 106:
#line 1203 "wikitext_ragel.c"
	if ( (*p) == 46 )
		goto st23;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st22;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr154;
	} else
		goto tr154;
	goto tr152;
tr154:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
#line 287 "wikitext_ragel.rl"
	{act = 21;}
	goto st107;
st107:
	if ( ++p == pe )
		goto _test_eof107;
case 107:
#line 1225 "wikitext_ragel.c"
	if ( (*p) == 46 )
		goto st23;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st22;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr155;
	} else
		goto tr155;
	goto tr152;
tr155:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
#line 287 "wikitext_ragel.rl"
	{act = 21;}
	goto st108;
st108:
	if ( ++p == pe )
		goto _test_eof108;
case 108:
#line 1247 "wikitext_ragel.c"
	if ( (*p) == 46 )
		goto st23;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st22;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st22;
	} else
		goto st22;
	goto tr152;
st109:
	if ( ++p == pe )
		goto _test_eof109;
case 109:
	switch( (*p) ) {
		case 43: goto st98;
		case 45: goto st98;
		case 47: goto st98;
		case 64: goto st98;
		case 92: goto st98;
		case 126: goto st98;
	}
	if ( (*p) < 65 ) {
		if ( (*p) > 37 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr156;
		} else if ( (*p) >= 36 )
			goto st98;
	} else if ( (*p) > 90 ) {
		if ( (*p) > 95 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr156;
		} else if ( (*p) >= 94 )
			goto st98;
	} else
		goto tr156;
	goto tr141;
tr156:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
#line 426 "wikitext_ragel.rl"
	{act = 42;}
	goto st110;
st110:
	if ( ++p == pe )
		goto _test_eof110;
case 110:
#line 1296 "wikitext_ragel.c"
	switch( (*p) ) {
		case 43: goto st98;
		case 46: goto st23;
		case 64: goto st98;
		case 92: goto st98;
		case 126: goto st98;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 37 ) {
			if ( 45 <= (*p) && (*p) <= 47 )
				goto st98;
		} else if ( (*p) >= 36 )
			goto st98;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 94 ) {
			if ( 65 <= (*p) && (*p) <= 90 )
				goto tr156;
		} else if ( (*p) > 95 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr156;
		} else
			goto st98;
	} else
		goto tr156;
	goto tr141;
tr122:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
#line 414 "wikitext_ragel.rl"
	{act = 41;}
	goto st111;
st111:
	if ( ++p == pe )
		goto _test_eof111;
case 111:
#line 1332 "wikitext_ragel.c"
	switch( (*p) ) {
		case 33: goto st97;
		case 44: goto st97;
		case 45: goto st20;
		case 46: goto tr122;
		case 63: goto st97;
		case 64: goto st21;
		case 95: goto st20;
	}
	if ( (*p) < 58 ) {
		if ( (*p) > 41 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto st20;
		} else if ( (*p) >= 40 )
			goto st97;
	} else if ( (*p) > 59 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st20;
		} else if ( (*p) >= 65 )
			goto st20;
	} else
		goto st97;
	goto tr140;
tr123:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
	goto st112;
st112:
	if ( ++p == pe )
		goto _test_eof112;
case 112:
#line 1365 "wikitext_ragel.c"
	switch( (*p) ) {
		case 47: goto st25;
		case 66: goto st55;
		case 69: goto st65;
		case 78: goto st67;
		case 80: goto st73;
		case 83: goto st76;
		case 84: goto st82;
		case 98: goto st55;
		case 101: goto st65;
		case 110: goto st67;
		case 112: goto st73;
		case 115: goto st76;
		case 116: goto st82;
	}
	goto tr157;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
	switch( (*p) ) {
		case 66: goto st26;
		case 69: goto st36;
		case 78: goto st38;
		case 80: goto st44;
		case 83: goto st47;
		case 84: goto st53;
		case 98: goto st26;
		case 101: goto st36;
		case 110: goto st38;
		case 112: goto st44;
		case 115: goto st47;
		case 116: goto st53;
	}
	goto tr30;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
	switch( (*p) ) {
		case 76: goto st27;
		case 108: goto st27;
	}
	goto tr30;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
	switch( (*p) ) {
		case 79: goto st28;
		case 111: goto st28;
	}
	goto tr30;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
	switch( (*p) ) {
		case 67: goto st29;
		case 99: goto st29;
	}
	goto tr30;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
	switch( (*p) ) {
		case 75: goto st30;
		case 107: goto st30;
	}
	goto tr30;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
	switch( (*p) ) {
		case 81: goto st31;
		case 113: goto st31;
	}
	goto tr30;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
	switch( (*p) ) {
		case 85: goto st32;
		case 117: goto st32;
	}
	goto tr30;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
	switch( (*p) ) {
		case 79: goto st33;
		case 111: goto st33;
	}
	goto tr30;
st33:
	if ( ++p == pe )
		goto _test_eof33;
case 33:
	switch( (*p) ) {
		case 84: goto st34;
		case 116: goto st34;
	}
	goto tr30;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
	switch( (*p) ) {
		case 69: goto st35;
		case 101: goto st35;
	}
	goto tr30;
st35:
	if ( ++p == pe )
		goto _test_eof35;
case 35:
	if ( (*p) == 62 )
		goto tr46;
	goto tr30;
st36:
	if ( ++p == pe )
		goto _test_eof36;
case 36:
	switch( (*p) ) {
		case 77: goto st37;
		case 109: goto st37;
	}
	goto tr30;
st37:
	if ( ++p == pe )
		goto _test_eof37;
case 37:
	if ( (*p) == 62 )
		goto tr48;
	goto tr30;
st38:
	if ( ++p == pe )
		goto _test_eof38;
case 38:
	switch( (*p) ) {
		case 79: goto st39;
		case 111: goto st39;
	}
	goto tr30;
st39:
	if ( ++p == pe )
		goto _test_eof39;
case 39:
	switch( (*p) ) {
		case 87: goto st40;
		case 119: goto st40;
	}
	goto tr30;
st40:
	if ( ++p == pe )
		goto _test_eof40;
case 40:
	switch( (*p) ) {
		case 73: goto st41;
		case 105: goto st41;
	}
	goto tr30;
st41:
	if ( ++p == pe )
		goto _test_eof41;
case 41:
	switch( (*p) ) {
		case 75: goto st42;
		case 107: goto st42;
	}
	goto tr30;
st42:
	if ( ++p == pe )
		goto _test_eof42;
case 42:
	switch( (*p) ) {
		case 73: goto st43;
		case 105: goto st43;
	}
	goto tr30;
st43:
	if ( ++p == pe )
		goto _test_eof43;
case 43:
	if ( (*p) == 62 )
		goto tr54;
	goto tr30;
st44:
	if ( ++p == pe )
		goto _test_eof44;
case 44:
	switch( (*p) ) {
		case 82: goto st45;
		case 114: goto st45;
	}
	goto tr30;
st45:
	if ( ++p == pe )
		goto _test_eof45;
case 45:
	switch( (*p) ) {
		case 69: goto st46;
		case 101: goto st46;
	}
	goto tr30;
st46:
	if ( ++p == pe )
		goto _test_eof46;
case 46:
	if ( (*p) == 62 )
		goto tr57;
	goto tr30;
st47:
	if ( ++p == pe )
		goto _test_eof47;
case 47:
	switch( (*p) ) {
		case 84: goto st48;
		case 116: goto st48;
	}
	goto tr30;
st48:
	if ( ++p == pe )
		goto _test_eof48;
case 48:
	switch( (*p) ) {
		case 82: goto st49;
		case 114: goto st49;
	}
	goto tr30;
st49:
	if ( ++p == pe )
		goto _test_eof49;
case 49:
	switch( (*p) ) {
		case 79: goto st50;
		case 111: goto st50;
	}
	goto tr30;
st50:
	if ( ++p == pe )
		goto _test_eof50;
case 50:
	switch( (*p) ) {
		case 78: goto st51;
		case 110: goto st51;
	}
	goto tr30;
st51:
	if ( ++p == pe )
		goto _test_eof51;
case 51:
	switch( (*p) ) {
		case 71: goto st52;
		case 103: goto st52;
	}
	goto tr30;
st52:
	if ( ++p == pe )
		goto _test_eof52;
case 52:
	if ( (*p) == 62 )
		goto tr63;
	goto tr30;
st53:
	if ( ++p == pe )
		goto _test_eof53;
case 53:
	switch( (*p) ) {
		case 84: goto st54;
		case 116: goto st54;
	}
	goto tr30;
st54:
	if ( ++p == pe )
		goto _test_eof54;
case 54:
	if ( (*p) == 62 )
		goto tr65;
	goto tr30;
st55:
	if ( ++p == pe )
		goto _test_eof55;
case 55:
	switch( (*p) ) {
		case 76: goto st56;
		case 108: goto st56;
	}
	goto tr30;
st56:
	if ( ++p == pe )
		goto _test_eof56;
case 56:
	switch( (*p) ) {
		case 79: goto st57;
		case 111: goto st57;
	}
	goto tr30;
st57:
	if ( ++p == pe )
		goto _test_eof57;
case 57:
	switch( (*p) ) {
		case 67: goto st58;
		case 99: goto st58;
	}
	goto tr30;
st58:
	if ( ++p == pe )
		goto _test_eof58;
case 58:
	switch( (*p) ) {
		case 75: goto st59;
		case 107: goto st59;
	}
	goto tr30;
st59:
	if ( ++p == pe )
		goto _test_eof59;
case 59:
	switch( (*p) ) {
		case 81: goto st60;
		case 113: goto st60;
	}
	goto tr30;
st60:
	if ( ++p == pe )
		goto _test_eof60;
case 60:
	switch( (*p) ) {
		case 85: goto st61;
		case 117: goto st61;
	}
	goto tr30;
st61:
	if ( ++p == pe )
		goto _test_eof61;
case 61:
	switch( (*p) ) {
		case 79: goto st62;
		case 111: goto st62;
	}
	goto tr30;
st62:
	if ( ++p == pe )
		goto _test_eof62;
case 62:
	switch( (*p) ) {
		case 84: goto st63;
		case 116: goto st63;
	}
	goto tr30;
st63:
	if ( ++p == pe )
		goto _test_eof63;
case 63:
	switch( (*p) ) {
		case 69: goto st64;
		case 101: goto st64;
	}
	goto tr30;
st64:
	if ( ++p == pe )
		goto _test_eof64;
case 64:
	if ( (*p) == 62 )
		goto tr75;
	goto tr30;
st65:
	if ( ++p == pe )
		goto _test_eof65;
case 65:
	switch( (*p) ) {
		case 77: goto st66;
		case 109: goto st66;
	}
	goto tr30;
st66:
	if ( ++p == pe )
		goto _test_eof66;
case 66:
	if ( (*p) == 62 )
		goto tr77;
	goto tr30;
st67:
	if ( ++p == pe )
		goto _test_eof67;
case 67:
	switch( (*p) ) {
		case 79: goto st68;
		case 111: goto st68;
	}
	goto tr30;
st68:
	if ( ++p == pe )
		goto _test_eof68;
case 68:
	switch( (*p) ) {
		case 87: goto st69;
		case 119: goto st69;
	}
	goto tr30;
st69:
	if ( ++p == pe )
		goto _test_eof69;
case 69:
	switch( (*p) ) {
		case 73: goto st70;
		case 105: goto st70;
	}
	goto tr30;
st70:
	if ( ++p == pe )
		goto _test_eof70;
case 70:
	switch( (*p) ) {
		case 75: goto st71;
		case 107: goto st71;
	}
	goto tr30;
st71:
	if ( ++p == pe )
		goto _test_eof71;
case 71:
	switch( (*p) ) {
		case 73: goto st72;
		case 105: goto st72;
	}
	goto tr30;
st72:
	if ( ++p == pe )
		goto _test_eof72;
case 72:
	if ( (*p) == 62 )
		goto tr83;
	goto tr30;
st73:
	if ( ++p == pe )
		goto _test_eof73;
case 73:
	switch( (*p) ) {
		case 82: goto st74;
		case 114: goto st74;
	}
	goto tr30;
st74:
	if ( ++p == pe )
		goto _test_eof74;
case 74:
	switch( (*p) ) {
		case 69: goto st75;
		case 101: goto st75;
	}
	goto tr30;
st75:
	if ( ++p == pe )
		goto _test_eof75;
case 75:
	if ( (*p) == 62 )
		goto tr86;
	goto tr30;
st76:
	if ( ++p == pe )
		goto _test_eof76;
case 76:
	switch( (*p) ) {
		case 84: goto st77;
		case 116: goto st77;
	}
	goto tr30;
st77:
	if ( ++p == pe )
		goto _test_eof77;
case 77:
	switch( (*p) ) {
		case 82: goto st78;
		case 114: goto st78;
	}
	goto tr30;
st78:
	if ( ++p == pe )
		goto _test_eof78;
case 78:
	switch( (*p) ) {
		case 79: goto st79;
		case 111: goto st79;
	}
	goto tr30;
st79:
	if ( ++p == pe )
		goto _test_eof79;
case 79:
	switch( (*p) ) {
		case 78: goto st80;
		case 110: goto st80;
	}
	goto tr30;
st80:
	if ( ++p == pe )
		goto _test_eof80;
case 80:
	switch( (*p) ) {
		case 71: goto st81;
		case 103: goto st81;
	}
	goto tr30;
st81:
	if ( ++p == pe )
		goto _test_eof81;
case 81:
	if ( (*p) == 62 )
		goto tr92;
	goto tr30;
st82:
	if ( ++p == pe )
		goto _test_eof82;
case 82:
	switch( (*p) ) {
		case 84: goto st83;
		case 116: goto st83;
	}
	goto tr30;
st83:
	if ( ++p == pe )
		goto _test_eof83;
case 83:
	if ( (*p) == 62 )
		goto tr94;
	goto tr30;
tr124:
#line 36 "wikitext_ragel.rl"
	{
        MARK();
    }
	goto st113;
st113:
	if ( ++p == pe )
		goto _test_eof113;
case 113:
#line 1909 "wikitext_ragel.c"
	switch( (*p) ) {
		case 32: goto st114;
		case 61: goto tr124;
	}
	goto tr165;
st114:
	if ( ++p == pe )
		goto _test_eof114;
case 114:
	if ( (*p) == 32 )
		goto st114;
	goto tr165;
tr125:
#line 36 "wikitext_ragel.rl"
	{
        MARK();
    }
	goto st115;
st115:
	if ( ++p == pe )
		goto _test_eof115;
case 115:
#line 1932 "wikitext_ragel.c"
	if ( (*p) == 32 )
		goto tr168;
	goto tr167;
tr126:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
#line 426 "wikitext_ragel.rl"
	{act = 42;}
	goto st116;
st116:
	if ( ++p == pe )
		goto _test_eof116;
case 116:
#line 1946 "wikitext_ragel.c"
	switch( (*p) ) {
		case 43: goto st98;
		case 46: goto st20;
		case 47: goto st98;
		case 64: goto st109;
		case 84: goto tr169;
		case 92: goto st98;
		case 94: goto st98;
		case 95: goto tr121;
		case 116: goto tr169;
		case 126: goto st98;
	}
	if ( (*p) < 45 ) {
		if ( 36 <= (*p) && (*p) <= 37 )
			goto st98;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr121;
		} else if ( (*p) >= 65 )
			goto tr121;
	} else
		goto tr121;
	goto tr141;
tr169:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
#line 426 "wikitext_ragel.rl"
	{act = 42;}
	goto st117;
st117:
	if ( ++p == pe )
		goto _test_eof117;
case 117:
#line 1981 "wikitext_ragel.c"
	switch( (*p) ) {
		case 43: goto st98;
		case 46: goto st20;
		case 47: goto st98;
		case 64: goto st109;
		case 80: goto tr170;
		case 92: goto st98;
		case 94: goto st98;
		case 95: goto tr121;
		case 112: goto tr170;
		case 126: goto st98;
	}
	if ( (*p) < 45 ) {
		if ( 36 <= (*p) && (*p) <= 37 )
			goto st98;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr121;
		} else if ( (*p) >= 65 )
			goto tr121;
	} else
		goto tr121;
	goto tr141;
tr170:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
#line 426 "wikitext_ragel.rl"
	{act = 42;}
	goto st118;
st118:
	if ( ++p == pe )
		goto _test_eof118;
case 118:
#line 2016 "wikitext_ragel.c"
	switch( (*p) ) {
		case 43: goto st98;
		case 46: goto st20;
		case 47: goto st98;
		case 58: goto st84;
		case 64: goto st109;
		case 92: goto st98;
		case 94: goto st98;
		case 95: goto tr121;
		case 126: goto st98;
	}
	if ( (*p) < 45 ) {
		if ( 36 <= (*p) && (*p) <= 37 )
			goto st98;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr121;
		} else if ( (*p) >= 65 )
			goto tr121;
	} else
		goto tr121;
	goto tr141;
st84:
	if ( ++p == pe )
		goto _test_eof84;
case 84:
	if ( (*p) == 47 )
		goto st85;
	goto tr95;
st85:
	if ( ++p == pe )
		goto _test_eof85;
case 85:
	if ( (*p) == 47 )
		goto st86;
	goto tr95;
st86:
	if ( ++p == pe )
		goto _test_eof86;
case 86:
	switch( (*p) ) {
		case 45: goto tr98;
		case 61: goto tr98;
		case 95: goto tr98;
		case 126: goto tr98;
	}
	if ( (*p) < 47 ) {
		if ( (*p) > 40 ) {
			if ( 42 <= (*p) && (*p) <= 43 )
				goto tr98;
		} else if ( (*p) >= 35 )
			goto tr98;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr98;
		} else if ( (*p) >= 64 )
			goto tr98;
	} else
		goto tr98;
	goto tr95;
tr98:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
	goto st119;
st119:
	if ( ++p == pe )
		goto _test_eof119;
case 119:
#line 2087 "wikitext_ragel.c"
	switch( (*p) ) {
		case 33: goto st87;
		case 41: goto st87;
		case 44: goto st87;
		case 46: goto st87;
		case 61: goto tr98;
		case 63: goto st87;
		case 95: goto tr98;
		case 126: goto tr98;
	}
	if ( (*p) < 58 ) {
		if ( 35 <= (*p) && (*p) <= 57 )
			goto tr98;
	} else if ( (*p) > 59 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr98;
		} else if ( (*p) >= 64 )
			goto tr98;
	} else
		goto st87;
	goto tr172;
st87:
	if ( ++p == pe )
		goto _test_eof87;
case 87:
	switch( (*p) ) {
		case 33: goto st87;
		case 41: goto st87;
		case 44: goto st87;
		case 46: goto st87;
		case 61: goto tr98;
		case 63: goto st87;
		case 95: goto tr98;
		case 126: goto tr98;
	}
	if ( (*p) < 58 ) {
		if ( 35 <= (*p) && (*p) <= 57 )
			goto tr98;
	} else if ( (*p) > 59 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr98;
		} else if ( (*p) >= 64 )
			goto tr98;
	} else
		goto st87;
	goto tr99;
tr127:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
#line 426 "wikitext_ragel.rl"
	{act = 42;}
	goto st120;
st120:
	if ( ++p == pe )
		goto _test_eof120;
case 120:
#line 2146 "wikitext_ragel.c"
	switch( (*p) ) {
		case 43: goto st98;
		case 46: goto st20;
		case 47: goto st98;
		case 64: goto st109;
		case 84: goto tr173;
		case 92: goto st98;
		case 94: goto st98;
		case 95: goto tr121;
		case 116: goto tr173;
		case 126: goto st98;
	}
	if ( (*p) < 45 ) {
		if ( 36 <= (*p) && (*p) <= 37 )
			goto st98;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr121;
		} else if ( (*p) >= 65 )
			goto tr121;
	} else
		goto tr121;
	goto tr141;
tr173:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
#line 426 "wikitext_ragel.rl"
	{act = 42;}
	goto st121;
st121:
	if ( ++p == pe )
		goto _test_eof121;
case 121:
#line 2181 "wikitext_ragel.c"
	switch( (*p) ) {
		case 43: goto st98;
		case 46: goto st20;
		case 47: goto st98;
		case 64: goto st109;
		case 84: goto tr174;
		case 92: goto st98;
		case 94: goto st98;
		case 95: goto tr121;
		case 116: goto tr174;
		case 126: goto st98;
	}
	if ( (*p) < 45 ) {
		if ( 36 <= (*p) && (*p) <= 37 )
			goto st98;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr121;
		} else if ( (*p) >= 65 )
			goto tr121;
	} else
		goto tr121;
	goto tr141;
tr174:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
#line 426 "wikitext_ragel.rl"
	{act = 42;}
	goto st122;
st122:
	if ( ++p == pe )
		goto _test_eof122;
case 122:
#line 2216 "wikitext_ragel.c"
	switch( (*p) ) {
		case 43: goto st98;
		case 46: goto st20;
		case 47: goto st98;
		case 64: goto st109;
		case 80: goto tr175;
		case 92: goto st98;
		case 94: goto st98;
		case 95: goto tr121;
		case 112: goto tr175;
		case 126: goto st98;
	}
	if ( (*p) < 45 ) {
		if ( 36 <= (*p) && (*p) <= 37 )
			goto st98;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr121;
		} else if ( (*p) >= 65 )
			goto tr121;
	} else
		goto tr121;
	goto tr141;
tr175:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
#line 426 "wikitext_ragel.rl"
	{act = 42;}
	goto st123;
st123:
	if ( ++p == pe )
		goto _test_eof123;
case 123:
#line 2251 "wikitext_ragel.c"
	switch( (*p) ) {
		case 43: goto st98;
		case 46: goto st20;
		case 47: goto st98;
		case 58: goto st84;
		case 64: goto st109;
		case 83: goto tr170;
		case 92: goto st98;
		case 94: goto st98;
		case 95: goto tr121;
		case 115: goto tr170;
		case 126: goto st98;
	}
	if ( (*p) < 45 ) {
		if ( 36 <= (*p) && (*p) <= 37 )
			goto st98;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr121;
		} else if ( (*p) >= 65 )
			goto tr121;
	} else
		goto tr121;
	goto tr141;
tr128:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
#line 426 "wikitext_ragel.rl"
	{act = 42;}
	goto st124;
st124:
	if ( ++p == pe )
		goto _test_eof124;
case 124:
#line 2287 "wikitext_ragel.c"
	switch( (*p) ) {
		case 43: goto st98;
		case 46: goto st20;
		case 47: goto st98;
		case 64: goto st109;
		case 65: goto tr176;
		case 92: goto st98;
		case 94: goto st98;
		case 95: goto tr121;
		case 97: goto tr176;
		case 126: goto st98;
	}
	if ( (*p) < 45 ) {
		if ( 36 <= (*p) && (*p) <= 37 )
			goto st98;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 98 <= (*p) && (*p) <= 122 )
				goto tr121;
		} else if ( (*p) >= 66 )
			goto tr121;
	} else
		goto tr121;
	goto tr141;
tr176:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
#line 426 "wikitext_ragel.rl"
	{act = 42;}
	goto st125;
st125:
	if ( ++p == pe )
		goto _test_eof125;
case 125:
#line 2322 "wikitext_ragel.c"
	switch( (*p) ) {
		case 43: goto st98;
		case 46: goto st20;
		case 47: goto st98;
		case 64: goto st109;
		case 73: goto tr177;
		case 92: goto st98;
		case 94: goto st98;
		case 95: goto tr121;
		case 105: goto tr177;
		case 126: goto st98;
	}
	if ( (*p) < 45 ) {
		if ( 36 <= (*p) && (*p) <= 37 )
			goto st98;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr121;
		} else if ( (*p) >= 65 )
			goto tr121;
	} else
		goto tr121;
	goto tr141;
tr177:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
#line 426 "wikitext_ragel.rl"
	{act = 42;}
	goto st126;
st126:
	if ( ++p == pe )
		goto _test_eof126;
case 126:
#line 2357 "wikitext_ragel.c"
	switch( (*p) ) {
		case 43: goto st98;
		case 46: goto st20;
		case 47: goto st98;
		case 64: goto st109;
		case 76: goto tr178;
		case 92: goto st98;
		case 94: goto st98;
		case 95: goto tr121;
		case 108: goto tr178;
		case 126: goto st98;
	}
	if ( (*p) < 45 ) {
		if ( 36 <= (*p) && (*p) <= 37 )
			goto st98;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr121;
		} else if ( (*p) >= 65 )
			goto tr121;
	} else
		goto tr121;
	goto tr141;
tr178:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
#line 426 "wikitext_ragel.rl"
	{act = 42;}
	goto st127;
st127:
	if ( ++p == pe )
		goto _test_eof127;
case 127:
#line 2392 "wikitext_ragel.c"
	switch( (*p) ) {
		case 43: goto st98;
		case 46: goto st20;
		case 47: goto st98;
		case 64: goto st109;
		case 84: goto tr179;
		case 92: goto st98;
		case 94: goto st98;
		case 95: goto tr121;
		case 116: goto tr179;
		case 126: goto st98;
	}
	if ( (*p) < 45 ) {
		if ( 36 <= (*p) && (*p) <= 37 )
			goto st98;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr121;
		} else if ( (*p) >= 65 )
			goto tr121;
	} else
		goto tr121;
	goto tr141;
tr179:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
#line 426 "wikitext_ragel.rl"
	{act = 42;}
	goto st128;
st128:
	if ( ++p == pe )
		goto _test_eof128;
case 128:
#line 2427 "wikitext_ragel.c"
	switch( (*p) ) {
		case 43: goto st98;
		case 46: goto st20;
		case 47: goto st98;
		case 64: goto st109;
		case 79: goto tr180;
		case 92: goto st98;
		case 94: goto st98;
		case 95: goto tr121;
		case 111: goto tr180;
		case 126: goto st98;
	}
	if ( (*p) < 45 ) {
		if ( 36 <= (*p) && (*p) <= 37 )
			goto st98;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr121;
		} else if ( (*p) >= 65 )
			goto tr121;
	} else
		goto tr121;
	goto tr141;
tr180:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
#line 426 "wikitext_ragel.rl"
	{act = 42;}
	goto st129;
st129:
	if ( ++p == pe )
		goto _test_eof129;
case 129:
#line 2462 "wikitext_ragel.c"
	switch( (*p) ) {
		case 43: goto st98;
		case 46: goto st20;
		case 47: goto st98;
		case 58: goto st88;
		case 64: goto st109;
		case 92: goto st98;
		case 94: goto st98;
		case 95: goto tr121;
		case 126: goto st98;
	}
	if ( (*p) < 45 ) {
		if ( 36 <= (*p) && (*p) <= 37 )
			goto st98;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr121;
		} else if ( (*p) >= 65 )
			goto tr121;
	} else
		goto tr121;
	goto tr141;
st88:
	if ( ++p == pe )
		goto _test_eof88;
case 88:
	if ( (*p) == 95 )
		goto st89;
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st89;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st89;
		} else if ( (*p) >= 65 )
			goto st89;
	} else
		goto st89;
	goto tr95;
st89:
	if ( ++p == pe )
		goto _test_eof89;
case 89:
	switch( (*p) ) {
		case 64: goto st90;
		case 95: goto st89;
	}
	if ( (*p) < 48 ) {
		if ( 45 <= (*p) && (*p) <= 46 )
			goto st89;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st89;
		} else if ( (*p) >= 65 )
			goto st89;
	} else
		goto st89;
	goto tr95;
st90:
	if ( ++p == pe )
		goto _test_eof90;
case 90:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st91;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st91;
	} else
		goto st91;
	goto tr95;
st91:
	if ( ++p == pe )
		goto _test_eof91;
case 91:
	if ( (*p) == 46 )
		goto st92;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st91;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st91;
	} else
		goto st91;
	goto tr23;
st92:
	if ( ++p == pe )
		goto _test_eof92;
case 92:
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st91;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st93;
	} else
		goto st93;
	goto tr23;
st93:
	if ( ++p == pe )
		goto _test_eof93;
case 93:
	if ( (*p) == 46 )
		goto st92;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st91;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr106;
	} else
		goto tr106;
	goto tr23;
tr106:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
#line 281 "wikitext_ragel.rl"
	{act = 20;}
	goto st130;
st130:
	if ( ++p == pe )
		goto _test_eof130;
case 130:
#line 2590 "wikitext_ragel.c"
	if ( (*p) == 46 )
		goto st92;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st91;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr182;
	} else
		goto tr182;
	goto tr172;
tr182:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
#line 281 "wikitext_ragel.rl"
	{act = 20;}
	goto st131;
st131:
	if ( ++p == pe )
		goto _test_eof131;
case 131:
#line 2612 "wikitext_ragel.c"
	if ( (*p) == 46 )
		goto st92;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st91;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr183;
	} else
		goto tr183;
	goto tr172;
tr183:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
#line 281 "wikitext_ragel.rl"
	{act = 20;}
	goto st132;
st132:
	if ( ++p == pe )
		goto _test_eof132;
case 132:
#line 2634 "wikitext_ragel.c"
	if ( (*p) == 46 )
		goto st92;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st91;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr184;
	} else
		goto tr184;
	goto tr172;
tr184:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
#line 281 "wikitext_ragel.rl"
	{act = 20;}
	goto st133;
st133:
	if ( ++p == pe )
		goto _test_eof133;
case 133:
#line 2656 "wikitext_ragel.c"
	if ( (*p) == 46 )
		goto st92;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st91;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st91;
	} else
		goto st91;
	goto tr172;
tr129:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
#line 426 "wikitext_ragel.rl"
	{act = 42;}
	goto st134;
st134:
	if ( ++p == pe )
		goto _test_eof134;
case 134:
#line 2678 "wikitext_ragel.c"
	switch( (*p) ) {
		case 43: goto st98;
		case 46: goto st20;
		case 47: goto st98;
		case 64: goto st109;
		case 86: goto tr185;
		case 92: goto st98;
		case 94: goto st98;
		case 95: goto tr121;
		case 118: goto tr185;
		case 126: goto st98;
	}
	if ( (*p) < 45 ) {
		if ( 36 <= (*p) && (*p) <= 37 )
			goto st98;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr121;
		} else if ( (*p) >= 65 )
			goto tr121;
	} else
		goto tr121;
	goto tr141;
tr185:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
#line 426 "wikitext_ragel.rl"
	{act = 42;}
	goto st135;
st135:
	if ( ++p == pe )
		goto _test_eof135;
case 135:
#line 2713 "wikitext_ragel.c"
	switch( (*p) ) {
		case 43: goto st98;
		case 46: goto st20;
		case 47: goto st98;
		case 64: goto st109;
		case 78: goto tr170;
		case 92: goto st98;
		case 94: goto st98;
		case 95: goto tr121;
		case 110: goto tr170;
		case 126: goto st98;
	}
	if ( (*p) < 45 ) {
		if ( 36 <= (*p) && (*p) <= 37 )
			goto st98;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr121;
		} else if ( (*p) >= 65 )
			goto tr121;
	} else
		goto tr121;
	goto tr141;
st136:
	if ( ++p == pe )
		goto _test_eof136;
case 136:
	if ( (*p) == 91 )
		goto tr187;
	goto tr186;
st137:
	if ( ++p == pe )
		goto _test_eof137;
case 137:
	if ( (*p) == 93 )
		goto tr189;
	goto tr188;
st138:
	if ( ++p == pe )
		goto _test_eof138;
case 138:
	if ( (*p) == 123 )
		goto tr191;
	goto tr190;
st139:
	if ( ++p == pe )
		goto _test_eof139;
case 139:
	if ( (*p) == 125 )
		goto tr193;
	goto tr192;
	}
	_test_eof94: cs = 94; goto _test_eof; 
	_test_eof1: cs = 1; goto _test_eof; 
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof95: cs = 95; goto _test_eof; 
	_test_eof96: cs = 96; goto _test_eof; 
	_test_eof97: cs = 97; goto _test_eof; 
	_test_eof98: cs = 98; goto _test_eof; 
	_test_eof99: cs = 99; goto _test_eof; 
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
	_test_eof100: cs = 100; goto _test_eof; 
	_test_eof101: cs = 101; goto _test_eof; 
	_test_eof102: cs = 102; goto _test_eof; 
	_test_eof103: cs = 103; goto _test_eof; 
	_test_eof104: cs = 104; goto _test_eof; 
	_test_eof20: cs = 20; goto _test_eof; 
	_test_eof21: cs = 21; goto _test_eof; 
	_test_eof22: cs = 22; goto _test_eof; 
	_test_eof23: cs = 23; goto _test_eof; 
	_test_eof24: cs = 24; goto _test_eof; 
	_test_eof105: cs = 105; goto _test_eof; 
	_test_eof106: cs = 106; goto _test_eof; 
	_test_eof107: cs = 107; goto _test_eof; 
	_test_eof108: cs = 108; goto _test_eof; 
	_test_eof109: cs = 109; goto _test_eof; 
	_test_eof110: cs = 110; goto _test_eof; 
	_test_eof111: cs = 111; goto _test_eof; 
	_test_eof112: cs = 112; goto _test_eof; 
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
	_test_eof79: cs = 79; goto _test_eof; 
	_test_eof80: cs = 80; goto _test_eof; 
	_test_eof81: cs = 81; goto _test_eof; 
	_test_eof82: cs = 82; goto _test_eof; 
	_test_eof83: cs = 83; goto _test_eof; 
	_test_eof113: cs = 113; goto _test_eof; 
	_test_eof114: cs = 114; goto _test_eof; 
	_test_eof115: cs = 115; goto _test_eof; 
	_test_eof116: cs = 116; goto _test_eof; 
	_test_eof117: cs = 117; goto _test_eof; 
	_test_eof118: cs = 118; goto _test_eof; 
	_test_eof84: cs = 84; goto _test_eof; 
	_test_eof85: cs = 85; goto _test_eof; 
	_test_eof86: cs = 86; goto _test_eof; 
	_test_eof119: cs = 119; goto _test_eof; 
	_test_eof87: cs = 87; goto _test_eof; 
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
	_test_eof88: cs = 88; goto _test_eof; 
	_test_eof89: cs = 89; goto _test_eof; 
	_test_eof90: cs = 90; goto _test_eof; 
	_test_eof91: cs = 91; goto _test_eof; 
	_test_eof92: cs = 92; goto _test_eof; 
	_test_eof93: cs = 93; goto _test_eof; 
	_test_eof130: cs = 130; goto _test_eof; 
	_test_eof131: cs = 131; goto _test_eof; 
	_test_eof132: cs = 132; goto _test_eof; 
	_test_eof133: cs = 133; goto _test_eof; 
	_test_eof134: cs = 134; goto _test_eof; 
	_test_eof135: cs = 135; goto _test_eof; 
	_test_eof136: cs = 136; goto _test_eof; 
	_test_eof137: cs = 137; goto _test_eof; 
	_test_eof138: cs = 138; goto _test_eof; 
	_test_eof139: cs = 139; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 95: goto tr136;
	case 96: goto tr138;
	case 97: goto tr140;
	case 98: goto tr141;
	case 99: goto tr142;
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
	case 100: goto tr146;
	case 101: goto tr146;
	case 102: goto tr146;
	case 103: goto tr146;
	case 104: goto tr141;
	case 20: goto tr23;
	case 21: goto tr23;
	case 22: goto tr23;
	case 23: goto tr23;
	case 24: goto tr23;
	case 105: goto tr152;
	case 106: goto tr152;
	case 107: goto tr152;
	case 108: goto tr152;
	case 109: goto tr141;
	case 110: goto tr141;
	case 111: goto tr140;
	case 112: goto tr157;
	case 25: goto tr30;
	case 26: goto tr30;
	case 27: goto tr30;
	case 28: goto tr30;
	case 29: goto tr30;
	case 30: goto tr30;
	case 31: goto tr30;
	case 32: goto tr30;
	case 33: goto tr30;
	case 34: goto tr30;
	case 35: goto tr30;
	case 36: goto tr30;
	case 37: goto tr30;
	case 38: goto tr30;
	case 39: goto tr30;
	case 40: goto tr30;
	case 41: goto tr30;
	case 42: goto tr30;
	case 43: goto tr30;
	case 44: goto tr30;
	case 45: goto tr30;
	case 46: goto tr30;
	case 47: goto tr30;
	case 48: goto tr30;
	case 49: goto tr30;
	case 50: goto tr30;
	case 51: goto tr30;
	case 52: goto tr30;
	case 53: goto tr30;
	case 54: goto tr30;
	case 55: goto tr30;
	case 56: goto tr30;
	case 57: goto tr30;
	case 58: goto tr30;
	case 59: goto tr30;
	case 60: goto tr30;
	case 61: goto tr30;
	case 62: goto tr30;
	case 63: goto tr30;
	case 64: goto tr30;
	case 65: goto tr30;
	case 66: goto tr30;
	case 67: goto tr30;
	case 68: goto tr30;
	case 69: goto tr30;
	case 70: goto tr30;
	case 71: goto tr30;
	case 72: goto tr30;
	case 73: goto tr30;
	case 74: goto tr30;
	case 75: goto tr30;
	case 76: goto tr30;
	case 77: goto tr30;
	case 78: goto tr30;
	case 79: goto tr30;
	case 80: goto tr30;
	case 81: goto tr30;
	case 82: goto tr30;
	case 83: goto tr30;
	case 113: goto tr165;
	case 114: goto tr165;
	case 115: goto tr167;
	case 116: goto tr141;
	case 117: goto tr141;
	case 118: goto tr141;
	case 84: goto tr95;
	case 85: goto tr95;
	case 86: goto tr95;
	case 119: goto tr172;
	case 87: goto tr99;
	case 120: goto tr141;
	case 121: goto tr141;
	case 122: goto tr141;
	case 123: goto tr141;
	case 124: goto tr141;
	case 125: goto tr141;
	case 126: goto tr141;
	case 127: goto tr141;
	case 128: goto tr141;
	case 129: goto tr141;
	case 88: goto tr95;
	case 89: goto tr95;
	case 90: goto tr95;
	case 91: goto tr23;
	case 92: goto tr23;
	case 93: goto tr23;
	case 130: goto tr172;
	case 131: goto tr172;
	case 132: goto tr172;
	case 133: goto tr172;
	case 134: goto tr141;
	case 135: goto tr141;
	case 136: goto tr186;
	case 137: goto tr188;
	case 138: goto tr190;
	case 139: goto tr192;
	}
	}

	_out: {}
	}
#line 498 "wikitext_ragel.rl"
    if (cs == wikitext_error)
        rb_raise(eWikitextParserError, "failed before finding a token");
    else if (out->type == NO_TOKEN)
        rb_raise(eWikitextParserError, "failed to produce a token");
}
