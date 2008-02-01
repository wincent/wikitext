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
#include <stdio.h>

#define EMIT(t) do { token.type = t; token.column_stop += (pe - p); } while (0)


#line 29 "wikitext_ragel.c"
static const char _wikitext_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 1, 4, 1, 5, 1, 6, 1, 
	7, 1, 8, 1, 9, 1, 10, 1, 
	11, 1, 12, 1, 13, 1, 14, 1, 
	15, 1, 16
};

static const char _wikitext_key_offsets[] = {
	0, 0, 1, 2, 7, 11, 13, 15, 
	17, 19, 21, 22, 24, 25, 27, 29, 
	31, 33, 35, 36, 38, 39, 46, 47, 
	48, 49
};

static const char _wikitext_trans_keys[] = {
	39, 39, 47, 78, 84, 110, 116, 78, 
	84, 110, 116, 79, 111, 87, 119, 73, 
	105, 75, 107, 73, 105, 62, 84, 116, 
	62, 79, 111, 87, 119, 73, 105, 75, 
	107, 73, 105, 62, 84, 116, 62, 10, 
	13, 32, 39, 60, 62, 96, 10, 39, 
	39, 32, 0
};

static const char _wikitext_single_lengths[] = {
	0, 1, 1, 5, 4, 2, 2, 2, 
	2, 2, 1, 2, 1, 2, 2, 2, 
	2, 2, 1, 2, 1, 7, 1, 1, 
	1, 1
};

static const char _wikitext_range_lengths[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0
};

static const char _wikitext_index_offsets[] = {
	0, 0, 2, 4, 10, 15, 18, 21, 
	24, 27, 30, 32, 35, 37, 40, 43, 
	46, 49, 52, 54, 57, 59, 67, 69, 
	71, 73
};

static const char _wikitext_trans_targs_wi[] = {
	23, 0, 21, 21, 4, 13, 19, 13, 
	19, 0, 5, 11, 5, 11, 0, 6, 
	6, 0, 7, 7, 0, 8, 8, 0, 
	9, 9, 0, 10, 10, 0, 21, 0, 
	12, 12, 0, 21, 0, 14, 14, 0, 
	15, 15, 0, 16, 16, 0, 17, 17, 
	0, 18, 18, 0, 21, 0, 20, 20, 
	0, 21, 0, 21, 22, 21, 1, 3, 
	25, 21, 0, 21, 21, 24, 21, 2, 
	21, 21, 21, 0
};

static const char _wikitext_trans_actions_wi[] = {
	0, 0, 11, 33, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 9, 0, 
	0, 0, 0, 17, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 7, 0, 0, 0, 
	0, 15, 0, 23, 0, 21, 0, 0, 
	0, 13, 0, 23, 31, 5, 27, 0, 
	25, 19, 29, 0
};

static const char _wikitext_to_state_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 1, 0, 0, 
	0, 0
};

static const char _wikitext_from_state_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 3, 0, 0, 
	0, 0
};

static const char _wikitext_eof_trans[] = {
	0, 0, 3, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 33, 34, 
	36, 38
};

static const int wikitext_start = 21;
static const int wikitext_first_final = 21;
static const int wikitext_error = 0;

static const int wikitext_en_main = 21;

#line 196 "wikitext_ragel.rl"


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
    
#line 177 "wikitext_ragel.c"
	{
	cs = wikitext_start;
	ts = 0;
	te = 0;
	act = 0;
	}
#line 243 "wikitext_ragel.rl"
    
#line 186 "wikitext_ragel.c"
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
	case 1:
#line 1 "wikitext_ragel.rl"
	{ts = p;}
	break;
#line 207 "wikitext_ragel.c"
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
	case 2:
#line 1 "wikitext_ragel.rl"
	{te = p+1;}
	break;
	case 3:
#line 34 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(NO_WIKI_START);
            {p++; goto _out; }
        }}
	break;
	case 4:
#line 40 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(NO_WIKI_END);
            {p++; goto _out; }
        }}
	break;
	case 5:
#line 46 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(STRONG_EM);
            {p++; goto _out; }
        }}
	break;
	case 6:
#line 64 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(TT);
            {p++; goto _out; }
        }}
	break;
	case 7:
#line 70 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(TT_START);
            {p++; goto _out; }
        }}
	break;
	case 8:
#line 76 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(TT_END);
            {p++; goto _out; }
        }}
	break;
	case 9:
#line 83 "wikitext_ragel.rl"
	{te = p+1;{
            if (token.column_start == 1 || last_token_type == BLOCKQUOTE)
                EMIT(BLOCKQUOTE);
            else
            {
                EMIT(GREATER);
                token.stop = token.start + 1;
                token.column_stop++;
            }
            {p++; goto _out; }
        }}
	break;
	case 10:
#line 98 "wikitext_ragel.rl"
	{te = p+1;{
            if (token.column_start == 1)
                EMIT(PRE);
            else
                EMIT(SPACE);
            {p++; goto _out; }
        }}
	break;
	case 11:
#line 107 "wikitext_ragel.rl"
	{te = p+1;{
            EMIT(CRLF);
            token.column_stop = 1;
            token.line_stop++;
            {p++; goto _out; }
        }}
	break;
	case 12:
#line 52 "wikitext_ragel.rl"
	{te = p;p--;{
            EMIT(STRONG);
            {p++; goto _out; }
        }}
	break;
	case 13:
#line 58 "wikitext_ragel.rl"
	{te = p;p--;{
            EMIT(EM);
            {p++; goto _out; }
        }}
	break;
	case 14:
#line 83 "wikitext_ragel.rl"
	{te = p;p--;{
            if (token.column_start == 1 || last_token_type == BLOCKQUOTE)
                EMIT(BLOCKQUOTE);
            else
            {
                EMIT(GREATER);
                token.stop = token.start + 1;
                token.column_stop++;
            }
            {p++; goto _out; }
        }}
	break;
	case 15:
#line 107 "wikitext_ragel.rl"
	{te = p;p--;{
            EMIT(CRLF);
            token.column_stop = 1;
            token.line_stop++;
            {p++; goto _out; }
        }}
	break;
	case 16:
#line 52 "wikitext_ragel.rl"
	{{p = ((te))-1;}{
            EMIT(STRONG);
            {p++; goto _out; }
        }}
	break;
#line 395 "wikitext_ragel.c"
		}
	}

_again:
	_acts = _wikitext_actions + _wikitext_to_state_actions[cs];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 ) {
		switch ( *_acts++ ) {
	case 0:
#line 1 "wikitext_ragel.rl"
	{ts = 0;}
	break;
#line 408 "wikitext_ragel.c"
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
#line 244 "wikitext_ragel.rl"
    if (cs == wikitext_error)
        rb_raise(eWikitextError, "failed before finding a token");
    else if (token.type == NO_TOKEN)
        rb_raise(eWikitextError, "failed to produce a token");
    else
        token.stop = p; // token.type already filled out in action
    return token;
}
