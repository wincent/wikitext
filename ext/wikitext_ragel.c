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
static const char _wikitext_actions[] = {
	0, 1, 0, 1, 1, 1, 5, 1, 
	6, 1, 7, 1, 12, 1, 13, 1, 
	14, 1, 15, 1, 16, 1, 17, 1, 
	18, 1, 19, 1, 20, 1, 21, 1, 
	22, 1, 23, 1, 24, 1, 25, 1, 
	26, 1, 27, 1, 28, 1, 30, 1, 
	31, 1, 32, 1, 33, 1, 34, 1, 
	35, 1, 36, 1, 37, 1, 38, 1, 
	39, 1, 40, 1, 41, 1, 42, 1, 
	43, 1, 44, 2, 1, 29, 2, 2, 
	29, 2, 3, 29, 2, 4, 29, 2, 
	7, 8, 2, 7, 9, 2, 7, 10, 
	2, 7, 11, 2, 28, 1
};

static const short _wikitext_key_offsets[] = {
	0, 0, 2, 4, 6, 8, 10, 12, 
	16, 19, 25, 32, 39, 42, 50, 58, 
	65, 73, 81, 89, 96, 100, 102, 104, 
	106, 108, 110, 111, 113, 114, 116, 118, 
	120, 122, 124, 125, 127, 128, 144, 177, 
	178, 179, 192, 199, 200, 201, 202, 203, 
	208, 210, 211, 212, 227, 242, 256, 270, 
	284, 307, 330, 346, 361, 376, 391, 392
};

static const char _wikitext_trans_keys[] = {
	-128, -65, -128, -65, -128, -65, -128, -65, 
	-128, -65, -128, -65, 88, 120, 48, 57, 
	59, 48, 57, 48, 57, 65, 70, 97, 
	102, 59, 48, 57, 65, 70, 97, 102, 
	59, 48, 57, 65, 90, 97, 122, 59, 
	48, 57, 59, 109, 48, 57, 65, 90, 
	97, 122, 59, 112, 48, 57, 65, 90, 
	97, 122, 59, 48, 57, 65, 90, 97, 
	122, 59, 117, 48, 57, 65, 90, 97, 
	122, 59, 111, 48, 57, 65, 90, 97, 
	122, 59, 116, 48, 57, 65, 90, 97, 
	122, 59, 48, 57, 65, 90, 97, 122, 
	78, 84, 110, 116, 79, 111, 87, 119, 
	73, 105, 75, 107, 73, 105, 62, 84, 
	116, 62, 79, 111, 87, 119, 73, 105, 
	75, 107, 73, 105, 62, 84, 116, 62, 
	33, 41, 44, 46, 61, 63, 95, 126, 
	35, 57, 58, 59, 64, 90, 97, 122, 
	10, 13, 32, 34, 35, 38, 39, 42, 
	60, 61, 62, 70, 72, 83, 91, 93, 
	96, 102, 104, 115, 124, 126, 127, -62, 
	-33, -32, -17, -16, -12, 1, 31, 33, 
	123, 10, 32, 33, 92, 126, 35, 37, 
	40, 59, 63, 90, 94, 95, 97, 123, 
	35, 97, 113, 65, 90, 98, 122, 39, 
	39, 39, 39, 47, 78, 84, 110, 116, 
	32, 61, 32, 32, 33, 84, 92, 116, 
	126, 35, 37, 40, 59, 63, 90, 94, 
	95, 97, 123, 33, 80, 92, 112, 126, 
	35, 37, 40, 59, 63, 90, 94, 95, 
	97, 123, 33, 58, 92, 126, 35, 37, 
	40, 59, 63, 90, 94, 95, 97, 123, 
	33, 47, 92, 126, 35, 37, 40, 59, 
	63, 90, 94, 95, 97, 123, 33, 47, 
	92, 126, 35, 37, 40, 59, 63, 90, 
	94, 95, 97, 123, 33, 41, 44, 46, 
	61, 63, 92, 94, 95, 123, 126, 35, 
	37, 38, 39, 40, 57, 58, 59, 64, 
	90, 97, 122, 33, 41, 44, 46, 61, 
	63, 92, 94, 95, 123, 126, 35, 37, 
	38, 39, 40, 57, 58, 59, 64, 90, 
	97, 122, 33, 41, 44, 46, 61, 63, 
	95, 126, 35, 57, 58, 59, 64, 90, 
	97, 122, 33, 84, 92, 116, 126, 35, 
	37, 40, 59, 63, 90, 94, 95, 97, 
	123, 33, 86, 92, 118, 126, 35, 37, 
	40, 59, 63, 90, 94, 95, 97, 123, 
	33, 78, 92, 110, 126, 35, 37, 40, 
	59, 63, 90, 94, 95, 97, 123, 91, 
	93, 0
};

