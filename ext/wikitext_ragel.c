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


#line 32 "wikitext_ragel.c"
static const char _wikitext_actions[] = {
	0, 1, 0, 1, 2, 1, 3, 1, 
	4, 1, 10, 1, 11, 1, 12, 1, 
	13, 1, 14, 1, 15, 1, 16, 1, 
	17, 1, 18, 1, 19, 1, 20, 1, 
	21, 1, 22, 1, 23, 1, 24, 1, 
	25, 1, 26, 1, 27, 1, 28, 1, 
	29, 1, 30, 1, 31, 1, 32, 1, 
	33, 1, 34, 1, 35, 1, 36, 1, 
	37, 1, 38, 1, 39, 1, 40, 1, 
	41, 1, 42, 1, 43, 1, 44, 1, 
	45, 1, 46, 1, 47, 2, 1, 42, 
	2, 4, 5, 2, 4, 6, 2, 4, 
	7, 2, 4, 8, 2, 4, 9
};

static const short _wikitext_key_offsets[] = {
	0, 0, 2, 4, 6, 10, 13, 19, 
	26, 33, 36, 44, 52, 59, 67, 75, 
	83, 90, 94, 96, 98, 100, 102, 104, 
	105, 107, 108, 110, 112, 114, 116, 118, 
	119, 121, 122, 138, 171, 172, 173, 187, 
	194, 209, 224, 239, 254, 259, 274, 275, 
	290, 291, 306, 307, 322, 323, 338, 339, 
	354, 355, 356, 372, 388, 403, 418, 433, 
	453, 473, 489, 505, 521, 537, 538
};

static const char _wikitext_trans_keys[] = {
	-128, -65, -128, -65, -128, -65, 88, 120, 
	48, 57, 59, 48, 57, 48, 57, 65, 
	70, 97, 102, 59, 48, 57, 65, 70, 
	97, 102, 59, 48, 57, 65, 90, 97, 
	122, 59, 48, 57, 59, 109, 48, 57, 
	65, 90, 97, 122, 59, 112, 48, 57, 
	65, 90, 97, 122, 59, 48, 57, 65, 
	90, 97, 122, 59, 117, 48, 57, 65, 
	90, 97, 122, 59, 111, 48, 57, 65, 
	90, 97, 122, 59, 116, 48, 57, 65, 
	90, 97, 122, 59, 48, 57, 65, 90, 
	97, 122, 78, 84, 110, 116, 79, 111, 
	87, 119, 73, 105, 75, 107, 73, 105, 
	62, 84, 116, 62, 79, 111, 87, 119, 
	73, 105, 75, 107, 73, 105, 62, 84, 
	116, 62, 33, 41, 44, 46, 61, 63, 
	95, 126, 35, 57, 58, 59, 64, 90, 
	97, 122, 10, 13, 32, 34, 35, 38, 
	39, 42, 60, 61, 62, 70, 72, 83, 
	91, 93, 96, 102, 104, 115, 124, 126, 
	127, -62, -33, -32, -17, -16, -12, 1, 
	31, 33, 123, 10, 32, 33, 61, 92, 
	126, 35, 37, 39, 59, 63, 90, 94, 
	95, 97, 123, 35, 97, 113, 65, 90, 
	98, 122, 33, 39, 61, 92, 126, 35, 
	37, 40, 59, 63, 90, 94, 95, 97, 
	123, 33, 39, 61, 92, 126, 35, 37, 
	40, 59, 63, 90, 94, 95, 97, 123, 
	33, 39, 61, 92, 126, 35, 37, 40, 
	59, 63, 90, 94, 95, 97, 123, 33, 
	39, 61, 92, 126, 35, 37, 40, 59, 
	63, 90, 94, 95, 97, 123, 47, 78, 
	84, 110, 116, 32, 33, 61, 92, 126, 
	35, 37, 39, 59, 63, 90, 94, 95, 
	97, 123, 32, 32, 33, 61, 92, 126, 
	35, 37, 39, 59, 63, 90, 94, 95, 
	97, 123, 32, 32, 33, 61, 92, 126, 
	35, 37, 39, 59, 63, 90, 94, 95, 
	97, 123, 32, 32, 33, 61, 92, 126, 
	35, 37, 39, 59, 63, 90, 94, 95, 
	97, 123, 32, 32, 33, 61, 92, 126, 
	35, 37, 39, 59, 63, 90, 94, 95, 
	97, 123, 32, 32, 33, 61, 92, 126, 
	35, 37, 39, 59, 63, 90, 94, 95, 
	97, 123, 32, 32, 33, 61, 84, 92, 
	116, 126, 35, 37, 39, 59, 63, 90, 
	94, 95, 97, 123, 33, 61, 80, 92, 
	112, 126, 35, 37, 39, 59, 63, 90, 
	94, 95, 97, 123, 33, 58, 61, 92, 
	126, 35, 37, 39, 59, 63, 90, 94, 
	95, 97, 123, 33, 47, 61, 92, 126, 
	35, 37, 39, 59, 63, 90, 94, 95, 
	97, 123, 33, 47, 61, 92, 126, 35, 
	37, 39, 59, 63, 90, 94, 95, 97, 
	123, 33, 38, 41, 44, 46, 61, 63, 
	92, 94, 95, 123, 126, 35, 57, 58, 
	59, 64, 90, 97, 122, 33, 38, 41, 
	44, 46, 61, 63, 92, 94, 95, 123, 
	126, 35, 57, 58, 59, 64, 90, 97, 
	122, 33, 41, 44, 46, 61, 63, 95, 
	126, 35, 57, 58, 59, 64, 90, 97, 
	122, 33, 61, 84, 92, 116, 126, 35, 
	37, 39, 59, 63, 90, 94, 95, 97, 
	123, 33, 61, 86, 92, 118, 126, 35, 
	37, 39, 59, 63, 90, 94, 95, 97, 
	123, 33, 61, 78, 92, 110, 126, 35, 
	37, 39, 59, 63, 90, 94, 95, 97, 
	123, 91, 93, 0
};

