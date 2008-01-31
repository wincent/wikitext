// Copyright 2007 Wincent Colaiuta
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

lexer grammar Wikitext;

options {
  filter = true;
  language = C;
}

tokens {
  // imaginary tokens for blocks which are inferred rather than explicitly marked up
  P;
  LI;
}

@members {

  // convenience macro for calling GETCHARPOSITIONINLINE using a more compact form
  #define COLUMN GETCHARPOSITIONINLINE()

  // convenience macro for pulling out the last-remembered token by accessing the userp field
  #define LAST_TOKEN ((ANTLR3_UINT32)ctx->pLexer->rec->userp)

}

// for simplicitly use just this syntax, not the literal HTML <pre></pre> tag syntax
PRE             : { COLUMN == 0 || LAST_TOKEN == BLOCKQUOTE }?=> ' ' ;

// nowiki is just like pre, but it is an inline element (not a block element) and doesn't use a teletype font
// even though it is intended for use to delimit a inline span, it can be (ab)used and contain newlines or other
// elements that would otherwise end the span
NO_WIKI_START   : '<' ('n' | 'N') ('o' | 'O') ('w' | 'W') ('i' | 'I') ('k' | 'K') ('i' | 'I') '>' ;
NO_WIKI_END     : '</' ('n' | 'N') ('o' | 'O') ('w' | 'W') ('i' | 'I') ('k' | 'K') ('i' | 'I') '>' ;

// only valid in first column or immediately nested inside a blockquote
BLOCKQUOTE      : { COLUMN == 0 || LAST_TOKEN == BLOCKQUOTE }?=> '>' ' '? ;

// again, for simplicity, don't scan for alternative forms here (em, strong, b, i etc)
STRONG_EM       : '\'' '\'' '\'' '\'' '\'' ;
STRONG          : '\'' '\'' '\'' ;
EM              : '\'' '\'' ;

// make backticks a synonym for this: eg `foobar` and <tt>foobar</tt> would be equivalent
TT_START        : '<' ('t' | 'T') ('t' | 'T') '>' ;
TT_END          : '</' ('t' | 'T') ('t' | 'T') '>' ;

// lists
// only valid in first column, immediately after another list token, or immediately nested inside a blockquote
OL              : { COLUMN == 0 || LAST_TOKEN == OL || LAST_TOKEN == UL || LAST_TOKEN == BLOCKQUOTE }?=> '#' ;
UL              : { COLUMN == 0 || LAST_TOKEN == UL || LAST_TOKEN == OL || LAST_TOKEN == BLOCKQUOTE }?=> '*' ;

fragment H6     : '======' ;
fragment H5     : '=====' ;
fragment H4     : '====' ;
fragment H3     : '===' ;
fragment H2     : '==' ;
fragment H1     : '=' ;

// only valid in first column or immediately nested inside a blockquote
H6_START        : { COLUMN == 0 || LAST_TOKEN == BLOCKQUOTE }?=> H6 ;
H5_START        : { COLUMN == 0 || LAST_TOKEN == BLOCKQUOTE }?=> H5 ;
H4_START        : { COLUMN == 0 || LAST_TOKEN == BLOCKQUOTE }?=> H4 ;
H3_START        : { COLUMN == 0 || LAST_TOKEN == BLOCKQUOTE }?=> H3 ;
H2_START        : { COLUMN == 0 || LAST_TOKEN == BLOCKQUOTE }?=> H2 ;
H1_START        : { COLUMN == 0 || LAST_TOKEN == BLOCKQUOTE }?=> H1 ;

// only valid if last thing on line
H6_END          : H6 { MARK(); } ' '* ( '\n' | '\r' | EOF ) { REWINDLAST(); } ;
H5_END          : H5 { MARK(); } ' '* ( '\n' | '\r' | EOF ) { REWINDLAST(); } ;
H4_END          : H4 { MARK(); } ' '* ( '\n' | '\r' | EOF ) { REWINDLAST(); } ;
H3_END          : H3 { MARK(); } ' '* ( '\n' | '\r' | EOF ) { REWINDLAST(); } ;
H2_END          : H2 { MARK(); } ' '* ( '\n' | '\r' | EOF ) { REWINDLAST(); } ;
H1_END          : H1 { MARK(); } ' '* ( '\n' | '\r' | EOF ) { REWINDLAST(); } ;

// the syntactic predicate is necessary here because without it on seeing SPECIAL_URI_CHARS,
// ANTLR will always predict SPECIAL_URI_CHARS followed by URI_CHARS;
// if it turns out that there are no following URI_CHARS then the entire rule will fail
URI               : (HTTP | FTP | SVN) URI_CHARS ((SPECIAL_URI_CHARS URI_CHARS)=> (SPECIAL_URI_CHARS URI_CHARS))*;