static const char _wikitext_single_lengths[] = {
	0, 0, 0, 0, 0, 0, 0, 2, 
	1, 0, 1, 1, 1, 2, 2, 1, 
	2, 2, 2, 1, 4, 2, 2, 2, 
	2, 2, 1, 2, 1, 2, 2, 2, 
	2, 2, 1, 2, 1, 8, 23, 1, 
	1, 3, 3, 1, 1, 1, 1, 5, 
	2, 1, 1, 5, 5, 4, 4, 4, 
	11, 11, 8, 5, 5, 5, 1, 1
};

static const char _wikitext_range_lengths[] = {
	0, 1, 1, 1, 1, 1, 1, 1, 
	1, 3, 3, 3, 1, 3, 3, 3, 
	3, 3, 3, 3, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 4, 5, 0, 
	0, 5, 2, 0, 0, 0, 0, 0, 
	0, 0, 0, 5, 5, 5, 5, 5, 
	6, 6, 4, 5, 5, 5, 0, 0
};

static const short _wikitext_index_offsets[] = {
	0, 0, 2, 4, 6, 8, 10, 12, 
	16, 19, 23, 28, 33, 36, 42, 48, 
	53, 59, 65, 71, 76, 81, 84, 87, 
	90, 93, 96, 98, 101, 103, 106, 109, 
	112, 115, 118, 120, 123, 125, 138, 167, 
	169, 171, 180, 186, 188, 190, 192, 194, 
	200, 203, 205, 207, 218, 229, 239, 249, 
	259, 277, 295, 308, 319, 330, 341, 343
};

static const char _wikitext_indicies[] = {
	0, 1, 2, 1, 3, 1, 4, 1, 
	5, 1, 6, 1, 9, 9, 8, 7, 
	10, 8, 7, 11, 11, 11, 7, 12, 
	11, 11, 11, 7, 14, 13, 15, 15, 
	7, 14, 13, 7, 14, 16, 13, 15, 
	15, 7, 14, 17, 13, 15, 15, 7, 
	18, 13, 15, 15, 7, 14, 19, 13, 
	15, 15, 7, 14, 20, 13, 15, 15, 
	7, 14, 21, 13, 15, 15, 7, 22, 
	13, 15, 15, 7, 24, 25, 24, 25, 
	23, 26, 26, 23, 27, 27, 23, 28, 
	28, 23, 29, 29, 23, 30, 30, 23, 
	31, 23, 32, 32, 23, 33, 23, 34, 
	34, 23, 35, 35, 23, 36, 36, 23, 
	37, 37, 23, 38, 38, 23, 39, 23, 
	40, 40, 23, 41, 23, 43, 43, 43, 
	43, 44, 43, 44, 44, 44, 43, 44, 
	44, 42, 49, 50, 51, 53, 54, 55, 
	56, 57, 58, 59, 60, 61, 62, 63, 
	64, 65, 66, 61, 62, 63, 67, 52, 
	48, 45, 46, 47, 48, 52, 1, 69, 
	68, 71, 70, 52, 52, 52, 52, 52, 
	52, 52, 52, 72, 74, 75, 76, 15, 
	15, 73, 78, 77, 79, 77, 80, 77, 
	81, 77, 83, 84, 85, 84, 85, 82, 
	87, 59, 86, 87, 86, 89, 88, 52, 
	91, 52, 91, 52, 52, 52, 52, 52, 
	52, 90, 52, 92, 52, 92, 52, 52, 
	52, 52, 52, 52, 90, 52, 93, 52, 
	52, 52, 52, 52, 52, 52, 90, 52, 
	94, 52, 52, 52, 52, 52, 52, 52, 
	90, 52, 95, 52, 52, 52, 52, 52, 
	52, 52, 90, 52, 52, 52, 52, 44, 
	52, 52, 52, 96, 52, 96, 96, 44, 
	96, 52, 96, 96, 90, 97, 97, 97, 
	97, 44, 97, 52, 52, 96, 52, 96, 
	96, 44, 96, 97, 96, 96, 72, 43, 
	43, 43, 43, 44, 43, 44, 44, 44, 
	43, 44, 44, 98, 52, 61, 52, 61, 
	52, 52, 52, 52, 52, 52, 90, 52, 
	99, 52, 99, 52, 52, 52, 52, 52, 
	52, 90, 52, 92, 52, 92, 52, 52, 
	52, 52, 52, 52, 90, 101, 100, 103, 
	102, 0
};