static const char _wikitext_single_lengths[] = {
	0, 0, 0, 0, 2, 1, 0, 1, 
	1, 1, 2, 2, 1, 2, 2, 2, 
	1, 4, 2, 2, 2, 2, 2, 1, 
	2, 1, 2, 2, 2, 2, 2, 1, 
	2, 1, 8, 23, 1, 1, 4, 3, 
	5, 5, 5, 5, 5, 5, 1, 5, 
	1, 5, 1, 5, 1, 5, 1, 5, 
	1, 1, 6, 6, 5, 5, 5, 12, 
	12, 8, 6, 6, 6, 1, 1
};

static const char _wikitext_range_lengths[] = {
	0, 1, 1, 1, 1, 1, 3, 3, 
	3, 1, 3, 3, 3, 3, 3, 3, 
	3, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 4, 5, 0, 0, 5, 2, 
	5, 5, 5, 5, 0, 5, 0, 5, 
	0, 5, 0, 5, 0, 5, 0, 5, 
	0, 0, 5, 5, 5, 5, 5, 4, 
	4, 4, 5, 5, 5, 0, 0
};

static const short _wikitext_index_offsets[] = {
	0, 0, 2, 4, 6, 10, 13, 17, 
	22, 27, 30, 36, 42, 47, 53, 59, 
	65, 70, 75, 78, 81, 84, 87, 90, 
	92, 95, 97, 100, 103, 106, 109, 112, 
	114, 117, 119, 132, 161, 163, 165, 175, 
	181, 192, 203, 214, 225, 231, 242, 244, 
	255, 257, 268, 270, 281, 283, 294, 296, 
	307, 309, 311, 323, 335, 346, 357, 368, 
	385, 402, 415, 427, 439, 451, 453
};