fragment
HTTP              : ('h' | 'H') ('t' | 'T') ('t' | 'T') ('p' | 'P') ('s' |'S')? '://' ;

fragment
FTP               : ('f' | 'F') ('t' | 'T') ('p' | 'P') '://' ;

fragment
SVN               : ('s' | 'S') ('v' | 'V') ('n' | 'N') '://' ;

fragment
URI_CHARS         : ('a'..'z' | 'A'..'Z' | '0'..'9' | '@' | '$' | '&' | '\'' | '(' | '*' | '+' | '=' | '\%' | '-' | '_' | '~' | '/' | '#')+ ;

// these special URI characters may only form part of the URI if followed by a non-special URI character
// runs of special characters are permitted provided they are followed by at least one non-special char
fragment
SPECIAL_URI_CHARS : (':' | '!' | ')' | ',' | ';' | '.' | '?')+ ;

// TODO: scan for email addresses (not necessarily RFC 2821 compliant)
// this will allow us to accept things like [mailto:user@example.com user@example.com]
// will need to define a CSS class for such links as well
//EMAIL             : ('m' | 'M') | ('a' | 'A') | ('i' | 'I') | ('l' | 'L') | ('t' | 'T') | ('o' | 'O') ':' USER '@' DOMAIN;
//fragment
//USER              : ('a'..'z' | 'A'..'Z' | '0'..'9' | '.' | '_' | '-')
//fragment
//DOMAIN            :

LINK_START      : '[[' ;
LINK_END        : ']]' ;
EXT_LINK_START  : '[' ;
EXT_LINK_END    : ']' ;
SEPARATOR       : '|' ;
SPACE           : ' ' ;

// named entities (eg &copy;, &frac14;)
NAMED_ENTITY    :  '&' ('a'..'z' | 'A'..'Z')+ ('0'..'9')* ';' ;

// numeric (Unicode) entities (eg &#9099;)

// http://www.w3.org/TR/html401/charset.html#h-5.3.1
// Numeric character references specify the code position of a character in the document character set. Numeric character references may take two forms:
// The syntax "&#D;", where D is a decimal number, refers to the ISO 10646 decimal character number D.
// The syntax "&#xH;" or "&#XH;", where H is a hexadecimal number, refers to the ISO 10646 hexadecimal character number H. Hexadecimal numbers in numeric character references are case-insensitive.
HEX_ENTITY      : '&#' ('x' | 'X') ('a'..'f' | 'A'..'F' | '0'..'9')+ ';' ;

DECIMAL_ENTITY  : '&#' '0'..'9'+ ';' ;

// Here in the entity rules we are only concerned with syntactic validity; (may) worry about semantic validity in the parser.
// we should probably capture the relevant bit (the bit afte the & or the &#) at scanning time, for later consideration
// maybe use return values? or can they only be used at parser level?
// in that case should I tokenize this separately and try to match entities in the parser instead of the lexer?

// ASCII entities
QUOT            : '"' ;
AMP             : '&' ;
LESS            : '<' ; // can't use LT as rule name here (would clash with ANTLR C runtime macro)
GREATER         : '>' ;

// TODO: non-ASCII entities (although not sure if i care about these as they tend to bloat the lexer a lot);
// this means that instead of emitting &euro; we'll emit &#x20ac;

CRLF            : ('\r'? '\n')=> '\r'? '\n'
                | '\r'
                ;

// catch-alls (everything else)

// all the printable ASCII characters (0x20 to 0x7E) excluding those explicitly covered elsewhere
PRINTABLE       : '\u0021'              // skip space (0x20) and quote (0x22)
                | '\u0023'..'\u0025'    // skip ampersand (0x26)
                | '\u0027'..'\u003B'
                | '\u003D'              // skip less than (0x3C) and greater than (0x3E)
                | '\u003F'..'\u007B'    // skip vertical bar (0x7C)
                | '\u007E'
                ;

// note that this range includes one non-UCS-2 char
// '\u0000'..'\uFFFE'
DEFAULT         : . ; // these are either non-printable ASCII, or non-ASCII: will be converted to numeric entities

// http://www.w3.org/TR/html401/sgml/entities.html
// Character 65533 (FFFD hexadecimal) is the last valid character in UCS-2.
// 65534 (FFFE hexadecimal) is unassigned and reserved as the byte-swapped version of ZERO WIDTH NON-BREAKING SPACE for byte-order detection purposes.
// 65535 (FFFF hexadecimal) is unassigned.