static const char _wikitext_trans_targs_wi[] = {
	38, 0, 3, 38, 5, 6, 38, 38, 
	8, 9, 38, 10, 38, 12, 38, 11, 
	14, 15, 38, 17, 18, 19, 38, 38, 
	21, 27, 22, 23, 24, 25, 26, 38, 
	28, 38, 30, 31, 32, 33, 34, 38, 
	36, 38, 38, 37, 58, 1, 2, 4, 
	38, 38, 39, 40, 41, 38, 41, 42, 
	43, 41, 47, 48, 50, 51, 59, 60, 
	62, 63, 38, 38, 38, 38, 38, 40, 
	38, 38, 7, 13, 16, 38, 44, 45, 
	46, 38, 38, 20, 29, 35, 38, 49, 
	38, 38, 38, 52, 53, 54, 55, 56, 
	57, 57, 38, 61, 38, 38, 38, 38
};

static const char _wikitext_trans_actions_wi[] = {
	78, 0, 0, 81, 0, 0, 84, 69, 
	0, 0, 39, 0, 37, 0, 35, 0, 
	0, 0, 33, 0, 0, 0, 31, 71, 
	0, 0, 0, 0, 0, 0, 0, 13, 
	0, 21, 0, 0, 0, 0, 0, 11, 
	0, 19, 67, 0, 9, 0, 0, 0, 
	75, 99, 3, 1, 96, 41, 87, 9, 
	0, 90, 9, 1, 1, 0, 0, 0, 
	0, 0, 17, 29, 63, 43, 49, 0, 
	73, 59, 0, 0, 0, 45, 0, 0, 
	0, 15, 61, 0, 0, 0, 51, 0, 
	47, 23, 65, 0, 0, 0, 0, 0, 
	93, 96, 53, 0, 55, 25, 57, 27
};

static const char _wikitext_to_state_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 5, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0
};

static const char _wikitext_from_state_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 7, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0
};

static const char _wikitext_eof_trans[] = {
	0, 0, 0, 0, 0, 0, 0, 8, 
	8, 8, 8, 8, 8, 8, 8, 8, 
	8, 8, 8, 8, 24, 24, 24, 24, 
	24, 24, 24, 24, 24, 24, 24, 24, 
	24, 24, 24, 24, 24, 43, 0, 69, 
	71, 73, 74, 78, 78, 78, 78, 83, 
	87, 87, 89, 91, 91, 91, 91, 91, 
	91, 73, 99, 91, 91, 91, 101, 103
};

static const int wikitext_start = 38;
static const int wikitext_first_final = 38;
static const int wikitext_error = 0;

static const int wikitext_en_main = 38;

#line 348 "wikitext_ragel.rl"


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
    
#line 310 "wikitext_ragel.c"
	{
	cs = wikitext_start;
	ts = 0;
	te = 0;
	act = 0;
	}
#line 390 "wikitext_ragel.rl"
    
#line 319 "wikitext_ragel.c"
	{
	int _klen;
	unsigned int _trans;
	const char *_acts;
	unsigned int _nacts;
	const char *_keys;

	if ( p == pe )
		goto _test_eof;
	if ( cs == 0 )
		goto _out;
_resume:
	_acts = _wikitext_actions + _wikitext_from_state_actions[cs];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 ) {
		switch ( *_acts++ ) {
	case 6:
#line 1 "wikitext_ragel.rl"
	{ts = p;}
	break;
#line 340 "wikitext_ragel.c"
		}
	}

	_keys = _wikitext_trans_keys + _wikitext_key_offsets[cs];
	_trans = _wikitext_index_offsets[cs];

	_klen = _wikitext_single_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + _klen - 1;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + ((_upper-_lower) >> 1);
			if ( (*p) < *_mid )
				_upper = _mid - 1;
			else if ( (*p) > *_mid )
				_lower = _mid + 1;
			else {
				_trans += (_mid - _keys);
				goto _match;
			}
		}
		_keys += _klen;
		_trans += _klen;
	}

	_klen = _wikitext_range_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + (_klen<<1) - 2;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + (((_upper-_lower) >> 1) & ~1);
			if ( (*p) < _mid[0] )
				_upper = _mid - 2;
			else if ( (*p) > _mid[1] )
				_lower = _mid + 2;
			else {
				_trans += ((_mid - _keys)>>1);
				goto _match;
			}
		}
		_trans += _klen;
	}