static const char _wikitext_indicies[] = {
	0, 1, 2, 1, 3, 1, 6, 6, 
	5, 4, 7, 5, 4, 8, 8, 8, 
	4, 9, 8, 8, 8, 4, 11, 10, 
	12, 12, 4, 11, 10, 4, 11, 13, 
	10, 12, 12, 4, 11, 14, 10, 12, 
	12, 4, 15, 10, 12, 12, 4, 11, 
	16, 10, 12, 12, 4, 11, 17, 10, 
	12, 12, 4, 11, 18, 10, 12, 12, 
	4, 19, 10, 12, 12, 4, 21, 22, 
	21, 22, 20, 23, 23, 20, 24, 24, 
	20, 25, 25, 20, 26, 26, 20, 27, 
	27, 20, 28, 20, 29, 29, 20, 30, 
	20, 31, 31, 20, 32, 32, 20, 33, 
	33, 20, 34, 34, 20, 35, 35, 20, 
	36, 20, 37, 37, 20, 38, 20, 40, 
	40, 40, 40, 41, 40, 41, 41, 41, 
	40, 41, 41, 39, 43, 44, 45, 47, 
	48, 49, 50, 51, 52, 53, 54, 55, 
	56, 57, 58, 59, 60, 55, 56, 57, 
	61, 46, 0, 2, 3, 42, 0, 46, 
	1, 43, 63, 65, 64, 46, 46, 46, 
	46, 46, 46, 46, 46, 46, 66, 68, 
	69, 70, 12, 12, 67, 46, 72, 46, 
	46, 46, 46, 46, 46, 46, 46, 71, 
	46, 74, 46, 46, 46, 46, 46, 46, 
	46, 46, 73, 46, 76, 46, 46, 46, 
	46, 46, 46, 46, 46, 75, 46, 77, 
	46, 46, 46, 46, 46, 46, 46, 46, 
	71, 79, 80, 81, 80, 81, 78, 83, 
	46, 84, 46, 46, 46, 46, 46, 46, 
	46, 82, 83, 82, 86, 46, 87, 46, 
	46, 46, 46, 46, 46, 46, 85, 86, 
	85, 89, 46, 90, 46, 46, 46, 46, 
	46, 46, 46, 88, 89, 88, 92, 46, 
	93, 46, 46, 46, 46, 46, 46, 46, 
	91, 92, 91, 95, 46, 96, 46, 46, 
	46, 46, 46, 46, 46, 94, 95, 94, 
	98, 46, 46, 46, 46, 46, 46, 46, 
	46, 46, 97, 98, 97, 100, 99, 46, 
	46, 101, 46, 101, 46, 46, 46, 46, 
	46, 46, 71, 46, 46, 102, 46, 102, 
	46, 46, 46, 46, 46, 46, 71, 46, 
	103, 46, 46, 46, 46, 46, 46, 46, 
	46, 71, 46, 104, 46, 46, 46, 46, 
	46, 46, 46, 46, 71, 46, 105, 46, 
	46, 46, 46, 46, 46, 46, 46, 71, 
	46, 41, 46, 46, 46, 106, 46, 46, 
	46, 106, 46, 106, 106, 46, 106, 106, 
	71, 107, 41, 107, 107, 107, 106, 107, 
	46, 46, 106, 46, 106, 106, 107, 106, 
	106, 66, 40, 40, 40, 40, 41, 40, 
	41, 41, 41, 40, 41, 41, 108, 46, 
	46, 55, 46, 55, 46, 46, 46, 46, 
	46, 46, 71, 46, 46, 109, 46, 109, 
	46, 46, 46, 46, 46, 46, 71, 46, 
	46, 102, 46, 102, 46, 46, 46, 46, 
	46, 46, 71, 111, 110, 113, 112, 0
};

static const char _wikitext_trans_targs_wi[] = {
	35, 0, 1, 2, 35, 5, 6, 35, 
	7, 35, 9, 35, 8, 11, 12, 35, 
	14, 15, 16, 35, 35, 18, 24, 19, 
	20, 21, 22, 23, 35, 25, 35, 27, 
	28, 29, 30, 31, 35, 33, 35, 35, 
	34, 65, 3, 35, 36, 37, 38, 35, 
	38, 39, 40, 38, 44, 45, 57, 58, 
	66, 67, 69, 70, 35, 35, 35, 35, 
	35, 37, 35, 35, 4, 10, 13, 35, 
	41, 35, 42, 35, 43, 38, 35, 17, 
	26, 32, 35, 46, 47, 35, 48, 49, 
	35, 50, 51, 35, 52, 53, 35, 54, 
	55, 35, 56, 35, 35, 59, 60, 61, 
	62, 63, 64, 64, 35, 68, 35, 35, 
	35, 35
};

static const char _wikitext_trans_actions_wi[] = {
	41, 0, 0, 0, 79, 0, 0, 35, 
	0, 33, 0, 31, 0, 0, 0, 29, 
	0, 0, 0, 27, 81, 0, 0, 0, 
	0, 0, 0, 0, 11, 0, 17, 0, 
	0, 0, 0, 0, 9, 0, 15, 77, 
	0, 7, 0, 39, 0, 1, 100, 37, 
	91, 7, 0, 94, 7, 1, 1, 0, 
	0, 0, 0, 0, 13, 25, 85, 73, 
	49, 0, 83, 69, 0, 0, 0, 75, 
	0, 45, 0, 43, 0, 88, 71, 0, 
	0, 0, 61, 0, 1, 59, 0, 1, 
	57, 0, 1, 55, 0, 1, 53, 0, 
	1, 51, 0, 47, 19, 0, 0, 0, 
	0, 0, 97, 100, 63, 0, 65, 21, 
	67, 23
};