_match:
	_trans = _wikitext_indicies[_trans];
_eof_trans:
	cs = _wikitext_trans_targs_wi[_trans];

	if ( _wikitext_trans_actions_wi[_trans] == 0 )
		goto _again;

	_acts = _wikitext_actions + _wikitext_trans_actions_wi[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
#line 36 "wikitext_ragel.rl"
	{
        MARK();
    }
	break;
	case 1:
#line 41 "wikitext_ragel.rl"
	{
        out->code_point = *p & 0x7f;
    }
	break;
	case 2:
#line 46 "wikitext_ragel.rl"
	{
        out->code_point = ((uint32_t)(*(p - 1)) & 0x1f) << 6 |
            (*p & 0x3f);
    }
	break;
	case 3:
#line 52 "wikitext_ragel.rl"
	{
        out->code_point = ((uint32_t)(*(p - 2)) & 0x0f) << 12 |
            ((uint32_t)(*(p - 1)) & 0x3f) << 6 |
            (*p & 0x3f);
    }
	break;
	case 4:
#line 59 "wikitext_ragel.rl"
	{
        out->code_point = ((uint32_t)(*(p - 3)) & 0x07) << 18 |
            ((uint32_t)(*(p - 2)) & 0x3f) << 12 |
            ((uint32_t)(*(p - 1)) & 0x3f) << 6 |
            (*p & 0x3f);
    }
	break;
	case 7:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
	break;
	case 8:
#line 148 "wikitext_ragel.rl"
	{act = 9;}
	break;
	case 9:
#line 157 "wikitext_ragel.rl"
	{act = 10;}
	break;
	case 10:
#line 218 "wikitext_ragel.rl"
	{act = 12;}
	break;
	case 11:
#line 319 "wikitext_ragel.rl"
	{act = 28;}
	break;
	case 12:
#line 73 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(NO_WIKI_START);
            {p++; goto _out; }
        }}
	break;
	case 13:
#line 79 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(NO_WIKI_END);
            {p++; goto _out; }
        }}
	break;
	case 14:
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
            {p++; goto _out; }
        }}
	break;
	case 15:
#line 103 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(TT);
            {p++; goto _out; }
        }}
	break;
	case 16:
#line 109 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(TT_START);
            {p++; goto _out; }
        }}
	break;
	case 17:
#line 115 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(TT_END);
            {p++; goto _out; }
        }}
	break;
	case 18:
#line 122 "wikitext_ragel.rl"
	{te = p+1;{
            if (out->column_start == 1 || last_token_type == BLOCKQUOTE)
                EMIT(BLOCKQUOTE);
            else
            {
                REWIND();
                EMIT(GREATER);
            }
            {p++; goto _out; }
        }}
	break;
	case 19:
#line 224 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(LINK_START);
            {p++; goto _out; }
        }}
	break;
	case 20:
#line 230 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(LINK_END);
            {p++; goto _out; }
        }}
	break;
	case 21:
#line 236 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(SEPARATOR);
            {p++; goto _out; }
        }}
	break;
	case 22:
#line 254 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(QUOT_ENTITY);
            {p++; goto _out; }
        }}
	break;
	case 23:
#line 260 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(AMP_ENTITY);
            {p++; goto _out; }
        }}
	break;
	case 24:
#line 266 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(NAMED_ENTITY);
            {p++; goto _out; }
        }}
	break;
	case 25:
#line 272 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(HEX_ENTITY);
            {p++; goto _out; }
        }}
	break;
	case 26:
#line 278 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(DECIMAL_ENTITY);
            {p++; goto _out; }
        }}
	break;
	case 27:
#line 284 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(QUOT);
            {p++; goto _out; }
        }}
	break;
	case 28:
#line 308 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(CRLF);
            out->column_stop = 1;
            out->line_stop++;
            {p++; goto _out; }
        }}
	break;
	case 29:
#line 339 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(DEFAULT);
            out->column_stop = out->column_start + 1;
            {p++; goto _out; }
        }}
	break;
	case 30:
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
            {p++; goto _out; }
        }}
	break;
	case 31:
#line 122 "wikitext_ragel.rl"
	{te = p;p--;{
            if (out->column_start == 1 || last_token_type == BLOCKQUOTE)
                EMIT(BLOCKQUOTE);
            else
            {
                REWIND();
                EMIT(GREATER);
            }
            {p++; goto _out; }
        }}
	break;
	case 32:
#line 136 "wikitext_ragel.rl"
	{te = p;p--;{
            if (out->column_start == 1)
            {
                REWIND();
                EMIT(PRE);
            }
            else
                EMIT(SPACE);
            {p++; goto _out; }
        }}
	break;
	case 33:
#line 166 "wikitext_ragel.rl"
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
            {p++; goto _out; }
        }}
	break;
	case 34:
#line 218 "wikitext_ragel.rl"
	{te = p;p--;{
            EMIT(URI);
            {p++; goto _out; }
        }}
	break;
	case 35:
#line 242 "wikitext_ragel.rl"
	{te = p;p--;{
            EMIT(EXT_LINK_START);
            {p++; goto _out; }
        }}
	break;
	case 36:
#line 248 "wikitext_ragel.rl"
	{te = p;p--;{
            EMIT(EXT_LINK_END);
            {p++; goto _out; }
        }}
	break;
	case 37:
#line 290 "wikitext_ragel.rl"
	{te = p;p--;{
            EMIT(AMP);
            {p++; goto _out; }
        }}
	break;
	case 38:
#line 296 "wikitext_ragel.rl"
	{te = p;p--;{
            EMIT(LESS);
            {p++; goto _out; }
        }}
	break;
	case 39:
#line 308 "wikitext_ragel.rl"
	{te = p;p--;{
            EMIT(CRLF);
            out->column_stop = 1;
            out->line_stop++;
            {p++; goto _out; }
        }}
	break;
	case 40:
#line 319 "wikitext_ragel.rl"
	{te = p;p--;{
            EMIT(PRINTABLE);
            {p++; goto _out; }
        }}
	break;
	case 41:
#line 218 "wikitext_ragel.rl"
	{{p = ((te))-1;}{
            EMIT(URI);
            {p++; goto _out; }
        }}
	break;
	case 42:
#line 290 "wikitext_ragel.rl"
	{{p = ((te))-1;}{
            EMIT(AMP);
            {p++; goto _out; }
        }}
	break;
	case 43:
#line 296 "wikitext_ragel.rl"
	{{p = ((te))-1;}{
            EMIT(LESS);
            {p++; goto _out; }
        }}
	break;
	case 44:
#line 1 "wikitext_ragel.rl"
	{	switch( act ) {
	case 9:
	{{p = ((te))-1;}
            if (out->column_start == 1 || last_token_type == OL || last_token_type == UL || last_token_type == BLOCKQUOTE)
                EMIT(OL);
            else
                EMIT(PRINTABLE);
            {p++; goto _out; }
        }
	break;
	case 10:
	{{p = ((te))-1;}
            if (out->column_start == 1 || last_token_type == OL || last_token_type == UL || last_token_type == BLOCKQUOTE)
                EMIT(UL);
            else
                EMIT(PRINTABLE);
            {p++; goto _out; }
        }
	break;
	case 12:
	{{p = ((te))-1;}
            EMIT(URI);
            {p++; goto _out; }
        }
	break;
	case 28:
	{{p = ((te))-1;}
            EMIT(PRINTABLE);
            {p++; goto _out; }
        }
	break;
	default: break;
	}
	}
	break;
#line 816 "wikitext_ragel.c"
		}
	}

_again:
	_acts = _wikitext_actions + _wikitext_to_state_actions[cs];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 ) {
		switch ( *_acts++ ) {
	case 5:
#line 1 "wikitext_ragel.rl"
	{ts = 0;}
	break;
#line 829 "wikitext_ragel.c"
		}
	}

	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	if ( p == eof )
	{
	if ( _wikitext_eof_trans[cs] > 0 ) {
		_trans = _wikitext_eof_trans[cs] - 1;
		goto _eof_trans;
	}
	}

	_out: {}
	}
#line 391 "wikitext_ragel.rl"
    if (cs == wikitext_error)
        rb_raise(eWikitextParserError, "failed before finding a token");
    else if (out->type == NO_TOKEN)
        rb_raise(eWikitextParserError, "failed to produce a token");
}