static const char _wikitext_to_state_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 3, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0
};

static const char _wikitext_from_state_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 5, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0
};

static const char _wikitext_eof_trans[] = {
	0, 0, 0, 0, 5, 5, 5, 5, 
	5, 5, 5, 5, 5, 5, 5, 5, 
	5, 21, 21, 21, 21, 21, 21, 21, 
	21, 21, 21, 21, 21, 21, 21, 21, 
	21, 21, 40, 0, 63, 65, 67, 68, 
	72, 74, 76, 72, 79, 83, 83, 86, 
	86, 89, 89, 92, 92, 95, 95, 98, 
	98, 100, 72, 72, 72, 72, 72, 72, 
	67, 109, 72, 72, 72, 111, 113
};

static const int wikitext_start = 35;
static const int wikitext_first_final = 35;
static const int wikitext_error = 0;

static const int wikitext_en_main = 35;

#line 394 "wikitext_ragel.rl"


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
    
#line 349 "wikitext_ragel.c"
	{
	cs = wikitext_start;
	ts = 0;
	te = 0;
	act = 0;
	}
#line 436 "wikitext_ragel.rl"
    
#line 358 "wikitext_ragel.c"
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
	case 3:
#line 1 "wikitext_ragel.rl"
	{ts = p;}
	break;
#line 379 "wikitext_ragel.c"
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
#line 33 "wikitext_ragel.rl"
	{
        MARK();
    }
	break;
	case 1:
#line 38 "wikitext_ragel.rl"
	{
        out->code_point = *p & 0x7f;
    }
	break;
	case 4:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
	break;
	case 5:
#line 82 "wikitext_ragel.rl"
	{act = 3;}
	break;
	case 6:
#line 145 "wikitext_ragel.rl"
	{act = 11;}
	break;
	case 7:
#line 154 "wikitext_ragel.rl"
	{act = 12;}
	break;
	case 8:
#line 264 "wikitext_ragel.rl"
	{act = 19;}
	break;
	case 9:
#line 365 "wikitext_ragel.rl"
	{act = 35;}
	break;
	case 10:
#line 70 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(NO_WIKI_START);
            {p++; goto _out; }
        }}
	break;
	case 11:
#line 76 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(NO_WIKI_END);
            {p++; goto _out; }
        }}
	break;
	case 12:
#line 100 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(TT);
            {p++; goto _out; }
        }}
	break;
	case 13:
#line 106 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(TT_START);
            {p++; goto _out; }
        }}
	break;
	case 14:
#line 112 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(TT_END);
            {p++; goto _out; }
        }}
	break;
	case 15:
#line 119 "wikitext_ragel.rl"
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
	case 16:
#line 270 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(LINK_START);
            {p++; goto _out; }
        }}
	break;
	case 17:
#line 276 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(LINK_END);
            {p++; goto _out; }
        }}
	break;
	case 18:
#line 282 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(SEPARATOR);
            {p++; goto _out; }
        }}
	break;
	case 19:
#line 300 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(QUOT_ENTITY);
            {p++; goto _out; }
        }}
	break;
	case 20:
#line 306 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(AMP_ENTITY);
            {p++; goto _out; }
        }}
	break;
	case 21:
#line 312 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(NAMED_ENTITY);
            {p++; goto _out; }
        }}
	break;
	case 22:
#line 318 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(HEX_ENTITY);
            {p++; goto _out; }
        }}
	break;
	case 23:
#line 324 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(DECIMAL_ENTITY);
            {p++; goto _out; }
        }}
	break;
	case 24:
#line 330 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(QUOT);
            {p++; goto _out; }
        }}
	break;
	case 25:
#line 354 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(CRLF);
            out->column_stop = 1;
            out->line_stop++;
            {p++; goto _out; }
        }}
	break;
	case 26:
#line 385 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(DEFAULT);
            out->column_stop = out->column_start + 1;
            {p++; goto _out; }
        }}
	break;
	case 27:
#line 88 "wikitext_ragel.rl"
	{te = p;p--;{
            EMIT(STRONG);
            {p++; goto _out; }
        }}
	break;
	case 28:
#line 94 "wikitext_ragel.rl"
	{te = p;p--;{
            EMIT(EM);
            {p++; goto _out; }
        }}
	break;
	case 29:
#line 119 "wikitext_ragel.rl"
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
	case 30:
#line 133 "wikitext_ragel.rl"
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
	case 31:
#line 163 "wikitext_ragel.rl"
	{te = p;p--;{
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
            {p++; goto _out; }
        }}
	break;
	case 32:
#line 180 "wikitext_ragel.rl"
	{te = p;p--;{
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
            {p++; goto _out; }
        }}
	break;
	case 33:
#line 196 "wikitext_ragel.rl"
	{te = p;p--;{
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
            {p++; goto _out; }
        }}
	break;
	case 34:
#line 213 "wikitext_ragel.rl"
	{te = p;p--;{
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
            {p++; goto _out; }
        }}
	break;
	case 35:
#line 230 "wikitext_ragel.rl"
	{te = p;p--;{
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
            {p++; goto _out; }
        }}
	break;
	case 36:
#line 247 "wikitext_ragel.rl"
	{te = p;p--;{
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
            {p++; goto _out; }
        }}
	break;
	case 37:
#line 264 "wikitext_ragel.rl"
	{te = p;p--;{
            EMIT(URI);
            {p++; goto _out; }
        }}
	break;
	case 38:
#line 288 "wikitext_ragel.rl"
	{te = p;p--;{
            EMIT(EXT_LINK_START);
            {p++; goto _out; }
        }}
	break;
	case 39:
#line 294 "wikitext_ragel.rl"
	{te = p;p--;{
            EMIT(EXT_LINK_END);
            {p++; goto _out; }
        }}
	break;
	case 40:
#line 336 "wikitext_ragel.rl"
	{te = p;p--;{
            EMIT(AMP);
            {p++; goto _out; }
        }}
	break;
	case 41:
#line 342 "wikitext_ragel.rl"
	{te = p;p--;{
            EMIT(LESS);
            {p++; goto _out; }
        }}
	break;
	case 42:
#line 354 "wikitext_ragel.rl"
	{te = p;p--;{
            EMIT(CRLF);
            out->column_stop = 1;
            out->line_stop++;
            {p++; goto _out; }
        }}
	break;
	case 43:
#line 365 "wikitext_ragel.rl"
	{te = p;p--;{
            EMIT(PRINTABLE);
            {p++; goto _out; }
        }}
	break;
	case 44:
#line 264 "wikitext_ragel.rl"
	{{p = ((te))-1;}{
            EMIT(URI);
            {p++; goto _out; }
        }}
	break;
	case 45:
#line 336 "wikitext_ragel.rl"
	{{p = ((te))-1;}{
            EMIT(AMP);
            {p++; goto _out; }
        }}
	break;
	case 46:
#line 342 "wikitext_ragel.rl"
	{{p = ((te))-1;}{
            EMIT(LESS);
            {p++; goto _out; }
        }}
	break;
	case 47:
#line 1 "wikitext_ragel.rl"
	{	switch( act ) {
	case 3:
	{{p = ((te))-1;}
            EMIT(STRONG_EM);
            {p++; goto _out; }
        }
	break;
	case 11:
	{{p = ((te))-1;}
            if (out->column_start == 1 || last_token_type == OL || last_token_type == UL || last_token_type == BLOCKQUOTE)
                EMIT(OL);
            else
                EMIT(PRINTABLE);
            {p++; goto _out; }
        }
	break;
	case 12:
	{{p = ((te))-1;}
            if (out->column_start == 1 || last_token_type == OL || last_token_type == UL || last_token_type == BLOCKQUOTE)
                EMIT(UL);
            else
                EMIT(PRINTABLE);
            {p++; goto _out; }
        }
	break;
	case 19:
	{{p = ((te))-1;}
            EMIT(URI);
            {p++; goto _out; }
        }
	break;
	case 35:
	{{p = ((te))-1;}
            EMIT(PRINTABLE);
            {p++; goto _out; }
        }
	break;
	default: break;
	}
	}
	break;
#line 871 "wikitext_ragel.c"
		}
	}

_again:
	_acts = _wikitext_actions + _wikitext_to_state_actions[cs];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 ) {
		switch ( *_acts++ ) {
	case 2:
#line 1 "wikitext_ragel.rl"
	{ts = 0;}
	break;
#line 884 "wikitext_ragel.c"
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
#line 437 "wikitext_ragel.rl"
    if (cs == wikitext_error)
        rb_raise(eWikitextParserError, "failed before finding a token");
    else if (out->type == NO_TOKEN)
        rb_raise(eWikitextParserError, "failed to produce a token");
}
