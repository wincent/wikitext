// Copyright 2007-2013 Wincent Colaiuta. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#include <stdbool.h>

#include "parser.h"
#include "ary.h"
#include "str.h"
#include "wikitext.h"
#include "wikitext_ragel.h"

#define IN(type) ary_includes(parser->scope, type)
#define IN_EITHER_OF(type1, type2) ary_includes2(parser->scope, type1, type2)
#define IN_ANY_OF(type1, type2, type3) ary_includes3(parser->scope, type1, type2, type3)

// output styles
enum { HTML_OUTPUT, XML_OUTPUT };

// poor man's object orientation in C:
// instead of passing around multiple parameters between functions in the parser
// we pack everything into a struct and pass around only a pointer to that
typedef struct
{
    str_t   *capture;               // capturing to link_target, link_text, or NULL (direct to output, not capturing)
    str_t   *output;                // for accumulating output to be returned
    str_t   *link_target;           // short term "memory" for parsing links
    str_t   *link_text;             // short term "memory" for parsing links
    str_t   *line_ending;
    str_t   *tabulation;            // caching buffer for emitting indentation
    ary_t   *scope;                 // stack for tracking scope
    ary_t   *line;                  // stack for tracking scope as implied by current line
    ary_t   *line_buffer;           // stack for tracking raw tokens (not scope) on current line
    VALUE   external_link_class;    // CSS class applied to external links
    VALUE   external_link_rel;      // rel attribute applied to external links
    VALUE   mailto_class;           // CSS class applied to email (mailto) links
    VALUE   img_prefix;             // path prepended when emitting img tags
    int     output_style;           // HTML_OUTPUT (default) or XML_OUTPUT
    int     base_indent;            // controlled by the :indent option to Wikitext::Parser#parse
    int     current_indent;         // fluctuates according to currently nested structures
    int     base_heading_level;
    bool    pending_crlf;
    bool    autolink;
    bool    space_to_underscore;
} parser_t;

const char null_str[]                   = { 0 };
const char escaped_no_wiki_start[]      = "&lt;nowiki&gt;";
const char escaped_no_wiki_end[]        = "&lt;/nowiki&gt;";
const char literal_strong_em[]          = "'''''";
const char literal_strong[]             = "'''";
const char literal_em[]                 = "''";
const char escaped_em_start[]           = "&lt;em&gt;";
const char escaped_em_end[]             = "&lt;/em&gt;";
const char escaped_strong_start[]       = "&lt;strong&gt;";
const char escaped_strong_end[]         = "&lt;/strong&gt;";
const char escaped_tt_start[]           = "&lt;tt&gt;";
const char escaped_tt_end[]             = "&lt;/tt&gt;";
const char pre_start[]                  = "<pre>";
const char pre_end[]                    = "</pre>";
const char escaped_pre_start[]          = "&lt;pre&gt;";
const char escaped_pre_end[]            = "&lt;/pre&gt;";
const char blockquote_start[]           = "<blockquote>";
const char blockquote_end[]             = "</blockquote>";
const char escaped_blockquote_start[]   = "&lt;blockquote&gt;";
const char escaped_blockquote_end[]     = "&lt;/blockquote&gt;";
const char strong_em_start[]            = "<strong><em>";
const char strong_start[]               = "<strong>";
const char strong_end[]                 = "</strong>";
const char em_start[]                   = "<em>";
const char em_end[]                     = "</em>";
const char code_start[]                 = "<code>";
const char code_end[]                   = "</code>";
const char ol_start[]                   = "<ol>";
const char ol_end[]                     = "</ol>";
const char ul_start[]                   = "<ul>";
const char ul_end[]                     = "</ul>";
const char li_start[]                   = "<li>";
const char li_end[]                     = "</li>";
const char h6_start[]                   = "<h6>";
const char h6_end[]                     = "</h6>";
const char h5_start[]                   = "<h5>";
const char h5_end[]                     = "</h5>";
const char h4_start[]                   = "<h4>";
const char h4_end[]                     = "</h4>";
const char h3_start[]                   = "<h3>";
const char h3_end[]                     = "</h3>";
const char h2_start[]                   = "<h2>";
const char h2_end[]                     = "</h2>";
const char h1_start[]                   = "<h1>";
const char h1_end[]                     = "</h1>";
const char p_start[]                    = "<p>";
const char p_end[]                      = "</p>";
const char space[]                      = " ";
const char a_start[]                    = "<a href=\"";
const char a_class[]                    = "\" class=\"";
const char a_rel[]                      = "\" rel=\"";
const char a_start_close[]              = "\">";
const char a_end[]                      = "</a>";
const char link_start[]                 = "[[";
const char link_end[]                   = "]]";
const char separator[]                  = "|";
const char ext_link_start[]             = "[";
const char backtick[]                   = "`";
const char quote[]                      = "\"";
const char ampersand[]                  = "&";
const char quot_entity[]                = "&quot;";
const char amp_entity[]                 = "&amp;";
const char lt_entity[]                  = "&lt;";
const char gt_entity[]                  = "&gt;";
const char escaped_blockquote[]         = "&gt; ";
const char ext_link_end[]               = "]";
const char literal_img_start[]          = "{{";
const char img_start[]                  = "<img src=\"";
const char img_end_xml[]                = "\" />";
const char img_end_html[]               = "\">";
const char img_alt[]                    = "\" alt=\"";
const char pre_class_start[]            = "<pre class=\"";
const char pre_class_end[]              = "-syntax\">";

// Mark the parser struct designated by ptr as a participant in Ruby's
// mark-and-sweep garbage collection scheme. A variable named name is placed on
// the C stack to prevent the structure from being prematurely collected.
#define GC_WRAP_PARSER(ptr, name) volatile VALUE name __attribute__((unused)) = Data_Wrap_Struct(rb_cObject, 0, parser_free, ptr)

parser_t *parser_new(void)
{
    parser_t *parser                = ALLOC_N(parser_t, 1);
    parser->capture                 = NULL; // not a real instance, pointer to other member's instance
    parser->output                  = str_new();
    parser->link_target             = str_new();
    parser->link_text               = str_new();
    parser->line_ending             = NULL; // caller should set up
    parser->tabulation              = str_new();
    parser->scope                   = ary_new();
    parser->line                    = ary_new();
    parser->line_buffer             = ary_new();
    parser->external_link_class     = Qnil; // caller should set up
    parser->external_link_rel       = Qnil; // caller should set up
    parser->mailto_class            = Qnil; // caller should set up
    parser->img_prefix              = Qnil; // caller should set up
    parser->output_style            = HTML_OUTPUT;
    parser->base_indent             = 0;
    parser->current_indent          = 0;
    parser->base_heading_level      = 0;
    parser->pending_crlf            = false;
    parser->autolink                = true;
    parser->space_to_underscore     = true;
    return parser;
}

void parser_free(parser_t *parser)
{
    // we don't free parser->capture; it's just a redundant pointer
    if (parser->output)         str_free(parser->output);
    if (parser->link_target)    str_free(parser->link_target);
    if (parser->link_text)      str_free(parser->link_text);
    if (parser->line_ending)    str_free(parser->line_ending);
    if (parser->tabulation)     str_free(parser->tabulation);
    if (parser->scope)          ary_free(parser->scope);
    if (parser->line)           ary_free(parser->line);
    if (parser->line_buffer)    ary_free(parser->line_buffer);
    free(parser);
}

// for testing and debugging only
VALUE Wikitext_parser_tokenize(VALUE self, VALUE string)
{
    if (NIL_P(string))
        return Qnil;
    string = StringValue(string);
    VALUE tokens = rb_ary_new();
    char *p = RSTRING_PTR(string);
    long len = RSTRING_LEN(string);
    char *pe = p + len;
    token_t token;
    next_token(&token, NULL, p, pe);
    rb_ary_push(tokens, wiki_token(&token));
    while (token.type != END_OF_FILE)
    {
        next_token(&token, &token, NULL, pe);
        rb_ary_push(tokens, wiki_token(&token));
    }
    return tokens;
}

// for benchmarking raw tokenization speed only
VALUE Wikitext_parser_benchmarking_tokenize(VALUE self, VALUE string)
{
    if (NIL_P(string))
        return Qnil;
    string = StringValue(string);
    char *p = RSTRING_PTR(string);
    long len = RSTRING_LEN(string);
    char *pe = p + len;
    token_t token;
    next_token(&token, NULL, p, pe);
    while (token.type != END_OF_FILE)
        next_token(&token, &token, NULL, pe);
    return Qnil;
}

VALUE Wikitext_parser_fulltext_tokenize(int argc, VALUE *argv, VALUE self)
{
    // process arguments
    VALUE string, options;
    if (rb_scan_args(argc, argv, "11", &string, &options) == 1) // 1 mandatory argument, 1 optional argument
        options = Qnil;
    if (NIL_P(string))
        return Qnil;
    string = StringValue(string);
    VALUE tokens = rb_ary_new();

    // check instance variables
    VALUE min = rb_iv_get(self, "@minimum_fulltext_token_length");

    // process options hash (can override instance variables)
    if (!NIL_P(options) && TYPE(options) == T_HASH)
    {
        if (rb_funcall(options, rb_intern("has_key?"), 1, ID2SYM(rb_intern("minimum"))) == Qtrue)
            min = rb_hash_aref(options, ID2SYM(rb_intern("minimum")));
    }
    int min_len = NIL_P(min) ? 3 : NUM2INT(min);
    if (min_len < 0)
        min_len = 0;

    // set up scanner
    char *p = RSTRING_PTR(string);
    long len = RSTRING_LEN(string);
    char *pe = p + len;
    token_t token;
    token_t *_token = &token;
    next_token(&token, NULL, p, pe);
    while (token.type != END_OF_FILE)
    {
        switch (token.type)
        {
            case URI:
            case MAIL:
            case ALNUM:
                if (TOKEN_LEN(_token) >= min_len)
                    rb_ary_push(tokens, TOKEN_TEXT(_token));
                break;
            default:
                // ignore everything else
                break;
        }
        next_token(&token, &token, NULL, pe);
    }
    return tokens;
}

// we downcase "in place", overwriting the original contents of the buffer
void wiki_downcase_bang(char *ptr, long len)
{
    for (long i = 0; i < len; i++)
    {
        if (ptr[i] >= 'A' && ptr[i] <= 'Z')
            ptr[i] += 32;
    }
}

void wiki_append_entity_from_utf32_char(str_t *output, uint32_t character)
{
    char hex_string[8]  = { '&', '#', 'x', 0, 0, 0, 0, ';' };
    char scratch        = (character & 0xf000) >> 12;
    hex_string[3]       = (scratch <= 9 ? scratch + 48 : scratch + 87);
    scratch             = (character & 0x0f00) >> 8;
    hex_string[4]       = (scratch <= 9 ? scratch + 48 : scratch + 87);
    scratch             = (character & 0x00f0) >> 4;
    hex_string[5]       = (scratch <= 9 ? scratch + 48 : scratch + 87);
    scratch             = character & 0x000f;
    hex_string[6]       = (scratch <= 9 ? scratch + 48 : scratch + 87);
    str_append(output, hex_string, sizeof(hex_string));
}

// Convert a single UTF-8 codepoint to UTF-32
//
// Expects an input buffer, src, containing a UTF-8 encoded character (which
// may be multi-byte). The end of the input buffer, end, is also passed in to
// allow the detection of invalidly truncated codepoints. The number of bytes
// in the UTF-8 character (between 1 and 4) is returned by reference in
// width_out.
//
// Raises a RangeError if the supplied character is invalid UTF-8.
uint32_t wiki_utf8_to_utf32(char *src, char *end, long *width_out)
{
    uint32_t dest = 0;
    if ((unsigned char)src[0] <= 0x7f)
    {
        // ASCII
        dest = src[0];
        *width_out = 1;
    }
    else if ((src[0] & 0xe0) == 0xc0)
    {
        // byte starts with 110..... : this should be a two-byte sequence
        if (src + 1 >= end)
            // no second byte
            rb_raise(eWikitextParserError, "invalid encoding: truncated byte sequence");
        else if (((unsigned char)src[0] == 0xc0) ||
                ((unsigned char)src[0] == 0xc1))
            // overlong encoding: lead byte of 110..... but code point <= 127
            rb_raise(eWikitextParserError, "invalid encoding: overlong encoding");
        else if ((src[1] & 0xc0) != 0x80 )
            // should have second byte starting with 10......
            rb_raise(eWikitextParserError, "invalid encoding: malformed byte sequence");

        dest =
            ((uint32_t)(src[0] & 0x1f)) << 6 |
            (src[1] & 0x3f);
        *width_out = 2;
    }
    else if ((src[0] & 0xf0) == 0xe0)
    {
        // byte starts with 1110.... : this should be a three-byte sequence
        if (src + 2 >= end)
            // missing second or third byte
            rb_raise(eWikitextParserError, "invalid encoding: truncated byte sequence");
        else if (((src[1] & 0xc0) != 0x80 ) ||
                ((src[2] & 0xc0) != 0x80 ))
            // should have second and third bytes starting with 10......
            rb_raise(eWikitextParserError, "invalid encoding: malformed byte sequence");

        dest =
            ((uint32_t)(src[0] & 0x0f)) << 12 |
            ((uint32_t)(src[1] & 0x3f)) << 6 |
            (src[2] & 0x3f);
        *width_out = 3;
    }
    else if ((src[0] & 0xf8) == 0xf0)
    {
        // bytes starts with 11110... : this should be a four-byte sequence
        if (src + 3 >= end)
            // missing second, third, or fourth byte
            rb_raise(eWikitextParserError, "invalid encoding: truncated byte sequence");
        else if ((unsigned char)src[0] >= 0xf5 &&
                (unsigned char)src[0] <= 0xf7)
            // disallowed by RFC 3629 (codepoints above 0x10ffff)
            rb_raise(eWikitextParserError, "invalid encoding: overlong encoding");
        else if (((src[1] & 0xc0) != 0x80 ) ||
                ((src[2] & 0xc0) != 0x80 ) ||
                ((src[3] & 0xc0) != 0x80 ))
            // should have second and third bytes starting with 10......
            rb_raise(eWikitextParserError, "invalid encoding: malformed byte sequence");

        dest =
            ((uint32_t)(src[0] & 0x07)) << 18 |
            ((uint32_t)(src[1] & 0x3f)) << 12 |
            ((uint32_t)(src[1] & 0x3f)) << 6 |
            (src[2] & 0x3f);
        *width_out = 4;
    }
    else
        rb_raise(eWikitextParserError, "invalid encoding: unexpected byte");
    return dest;
}

// - non-printable (non-ASCII) characters converted to numeric entities
// - QUOT and AMP characters converted to named entities
// - if trim is true, leading and trailing whitespace trimmed
// - if trim is false, there is no special treatment of spaces
void wiki_append_sanitized_link_target(str_t *link_target, str_t *output, bool trim)
{
    char    *src        = link_target->ptr;
    char    *start      = src;                          // remember this so we can check if we're at the start
    char    *non_space  = output->ptr + output->len;    // remember last non-space character output
    char    *end        = src + link_target->len;
    while (src < end)
    {
        // need at most 8 bytes to display each input character (&#x0000;)
        if (output->ptr + output->len + 8 > output->ptr + output->capacity) // outgrowing buffer, must grow
        {
            char *old_ptr = output->ptr;
            str_grow(output, output->len + (end - src) * 8);    // allocate enough for worst case
            if (old_ptr != output->ptr) // may have moved
                non_space += output->ptr - old_ptr;
        }

        if (*src == '"')
        {
            char quot_entity_literal[] = { '&', 'q', 'u', 'o', 't', ';' };  // no trailing NUL
            str_append(output, quot_entity_literal, sizeof(quot_entity_literal));
        }
        else if (*src == '&')
        {
            char amp_entity_literal[] = { '&', 'a', 'm', 'p', ';' };    // no trailing NUL
            str_append(output, amp_entity_literal, sizeof(amp_entity_literal));
        }
        else if (*src == '<' || *src == '>')
            rb_raise(rb_eRangeError, "invalid link text (\"%c\" may not appear in link text)", *src);
        else if (*src == ' ' && src == start && trim)
            start++;                            // we eat leading space
        else if (*src >= 0x20 && *src <= 0x7e)  // printable ASCII
        {
            *(output->ptr + output->len) = *src;
            output->len++;
        }
        else    // all others: must convert to entities
        {
            long        width;
            wiki_append_entity_from_utf32_char(output, wiki_utf8_to_utf32(src, end, &width));
            src         += width;
            non_space   = output->ptr + output->len;
            continue;
        }
        if (*src != ' ')
            non_space = output->ptr + output->len;
        src++;
    }

    // trim trailing space if necessary
    if (trim && output->ptr + output->len != non_space)
        output->len -= (output->ptr + output->len) - non_space;
}

// prepare hyperlink and append it to parser->output
// if check_autolink is true, checks parser->autolink to decide whether to emit a real hyperlink
// or merely the literal link target
// if link_text is Qnil, the link_target is re-used for the link text
void wiki_append_hyperlink(parser_t *parser, VALUE link_prefix, str_t *link_target, str_t *link_text, VALUE link_class, VALUE link_rel, bool check_autolink)
{
    if (check_autolink && !parser->autolink)
        wiki_append_sanitized_link_target(link_target, parser->output, true);
    else
    {
        str_append(parser->output, a_start, sizeof(a_start) - 1);               // <a href="
        if (!NIL_P(link_prefix))
            str_append_string(parser->output, link_prefix);
        wiki_append_sanitized_link_target(link_target, parser->output, true);

        // special handling for mailto URIs
        const char *mailto = "mailto:";
        long mailto_len = (long)sizeof(mailto) - 1; // don't count NUL byte
        if ((link_target->len >= mailto_len &&
             strncmp(mailto, link_target->ptr, mailto_len) == 0) ||
            (!NIL_P(link_prefix) &&
             RSTRING_LEN(link_prefix) >= mailto_len &&
             strncmp(mailto, RSTRING_PTR(link_prefix), mailto_len) == 0))
            link_class = parser->mailto_class; // use mailto_class from parser
        if (link_class != Qnil)
        {
            str_append(parser->output, a_class, sizeof(a_class) - 1);           // " class="
            str_append_string(parser->output, link_class);
        }
        if (link_rel != Qnil)
        {
            str_append(parser->output, a_rel, sizeof(a_rel) - 1);               // " rel="
            str_append_string(parser->output, link_rel);
        }
        str_append(parser->output, a_start_close, sizeof(a_start_close) - 1);   // ">
        if (!link_text || link_text->len == 0) // re-use link_target
            wiki_append_sanitized_link_target(link_target, parser->output, true);
        else
            str_append_str(parser->output, link_text);
        str_append(parser->output, a_end, sizeof(a_end) - 1);                   // </a>
    }
}

void wiki_append_img(parser_t *parser, char *token_ptr, long token_len)
{
    str_append(parser->output, img_start, sizeof(img_start) - 1);           // <img src="
    if (!NIL_P(parser->img_prefix) && *token_ptr != '/')                    // len always > 0
        str_append_string(parser->output, parser->img_prefix);
    str_append(parser->output, token_ptr, token_len);
    str_append(parser->output, img_alt, sizeof(img_alt) - 1);               // " alt="
    str_append(parser->output, token_ptr, token_len);
    if (parser->output_style == XML_OUTPUT)
        str_append(parser->output, img_end_xml, sizeof(img_end_xml) - 1);   // " />
    else
        str_append(parser->output, img_end_html, sizeof(img_end_html) - 1); // ">
}

// will emit indentation only if we are about to emit any of:
//      <blockquote>, <p>, <ul>, <ol>, <li>, <h1> etc, <pre>
// each time we enter one of those spans must ++ the indentation level
void wiki_indent(parser_t *parser)
{
    if (parser->base_indent == -1) // indentation disabled
        return;
    int space_count = parser->current_indent + parser->base_indent;
    if (space_count > 0)
    {
        char *old_end, *new_end;
        if (parser->tabulation->len < space_count)
            str_grow(parser->tabulation, space_count); // reallocates if necessary
        old_end = parser->tabulation->ptr + parser->tabulation->len;
        new_end = parser->tabulation->ptr + space_count;
        while (old_end < new_end)
            *old_end++ = ' ';
        if (space_count > parser->tabulation->len)
            parser->tabulation->len = space_count;
        str_append(parser->output, parser->tabulation->ptr, space_count);
    }
    parser->current_indent += 2;
}

void wiki_append_pre_start(parser_t *parser, token_t *token)
{
    wiki_indent(parser);
    if ((size_t)TOKEN_LEN(token) > sizeof(pre_start) - 1)
    {
        str_append(parser->output, pre_class_start, sizeof(pre_class_start) - 1);   // <pre class="
        str_append(parser->output, token->start + 11, TOKEN_LEN(token) - 13);       // (the "lang" substring)
        str_append(parser->output, pre_class_end, sizeof(pre_class_end) - 1);       // -syntax">
    }
    else
        str_append(parser->output, pre_start, sizeof(pre_start) - 1);
    ary_push(parser->scope, PRE_START);
    ary_push(parser->line, PRE_START);
}

void wiki_dedent(parser_t *parser, bool emit)
{
    if (parser->base_indent == -1) // indentation disabled
        return;
    parser->current_indent -= 2;
    if (!emit)
        return;
    int space_count = parser->current_indent + parser->base_indent;
    if (space_count > 0)
        str_append(parser->output, parser->tabulation->ptr, space_count);
}

// Pops a single item off the parser's scope stack.
// A corresponding closing tag is written to the target string.
// The target string may be the main output buffer, or a substring capturing buffer if a link is being scanned.
void wiki_pop_from_stack(parser_t *parser, str_t *target)
{
    int top = ary_entry(parser->scope, -1);
    if (NO_ITEM(top))
        return;
    if (!target)
        target = parser->output;

    // for headings, take base_heading_level into account
    if (top >= H1_START && top <= H6_START)
    {
        top += parser->base_heading_level;
        // no need to check for underflow (base_heading_level is never negative)
        if (top > H6_START)
            top = H6_START;
    }

    switch (top)
    {
        case PRE:
        case PRE_START:
            str_append(target, pre_end, sizeof(pre_end) - 1);
            str_append_str(target, parser->line_ending);
            wiki_dedent(parser, false);
            break;

        case BLOCKQUOTE:
        case BLOCKQUOTE_START:
            wiki_dedent(parser, true);
            str_append(target, blockquote_end, sizeof(blockquote_end) - 1);
            str_append_str(target, parser->line_ending);
            break;

        case NO_WIKI_START:
            // not a real HTML tag; so nothing to pop
            break;

        case STRONG:
        case STRONG_START:
            str_append(target, strong_end, sizeof(strong_end) - 1);
            break;

        case EM:
        case EM_START:
            str_append(target, em_end, sizeof(em_end) - 1);
            break;

        case TT:
        case TT_START:
            str_append(target, code_end, sizeof(code_end) - 1);
            break;

        case OL:
            wiki_dedent(parser, true);
            str_append(target, ol_end, sizeof(ol_end) - 1);
            str_append_str(target, parser->line_ending);
            break;

        case UL:
            wiki_dedent(parser, true);
            str_append(target, ul_end, sizeof(ul_end) - 1);
            str_append_str(target, parser->line_ending);
            break;

        case NESTED_LIST:
            // next token to pop will be a LI
            // LI is an interesting token because sometimes we want it to behave like P (ie. do a non-emitting indent)
            // and other times we want it to behave like BLOCKQUOTE (ie. when it has a nested list inside)
            // hence this hack: we do an emitting dedent on behalf of the LI that we know must be coming
            // and then when we pop the actual LI itself (below) we do the standard non-emitting indent
            wiki_dedent(parser, true);      // we really only want to emit the spaces
            parser->current_indent += 2;    // we don't want to decrement the actual indent level, so put it back
            break;

        case LI:
            str_append(target, li_end, sizeof(li_end) - 1);
            str_append_str(target, parser->line_ending);
            wiki_dedent(parser, false);
            break;

        case H6_START:
            str_append(target, h6_end, sizeof(h6_end) - 1);
            str_append_str(target, parser->line_ending);
            wiki_dedent(parser, false);
            break;

        case H5_START:
            str_append(target, h5_end, sizeof(h5_end) - 1);
            str_append_str(target, parser->line_ending);
            wiki_dedent(parser, false);
            break;

        case H4_START:
            str_append(target, h4_end, sizeof(h4_end) - 1);
            str_append_str(target, parser->line_ending);
            wiki_dedent(parser, false);
            break;

        case H3_START:
            str_append(target, h3_end, sizeof(h3_end) - 1);
            str_append_str(target, parser->line_ending);
            wiki_dedent(parser, false);
            break;

        case H2_START:
            str_append(target, h2_end, sizeof(h2_end) - 1);
            str_append_str(target, parser->line_ending);
            wiki_dedent(parser, false);
            break;

        case H1_START:
            str_append(target, h1_end, sizeof(h1_end) - 1);
            str_append_str(target, parser->line_ending);
            wiki_dedent(parser, false);
            break;

        case LINK_START:
            // not an HTML tag; so nothing to emit
            break;

        case EXT_LINK_START:
            // not an HTML tag; so nothing to emit
            break;

        case PATH:
            // not an HTML tag; so nothing to emit
            break;

        case SPACE:
            // not an HTML tag (only used to separate an external link target from the link text); so nothing to emit
            break;

        case SEPARATOR:
            // not an HTML tag (only used to separate an external link target from the link text); so nothing to emit
            break;

        case P:
            str_append(target, p_end, sizeof(p_end) - 1);
            str_append_str(target, parser->line_ending);
            wiki_dedent(parser, false);
            break;

        case END_OF_FILE:
            // nothing to do
            break;

        default:
            // should probably raise an exception here
            break;
    }
    ary_pop(parser->scope);
}

// Pops items off the top of parser's scope stack, accumulating closing tags for them into the target string, until item is reached.
// If including is true then the item itself is also popped.
// The target string may be the main output buffer, or a substring capturing buffer when scanning links.
void wiki_pop_from_stack_up_to(parser_t *parser, str_t *target, int item, bool including)
{
    int continue_looping = 1;
    do
    {
        int top = ary_entry(parser->scope, -1);
        if (NO_ITEM(top))
            return;
        if (top == item)
        {
            if (!including)
                return;
            continue_looping = 0;
        }
        wiki_pop_from_stack(parser, target);
    } while (continue_looping);
}

void wiki_pop_all_from_stack(parser_t *parser)
{
    for (int i = 0, max = parser->scope->count; i < max; i++)
        wiki_pop_from_stack(parser, NULL);
}

void wiki_start_para_if_necessary(parser_t *parser)
{
    if (parser->capture)
        return;

    // if no block open yet, or top of stack is BLOCKQUOTE/BLOCKQUOTE_START (with nothing in it yet)
    if (parser->scope->count == 0 ||
        ary_entry(parser->scope, -1) == BLOCKQUOTE ||
        ary_entry(parser->scope, -1) == BLOCKQUOTE_START)
    {
        wiki_indent(parser);
        str_append(parser->output, p_start, sizeof(p_start) - 1);
        ary_push(parser->scope, P);
        ary_push(parser->line, P);
    }
    else if (parser->pending_crlf)
    {
        if (IN(P))
            // already in a paragraph block; convert pending CRLF into a space
            str_append(parser->output, space, sizeof(space) - 1);
        else if (IN(PRE))
            // PRE blocks can have pending CRLF too (helps us avoid emitting the trailing newline)
            str_append_str(parser->output, parser->line_ending);
    }
    parser->pending_crlf = false;
}

void wiki_emit_pending_crlf_if_necessary(parser_t *parser)
{
    if (parser->pending_crlf)
    {
        str_append_str(parser->output, parser->line_ending);
        parser->pending_crlf = false;
    }
}

// Helper function that pops any excess elements off scope (pushing is already handled in the respective rules).
// For example, given input like:
//
//      > > foo
//      bar
//
// Upon seeing "bar", we want to pop two BLOCKQUOTE elements from the scope.
// The reverse case (shown below) is handled from inside the BLOCKQUOTE rule itself:
//
//      foo
//      > > bar
//
// Things are made slightly more complicated by the fact that there is one block-level tag that can be on the scope
// but not on the line scope:
//
//      <blockquote>foo
//      bar</blockquote>
//
// Here on seeing "bar" we have one item on the scope (BLOCKQUOTE_START) which we don't want to pop, but we have nothing
// on the line scope.
// Luckily, BLOCKQUOTE_START tokens can only appear at the start of the scope array, so we can check for them first before
// entering the for loop.
void wiki_pop_excess_elements(parser_t *parser)
{
    if (parser->capture)
        return;
    for (int i = parser->scope->count - ary_count(parser->scope, BLOCKQUOTE_START), j = parser->line->count; i > j; i--)
    {
        // special case for last item on scope
        if (i - j == 1)
        {
            // don't auto-pop P if it is only item on scope
            if (ary_entry(parser->scope, -1) == P)
            {
                // add P to the line scope to prevent us entering the loop at all next time around
                ary_push(parser->line, P);
                continue;
            }
        }
        wiki_pop_from_stack(parser, NULL);
    }
}

// trim parser->link_text in place
void wiki_trim_link_text(parser_t *parser)
{
    char    *src        = parser->link_text->ptr;
    char    *start      = src;                  // remember this so we can check if we're at the start
    char    *left       = src;
    char    *non_space  = src;                  // remember last non-space character output
    char    *end        = src + parser->link_text->len;
    while (src < end)
    {
        if (*src == ' ')
        {
            if (src == left)
                left++;
        }
        else
            non_space = src;
        src++;
    }
    if (left != start || non_space + 1 != end)
    {
        // TODO: could potentially avoid this memmove by extending the str_t struct with an "offset" or "free" member
        parser->link_text->len = (non_space + 1) - left;
        memmove(parser->link_text->ptr, left, parser->link_text->len);
    }
}

VALUE Wikitext_parser_sanitize_link_target(VALUE self, VALUE string)
{
    str_t *link_target = str_new_from_string(string);
    GC_WRAP_STR(link_target, link_target_gc);
    str_t *output = str_new();
    GC_WRAP_STR(output, output_gc);
    wiki_append_sanitized_link_target(link_target, output, true);
    return string_from_str(output);
}

// Encodes the parser link_target member (in-place) according to RFCs 2396 and 2718
//
// Leading and trailing whitespace trimmed. Spaces are converted to
// underscores if the parser space_to_underscore member is true.
static void wiki_encode_link_target(parser_t *parser)
{
    char        *src        = parser->link_target->ptr;
    char        *start      = src;  // remember this so we can check if we're at the start
    long        len         = parser->link_target->len;
    if (!(len > 0))
        return;
    char        *end        = src + len;
    long        dest_len    = len * 2;
    char        *dest       = ALLOC_N(char, dest_len);
    char        *dest_ptr   = dest; // hang on to this so we can pass it to free() later
    char        *non_space  = dest; // remember last non-space character output
    static char hex[]       = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
    for (; src < end; src++)
    {
        // worst case: a single character may grow to 3 characters once encoded
        if ((dest + 3) > (dest_ptr + dest_len))
        {
            // outgrowing buffer, must reallocate
            char *old_dest      = dest;
            char *old_dest_ptr  = dest_ptr;
            dest_len            += len;
            dest                = realloc(dest_ptr, dest_len);
            if (dest == NULL)
            {
                // would have used reallocf, but this has to run on Linux too, not just Darwin
                free(dest_ptr);
                rb_raise(rb_eNoMemError, "failed to re-allocate temporary storage (memory allocation error)");
            }
            dest_ptr    = dest;
            dest        = dest_ptr + (old_dest - old_dest_ptr);
            non_space   = dest_ptr + (non_space - old_dest_ptr);
        }

        // pass through unreserved characters
        if ((*src >= 'a' && *src <= 'z') ||
            (*src >= 'A' && *src <= 'Z') ||
            (*src >= '0' && *src <= '9') ||
            *src == '-' ||
            *src == '_' ||
            *src == '.' ||
            *src == '~')
        {
            *dest++     = *src;
            non_space   = dest;
        }
        else if (*src == ' ' && src == start)
            start++;                    // we eat leading space
        else if (*src == ' ' && parser->space_to_underscore)
            *dest++     = '_';
        else    // everything else gets URL-encoded
        {
            *dest++     = '%';
            *dest++     = hex[(unsigned char)(*src) / 16];   // left
            *dest++     = hex[(unsigned char)(*src) % 16];   // right
            if (*src != ' ')
                non_space = dest;
        }
    }

    // trim trailing space if necessary
    if (non_space > dest_ptr && dest != non_space)
        dest_len = non_space - dest_ptr;
    else
        dest_len = dest - dest_ptr;
    str_clear(parser->link_target);
    str_append(parser->link_target, dest_ptr, dest_len);
    free(dest_ptr);
}

VALUE Wikitext_parser_encode_link_target(VALUE self, VALUE in)
{
    parser_t parser;
    parser.space_to_underscore      = false;
    parser.link_target              = str_new_from_string(in);
    GC_WRAP_STR(parser.link_target, link_target_gc);
    wiki_encode_link_target(&parser);
    return string_from_str(parser.link_target);
}

// returns 1 (true) if supplied string is blank (nil, empty, or all whitespace)
// returns 0 (false) otherwise
bool wiki_blank(str_t *str)
{
    if (str->len == 0)
        return true;
    for (char *ptr = str->ptr,
        *end = str->ptr + str->len;
        ptr < end; ptr++)
    {
        if (*ptr != ' ')
            return false;
    }
    return true;
}

void wiki_rollback_failed_internal_link(parser_t *parser)
{
    if (!IN(LINK_START))
        return; // nothing to do!
    int scope_includes_separator = IN(SEPARATOR);
    wiki_pop_from_stack_up_to(parser, NULL, LINK_START, true);
    str_append(parser->output, link_start, sizeof(link_start) - 1);
    if (parser->link_target->len > 0)
    {
        wiki_append_sanitized_link_target(parser->link_target, parser->output, false);
        if (scope_includes_separator)
        {
            str_append(parser->output, separator, sizeof(separator) - 1);
            if (parser->link_text->len > 0)
                str_append_str(parser->output, parser->link_text);
        }
    }
    parser->capture = NULL;
    str_clear(parser->link_target);
    str_clear(parser->link_text);
}

void wiki_rollback_failed_external_link(parser_t *parser)
{
    if (!IN(EXT_LINK_START))
        return; // nothing to do!

    // store a couple of values before popping
    int scope_includes_space = IN(SPACE);
    VALUE link_class = IN(PATH) ? Qnil : parser->external_link_class;
    VALUE link_rel   = IN(PATH) ? Qnil : parser->external_link_rel;
    wiki_pop_from_stack_up_to(parser, NULL, EXT_LINK_START, true);

    str_append(parser->output, ext_link_start, sizeof(ext_link_start) - 1);
    if (parser->link_target->len > 0)
    {
        wiki_append_hyperlink(parser, Qnil, parser->link_target, NULL, link_class, link_rel, true);
        if (scope_includes_space)
        {
            str_append(parser->output, space, sizeof(space) - 1);
            if (parser->link_text->len > 0)
                str_append_str(parser->output, parser->link_text);
        }
    }
    parser->capture = NULL;
    str_clear(parser->link_target);
    str_clear(parser->link_text);
}

void wiki_rollback_failed_link(parser_t *parser)
{
    wiki_rollback_failed_internal_link(parser);
    wiki_rollback_failed_external_link(parser);
}

VALUE Wikitext_parser_initialize(int argc, VALUE *argv, VALUE self)
{
    // process arguments
    VALUE options;
    if (rb_scan_args(argc, argv, "01", &options) == 0) // 0 mandatory arguments, 1 optional argument
        options = Qnil;

    // defaults
    VALUE autolink                      = Qtrue;
    VALUE line_ending                   = rb_str_new2("\n");
    VALUE external_link_class           = rb_str_new2("external");
    VALUE external_link_rel             = Qnil;
    VALUE mailto_class                  = rb_str_new2("mailto");
    VALUE link_proc                     = Qnil;
    VALUE internal_link_prefix          = rb_str_new2("/wiki/");
    VALUE img_prefix                    = rb_str_new2("/images/");
    VALUE output_style                  = ID2SYM(rb_intern("html"));
    VALUE space_to_underscore           = Qtrue;
    VALUE minimum_fulltext_token_length = INT2NUM(3);
    VALUE base_heading_level            = INT2NUM(0);

    // process options hash (override defaults)
    if (!NIL_P(options) && TYPE(options) == T_HASH)
    {
#define OVERRIDE_IF_SET(name)   rb_funcall(options, rb_intern("has_key?"), 1, ID2SYM(rb_intern(#name))) == Qtrue ? \
                                rb_hash_aref(options, ID2SYM(rb_intern(#name))) : name
        autolink                        = OVERRIDE_IF_SET(autolink);
        line_ending                     = OVERRIDE_IF_SET(line_ending);
        external_link_class             = OVERRIDE_IF_SET(external_link_class);
        external_link_rel               = OVERRIDE_IF_SET(external_link_rel);
        mailto_class                    = OVERRIDE_IF_SET(mailto_class);
        link_proc                       = OVERRIDE_IF_SET(link_proc);
        internal_link_prefix            = OVERRIDE_IF_SET(internal_link_prefix);
        img_prefix                      = OVERRIDE_IF_SET(img_prefix);
        output_style                    = OVERRIDE_IF_SET(output_style);
        space_to_underscore             = OVERRIDE_IF_SET(space_to_underscore);
        minimum_fulltext_token_length   = OVERRIDE_IF_SET(minimum_fulltext_token_length);
        base_heading_level              = OVERRIDE_IF_SET(base_heading_level);
    }

    // no need to call super here; rb_call_super()
    rb_iv_set(self, "@autolink",                        autolink);
    rb_iv_set(self, "@line_ending",                     line_ending);
    rb_iv_set(self, "@external_link_class",             external_link_class);
    rb_iv_set(self, "@external_link_rel",               external_link_rel);
    rb_iv_set(self, "@mailto_class",                    mailto_class);
    rb_iv_set(self, "@link_proc",                       link_proc);
    rb_iv_set(self, "@internal_link_prefix",            internal_link_prefix);
    rb_iv_set(self, "@img_prefix",                      img_prefix);
    rb_iv_set(self, "@output_style",                    output_style);
    rb_iv_set(self, "@space_to_underscore",             space_to_underscore);
    rb_iv_set(self, "@minimum_fulltext_token_length",   minimum_fulltext_token_length);
    rb_iv_set(self, "@base_heading_level",              base_heading_level);
    return self;
}

VALUE Wikitext_parser_profiling_parse(VALUE self, VALUE string)
{
    for (int i = 0; i < 100000; i++)
        Wikitext_parser_parse(1, &string, self);
    return Qnil;
}

// convert a Ruby object (:xml, :html etc) into an int output style
int Wikitext_output_style(VALUE output)
{
    if (TYPE(output) == T_SYMBOL)
    {
        if (SYM2ID(output) == rb_intern("xml"))
            return XML_OUTPUT;
    }
    return HTML_OUTPUT; // fall back to default
}

VALUE Wikitext_parser_parse(int argc, VALUE *argv, VALUE self)
{
    // process arguments
    VALUE string, options;
    if (rb_scan_args(argc, argv, "11", &string, &options) == 1) // 1 mandatory argument, 1 optional argument
        options = Qnil;
    if (NIL_P(string))
        return Qnil;
    string = StringValue(string);

    // access these once per parse
    VALUE line_ending   = rb_iv_get(self, "@line_ending");
    line_ending         = StringValue(line_ending);
    VALUE link_class    = rb_iv_get(self, "@external_link_class");
    link_class          = NIL_P(link_class) ? Qnil : StringValue(link_class);
    VALUE link_rel      = rb_iv_get(self, "@external_link_rel");
    link_rel            = NIL_P(link_rel) ? Qnil : StringValue(link_rel);
    VALUE link_proc     = rb_iv_get(self, "@link_proc");
    VALUE mailto_class  = rb_iv_get(self, "@mailto_class");
    mailto_class        = NIL_P(mailto_class) ? Qnil : StringValue(mailto_class);
    VALUE prefix        = rb_iv_get(self, "@internal_link_prefix");
    int output_style    = Wikitext_output_style(rb_iv_get(self, "@output_style"));

    // process options hash
    int base_indent = 0;
    int base_heading_level = NUM2INT(rb_iv_get(self, "@base_heading_level"));
    if (!NIL_P(options) && TYPE(options) == T_HASH)
    {
        // :indent => 0 (or more)
        ID has_key = rb_intern("has_key?");
        ID id = ID2SYM(rb_intern("indent"));
        if (rb_funcall(options, has_key, 1, id) == Qtrue)
        {
            VALUE indent = rb_hash_aref(options, id);
            if (indent == Qfalse)
                base_indent = -1; // indentation disabled
            else
            {
                base_indent = NUM2INT(indent);
                if (base_indent < 0)
                    base_indent = 0;
            }
        }

        // :base_heading_level => 0/1/2/3/4/5/6
        id = ID2SYM(rb_intern("base_heading_level"));
        if (rb_funcall(options, has_key, 1, id) == Qtrue)
            base_heading_level = NUM2INT(rb_hash_aref(options, id));

        // :external_link_rel => 'nofollow'
        id = ID2SYM(rb_intern("external_link_rel"));
        if (rb_funcall(options, has_key, 1, id) == Qtrue)
        {
            link_rel = rb_hash_aref(options, id);
            link_rel = NIL_P(link_rel) ? Qnil : StringValue(link_rel);
        }

        // :output_style => :html/:xml
        id = ID2SYM(rb_intern("output_style"));
        if (rb_funcall(options, has_key, 1, id) == Qtrue)
            output_style = Wikitext_output_style(rb_hash_aref(options, id));

        // :link_proc => lambda { |link_target| ... }
        id = ID2SYM(rb_intern("link_proc"));
        if (rb_funcall(options, has_key, 1, id) == Qtrue)
            link_proc = rb_hash_aref(options, id);
    }

    // normalize, regardless of whether this came from instance variable or override
    if (base_heading_level < 0)
        base_heading_level = 0;
    if (base_heading_level > 6)
        base_heading_level = 6;

    // set up scanner
    char *p = RSTRING_PTR(string);
    long len = RSTRING_LEN(string);
    char *pe = p + len;

    // set up parser struct to make passing parameters a little easier
    parser_t *parser                = parser_new();
    GC_WRAP_PARSER(parser, parser_gc);
    parser->external_link_class     = link_class;
    parser->external_link_rel       = link_rel;
    parser->mailto_class            = mailto_class;
    parser->img_prefix              = rb_iv_get(self, "@img_prefix");
    parser->autolink                = rb_iv_get(self, "@autolink") == Qtrue ? true : false;
    parser->space_to_underscore     = rb_iv_get(self, "@space_to_underscore") == Qtrue ? true : false;
    parser->line_ending             = str_new_from_string(line_ending);
    parser->base_indent             = base_indent;
    parser->base_heading_level      = base_heading_level;
    parser->output_style            = output_style;

    // this simple looping design leads to a single enormous function,
    // but it's faster than doing actual recursive descent and also secure in the face of
    // malicious input that seeks to overflow the stack
    // (with "<blockquote><blockquote><blockquote>..." times by 10,000, for example)
    // given that we expect to deal with a lot of malformed input, a recursive descent design is less appropriate
    // than a straightforward looping translator like this one anyway
    token_t _token;
    _token.type = NO_TOKEN;
    token_t *token = NULL;
    do
    {
        // note that whenever we grab a token we push it into the line buffer
        // this provides us with context-sensitive "memory" of what's been seen so far on this line
#define NEXT_TOKEN()    token = &_token, next_token(token, token, NULL, pe), ary_push(parser->line_buffer, token->type)

        // check to see if we have a token hanging around from a previous iteration of this loop
        if (token == NULL)
        {
            if (_token.type == NO_TOKEN)
            {
                // first time here (haven't started scanning yet)
                token = &_token;
                next_token(token, NULL, p, pe);
                ary_push(parser->line_buffer, token->type);
            }
            else
                // already scanning
                NEXT_TOKEN();
        }
        int type = token->type;

        // can't declare new variables inside a switch statement, so predeclare them here
        long remove_strong          = -1;
        long remove_em              = -1;

        // general purpose counters, flags and pointers
        long i                      = 0;
        long j                      = 0;
        long k                      = 0;
        str_t *output               = NULL;
        str_t _token_str;
        str_t *token_str            = &_token_str;

        // The following giant switch statement contains cases for all the possible token types.
        // In the most basic sense we are emitting the HTML that corresponds to each token,
        // but some tokens require context information in order to decide what to output.
        // For example, does the STRONG token (''') translate to <strong> or </strong>?
        // So when looking at any given token we have three state-maintaining variables which gives us a notion of "where we are":
        //
        //  - the "scope" stack (indicates what HTML DOM structures we are currently nested inside, similar to a CSS selector)
        //  - the line buffer (records tokens seen so far on the current line)
        //  - the line "scope" stack (indicates what the scope should be based only on what is visible on the line so far)
        //
        // Although this is fairly complicated, there is one key simplifying factor:
        // The translator continuously performs auto-correction, and this means that we always have a guarantee that the
        // scope stack (up to the current token) is valid; our translator can take this as a given.
        // Auto-correction basically consists of inserting missing tokens (preventing subsquent HTML from being messed up),
        // or converting illegal (unexpected) tokens to their plain text equivalents (providing visual feedback to Wikitext author).
        switch (type)
        {
            case PRE:
                if (IN_EITHER_OF(NO_WIKI_START, PRE_START))
                {
                    str_append(parser->output, space, sizeof(space) - 1);
                    break;
                }
                else if (IN(BLOCKQUOTE_START))
                {
                    // this kind of nesting not allowed (to avoid user confusion)
                    wiki_pop_excess_elements(parser);
                    wiki_start_para_if_necessary(parser);
                    output = parser->capture ? parser->capture : parser->output;
                    str_append(output, space, sizeof(space) - 1);
                    break;
                }

                // count number of BLOCKQUOTE tokens in line buffer and in scope stack
                ary_push(parser->line, PRE);
                i = ary_count(parser->line, BLOCKQUOTE);
                j = ary_count(parser->scope, BLOCKQUOTE);
                if (i < j)
                {
                    // must pop (reduce nesting level)
                    for (i = j - i; i > 0; i--)
                        wiki_pop_from_stack_up_to(parser, NULL, BLOCKQUOTE, true);
                }

                if (!IN(PRE))
                {
                    parser->pending_crlf = false;
                    wiki_pop_from_stack_up_to(parser, NULL, BLOCKQUOTE, false);
                    wiki_indent(parser);
                    str_append(parser->output, pre_start, sizeof(pre_start) - 1);
                    ary_push(parser->scope, PRE);
                }
                break;

            case PRE_START:
                if (IN_ANY_OF(NO_WIKI_START, PRE, PRE_START))
                {
                    wiki_emit_pending_crlf_if_necessary(parser);
                    str_append(parser->output, escaped_pre_start, sizeof(escaped_pre_start) - 1);
                }
                else if (IN(BLOCKQUOTE_START))
                {
                    wiki_rollback_failed_link(parser); // if any
                    wiki_pop_from_stack_up_to(parser, NULL, BLOCKQUOTE_START, false);
                    wiki_append_pre_start(parser, token);
                }
                else if (IN(BLOCKQUOTE))
                {
                    if (token->column_start == 1) // only allowed in first column
                    {
                        wiki_rollback_failed_link(parser); // if any
                        wiki_pop_all_from_stack(parser);
                        wiki_append_pre_start(parser, token);
                    }
                    else // PRE_START illegal here
                    {
                        output = parser->capture ? parser->capture : parser->output;
                        wiki_pop_excess_elements(parser);
                        wiki_start_para_if_necessary(parser);
                        str_append(output, escaped_pre_start, sizeof(escaped_pre_start) - 1);
                    }
                }
                else
                {
                    wiki_rollback_failed_link(parser); // if any
                    wiki_pop_from_stack_up_to(parser, NULL, P, true);
                    wiki_append_pre_start(parser, token);
                }
                break;

            case PRE_END:
                if (IN_EITHER_OF(NO_WIKI_START, PRE))
                {
                    wiki_emit_pending_crlf_if_necessary(parser);
                    str_append(parser->output, escaped_pre_end, sizeof(escaped_pre_end) - 1);
                }
                else
                {
                    if (IN(PRE_START))
                        wiki_pop_from_stack_up_to(parser, parser->output, PRE_START, true);
                    else
                    {
                        output = parser->capture ? parser->capture : parser->output;
                        wiki_pop_excess_elements(parser);
                        wiki_start_para_if_necessary(parser);
                        str_append(output, escaped_pre_end, sizeof(escaped_pre_end) - 1);
                    }
                }
                break;

            case BLOCKQUOTE:
                if (IN_EITHER_OF(NO_WIKI_START, PRE_START))
                    // no need to check for <pre>; can never appear inside it
                    str_append(parser->output, escaped_blockquote, TOKEN_LEN(token) + 3); // will either emit "&gt;" or "&gt; "
                else if (IN(BLOCKQUOTE_START))
                {
                    // this kind of nesting not allowed (to avoid user confusion)
                    wiki_pop_excess_elements(parser);
                    wiki_start_para_if_necessary(parser);
                    output = parser->capture ? parser->capture : parser->output;
                    str_append(output, escaped_blockquote, TOKEN_LEN(token) + 3); // will either emit "&gt;" or "&gt; "
                    break;
                }
                else
                {
                    ary_push(parser->line, BLOCKQUOTE);

                    // count number of BLOCKQUOTE tokens in line buffer and in scope stack
                    i = ary_count(parser->line, BLOCKQUOTE);
                    j = ary_count(parser->scope, BLOCKQUOTE);

                    // given that BLOCKQUOTE tokens can be nested, peek ahead and see if there are any more which might affect the decision to push or pop
                    while (NEXT_TOKEN(), (token->type == BLOCKQUOTE))
                    {
                        ary_push(parser->line, BLOCKQUOTE);
                        i++;
                    }

                    // now decide whether to push, pop or do nothing
                    if (i > j)
                    {
                        // must push (increase nesting level)
                        wiki_pop_from_stack_up_to(parser, NULL, BLOCKQUOTE, false);
                        for (i = i - j; i > 0; i--)
                        {
                            wiki_indent(parser);
                            str_append(parser->output, blockquote_start, sizeof(blockquote_start) - 1);
                            str_append_str(parser->output, parser->line_ending);
                            ary_push(parser->scope, BLOCKQUOTE);
                        }
                    }
                    else if (i < j)
                    {
                        // must pop (reduce nesting level)
                        for (i = j - i; i > 0; i--)
                            wiki_pop_from_stack_up_to(parser, NULL, BLOCKQUOTE, true);
                    }

                    // jump to top of the loop to process token we scanned during lookahead
                    continue;
                }
                break;

            case BLOCKQUOTE_START:
                if (IN_ANY_OF(NO_WIKI_START, PRE, PRE_START))
                {
                    wiki_emit_pending_crlf_if_necessary(parser);
                    str_append(parser->output, escaped_blockquote_start, sizeof(escaped_blockquote_start) - 1);
                }
                else if (IN(BLOCKQUOTE_START))
                {
                    // nesting is fine here
                    wiki_rollback_failed_link(parser); // if any
                    wiki_pop_from_stack_up_to(parser, NULL, BLOCKQUOTE_START, false);
                    wiki_indent(parser);
                    str_append(parser->output, blockquote_start, sizeof(blockquote_start) - 1);
                    str_append_str(parser->output, parser->line_ending);
                    ary_push(parser->scope, BLOCKQUOTE_START);
                    ary_push(parser->line, BLOCKQUOTE_START);
                }
                else if (IN(BLOCKQUOTE))
                {
                    if (token->column_start == 1) // only allowed in first column
                    {
                        wiki_rollback_failed_link(parser); // if any
                        wiki_pop_all_from_stack(parser);
                        wiki_indent(parser);
                        str_append(parser->output, blockquote_start, sizeof(blockquote_start) - 1);
                        str_append_str(parser->output, parser->line_ending);
                        ary_push(parser->scope, BLOCKQUOTE_START);
                        ary_push(parser->line, BLOCKQUOTE_START);
                    }
                    else // BLOCKQUOTE_START illegal here
                    {
                        output = parser->capture ? parser->capture : parser->output;
                        wiki_pop_excess_elements(parser);
                        wiki_start_para_if_necessary(parser);
                        str_append(output, escaped_blockquote_start, sizeof(escaped_blockquote_start) - 1);
                    }
                }
                else
                {
                    // would be nice to eliminate the repetition here but it's probably the clearest way
                    wiki_rollback_failed_link(parser); // if any
                    wiki_pop_from_stack_up_to(parser, NULL, P, true);
                    wiki_indent(parser);
                    str_append(parser->output, blockquote_start, sizeof(blockquote_start) - 1);
                    str_append_str(parser->output, parser->line_ending);
                    ary_push(parser->scope, BLOCKQUOTE_START);
                    ary_push(parser->line, BLOCKQUOTE_START);
                }
                break;

            case BLOCKQUOTE_END:
                if (IN_ANY_OF(NO_WIKI_START, PRE, PRE_START))
                {
                    wiki_emit_pending_crlf_if_necessary(parser);
                    str_append(parser->output, escaped_blockquote_end, sizeof(escaped_blockquote_end) - 1);
                }
                else
                {
                    if (IN(BLOCKQUOTE_START))
                        wiki_pop_from_stack_up_to(parser, parser->output, BLOCKQUOTE_START, true);
                    else
                    {
                        output = parser->capture ? parser->capture : parser->output;
                        wiki_pop_excess_elements(parser);
                        wiki_start_para_if_necessary(parser);
                        str_append(output, escaped_blockquote_end, sizeof(escaped_blockquote_end) - 1);
                    }
                }
                break;

            case NO_WIKI_START:
                if (IN_ANY_OF(NO_WIKI_START, PRE, PRE_START))
                {
                    wiki_emit_pending_crlf_if_necessary(parser);
                    str_append(parser->output, escaped_no_wiki_start, sizeof(escaped_no_wiki_start) - 1);
                }
                else
                {
                    wiki_pop_excess_elements(parser);
                    wiki_start_para_if_necessary(parser);
                    ary_push(parser->scope, NO_WIKI_START);
                    ary_push(parser->line, NO_WIKI_START);
                }
                break;

            case NO_WIKI_END:
                if (IN(NO_WIKI_START))
                    // <nowiki> should always only ever be the last item in the stack, but use the helper routine just in case
                    wiki_pop_from_stack_up_to(parser, NULL, NO_WIKI_START, true);
                else
                {
                    wiki_pop_excess_elements(parser);
                    wiki_start_para_if_necessary(parser);
                    str_append(parser->output, escaped_no_wiki_end, sizeof(escaped_no_wiki_end) - 1);
                }
                break;

            case STRONG_EM:
                if (IN_ANY_OF(NO_WIKI_START, PRE, PRE_START))
                {
                    wiki_emit_pending_crlf_if_necessary(parser);
                    str_append(parser->output, literal_strong_em, sizeof(literal_strong_em) - 1);
                    break;
                }

                output = parser->capture ? parser->capture : parser->output;
                wiki_pop_excess_elements(parser);

                // if you've seen STRONG/STRONG_START or EM/EM_START, must close them in the reverse order that you saw them!
                // otherwise, must open them
                remove_strong  = -1;
                remove_em      = -1;
                j              = parser->scope->count;
                for (j = j - 1; j >= 0; j--)
                {
                    int val = ary_entry(parser->scope, (int)j);
                    if (val == STRONG || val == STRONG_START)
                    {
                        str_append(output, strong_end, sizeof(strong_end) - 1);
                        remove_strong = j;
                    }
                    else if (val == EM || val == EM_START)
                    {
                        str_append(output, em_end, sizeof(em_end) - 1);
                        remove_em = j;
                    }
                }

                if (remove_strong > remove_em)      // must remove strong first
                {
                    ary_pop(parser->scope);
                    if (remove_em > -1)
                        ary_pop(parser->scope);
                    else    // there was no em to remove!, so consider this an opening em tag
                    {
                        str_append(output, em_start, sizeof(em_start) - 1);
                        ary_push(parser->scope, EM);
                        ary_push(parser->line, EM);
                    }
                }
                else if (remove_em > remove_strong) // must remove em first
                {
                    ary_pop(parser->scope);
                    if (remove_strong > -1)
                        ary_pop(parser->scope);
                    else    // there was no strong to remove!, so consider this an opening strong tag
                    {
                        str_append(output, strong_start, sizeof(strong_start) - 1);
                        ary_push(parser->scope, STRONG);
                        ary_push(parser->line, STRONG);
                    }
                }
                else    // no strong or em to remove, so this must be a new opening of both
                {
                    wiki_start_para_if_necessary(parser);
                    str_append(output, strong_em_start, sizeof(strong_em_start) - 1);
                    ary_push(parser->scope, STRONG);
                    ary_push(parser->line, STRONG);
                    ary_push(parser->scope, EM);
                    ary_push(parser->line, EM);
                }
                break;

            case STRONG:
                if (IN_ANY_OF(NO_WIKI_START, PRE, PRE_START))
                {
                    wiki_emit_pending_crlf_if_necessary(parser);
                    str_append(parser->output, literal_strong, sizeof(literal_strong) - 1);
                }
                else
                {
                    output = parser->capture ? parser->capture : parser->output;
                    if (IN(STRONG_START))
                        // already in span started with <strong>, no choice but to emit this literally
                        str_append(output, literal_strong, sizeof(literal_strong) - 1);
                    else if (IN(STRONG))
                        // STRONG already seen, this is a closing tag
                        wiki_pop_from_stack_up_to(parser, output, STRONG, true);
                    else
                    {
                        // this is a new opening
                        wiki_pop_excess_elements(parser);
                        wiki_start_para_if_necessary(parser);
                        str_append(output, strong_start, sizeof(strong_start) - 1);
                        ary_push(parser->scope, STRONG);
                        ary_push(parser->line, STRONG);
                    }
                }
                break;

            case STRONG_START:
                if (IN_ANY_OF(NO_WIKI_START, PRE, PRE_START))
                {
                    wiki_emit_pending_crlf_if_necessary(parser);
                    str_append(parser->output, escaped_strong_start, sizeof(escaped_strong_start) - 1);
                }
                else
                {
                    output = parser->capture ? parser->capture : parser->output;
                    if (IN_EITHER_OF(STRONG_START, STRONG))
                        str_append(output, escaped_strong_start, sizeof(escaped_strong_start) - 1);
                    else
                    {
                        wiki_pop_excess_elements(parser);
                        wiki_start_para_if_necessary(parser);
                        str_append(output, strong_start, sizeof(strong_start) - 1);
                        ary_push(parser->scope, STRONG_START);
                        ary_push(parser->line, STRONG_START);
                    }
                }
                break;

            case STRONG_END:
                if (IN_ANY_OF(NO_WIKI_START, PRE, PRE_START))
                {
                    wiki_emit_pending_crlf_if_necessary(parser);
                    str_append(parser->output, escaped_strong_end, sizeof(escaped_strong_end) - 1);
                }
                else
                {
                    output = parser->capture ? parser->capture : parser->output;
                    if (IN(STRONG_START))
                        wiki_pop_from_stack_up_to(parser, output, STRONG_START, true);
                    else
                    {
                        // no STRONG_START in scope, so must interpret the STRONG_END without any special meaning
                        wiki_pop_excess_elements(parser);
                        wiki_start_para_if_necessary(parser);
                        str_append(output, escaped_strong_end, sizeof(escaped_strong_end) - 1);
                    }
                }
                break;

            case EM:
                if (IN_ANY_OF(NO_WIKI_START, PRE, PRE_START))
                {
                    wiki_emit_pending_crlf_if_necessary(parser);
                    str_append(parser->output, literal_em, sizeof(literal_em) - 1);
                }
                else
                {
                    output = parser->capture ? parser->capture : parser->output;
                    if (IN(EM_START))
                        // already in span started with <em>, no choice but to emit this literally
                        str_append(output, literal_em, sizeof(literal_em) - 1);
                    else if (IN(EM))
                        // EM already seen, this is a closing tag
                        wiki_pop_from_stack_up_to(parser, output, EM, true);
                    else
                    {
                        // this is a new opening
                        wiki_pop_excess_elements(parser);
                        wiki_start_para_if_necessary(parser);
                        str_append(output, em_start, sizeof(em_start) - 1);
                        ary_push(parser->scope, EM);
                        ary_push(parser->line, EM);
                    }
                }
                break;

            case EM_START:
                if (IN_ANY_OF(NO_WIKI_START, PRE, PRE_START))
                {
                    wiki_emit_pending_crlf_if_necessary(parser);
                    str_append(parser->output, escaped_em_start, sizeof(escaped_em_start) - 1);
                }
                else
                {
                    output = parser->capture ? parser->capture : parser->output;
                    if (IN_EITHER_OF(EM_START, EM))
                        str_append(output, escaped_em_start, sizeof(escaped_em_start) - 1);
                    else
                    {
                        wiki_pop_excess_elements(parser);
                        wiki_start_para_if_necessary(parser);
                        str_append(output, em_start, sizeof(em_start) - 1);
                        ary_push(parser->scope, EM_START);
                        ary_push(parser->line, EM_START);
                    }
                }
                break;

            case EM_END:
                if (IN_ANY_OF(NO_WIKI_START, PRE, PRE_START))
                {
                    wiki_emit_pending_crlf_if_necessary(parser);
                    str_append(parser->output, escaped_em_end, sizeof(escaped_em_end) - 1);
                }
                else
                {
                    output = parser->capture ? parser->capture : parser->output;
                    if (IN(EM_START))
                        wiki_pop_from_stack_up_to(parser, output, EM_START, true);
                    else
                    {
                        // no EM_START in scope, so must interpret the EM_END without any special meaning
                        wiki_pop_excess_elements(parser);
                        wiki_start_para_if_necessary(parser);
                        str_append(output, escaped_em_end, sizeof(escaped_em_end) - 1);
                    }
                }
                break;

            case TT:
                if (IN_ANY_OF(NO_WIKI_START, PRE, PRE_START))
                {
                    wiki_emit_pending_crlf_if_necessary(parser);
                    str_append(parser->output, backtick, sizeof(backtick) - 1);
                }
                else
                {
                    output = parser->capture ? parser->capture : parser->output;
                    if (IN(TT_START))
                        // already in span started with <tt>, no choice but to emit this literally
                        str_append(output, backtick, sizeof(backtick) - 1);
                    else if (IN(TT))
                        // TT (`) already seen, this is a closing tag
                        wiki_pop_from_stack_up_to(parser, output, TT, true);
                    else
                    {
                        // this is a new opening
                        wiki_pop_excess_elements(parser);
                        wiki_start_para_if_necessary(parser);
                        str_append(output, code_start, sizeof(code_start) - 1);
                        ary_push(parser->scope, TT);
                        ary_push(parser->line, TT);
                    }
                }
                break;

            case TT_START:
                if (IN_ANY_OF(NO_WIKI_START, PRE, PRE_START))
                {
                    wiki_emit_pending_crlf_if_necessary(parser);
                    str_append(parser->output, escaped_tt_start, sizeof(escaped_tt_start) - 1);
                }
                else
                {
                    output = parser->capture ? parser->capture : parser->output;
                    if (IN_EITHER_OF(TT_START, TT))
                        str_append(output, escaped_tt_start, sizeof(escaped_tt_start) - 1);
                    else
                    {
                        wiki_pop_excess_elements(parser);
                        wiki_start_para_if_necessary(parser);
                        str_append(output, code_start, sizeof(code_start) - 1);
                        ary_push(parser->scope, TT_START);
                        ary_push(parser->line, TT_START);
                    }
                }
                break;

            case TT_END:
                if (IN_ANY_OF(NO_WIKI_START, PRE, PRE_START))
                {
                    wiki_emit_pending_crlf_if_necessary(parser);
                    str_append(parser->output, escaped_tt_end, sizeof(escaped_tt_end) - 1);
                }
                else
                {
                    output = parser->capture ? parser->capture : parser->output;
                    if (IN(TT_START))
                        wiki_pop_from_stack_up_to(parser, output, TT_START, true);
                    else
                    {
                        // no TT_START in scope, so must interpret the TT_END without any special meaning
                        wiki_pop_excess_elements(parser);
                        wiki_start_para_if_necessary(parser);
                        str_append(output, escaped_tt_end, sizeof(escaped_tt_end) - 1);
                    }
                }
                break;

            case OL:
            case UL:
                if (IN_EITHER_OF(NO_WIKI_START, PRE_START))
                {
                    // no need to check for PRE; can never appear inside it
                    str_append(parser->output, token->start, TOKEN_LEN(token));
                    break;
                }

                // count number of tokens in line and scope stacks
                int bq_count = ary_count(parser->scope, BLOCKQUOTE_START);
                i = parser->line->count - ary_count(parser->line, BLOCKQUOTE_START);
                j = parser->scope->count - bq_count;
                k = i;

                // list tokens can be nested so look ahead for any more which might affect the decision to push or pop
                for (;;)
                {
                    type = token->type;
                    if (type == OL || type == UL)
                    {
                        token = NULL;
                        if (i - k >= 2)                             // already seen at least one OL or UL
                        {
                            ary_push(parser->line, NESTED_LIST);    // which means this is a nested list
                            i += 3;
                        }
                        else
                            i += 2;
                        ary_push(parser->line, type);
                        ary_push(parser->line, LI);

                        // want to compare line with scope but can only do so if scope has enough items on it
                        if (j >= i)
                        {
                            if (ary_entry(parser->scope, (int)(i + bq_count - 2)) == type &&
                                ary_entry(parser->scope, (int)(i + bq_count - 1)) == LI)
                            {
                                // line and scope match at this point: do nothing yet
                            }
                            else
                            {
                                // item just pushed onto line does not match corresponding slot of scope!
                                for (; j >= i - 2; j--)
                                    // must pop back before emitting
                                    wiki_pop_from_stack(parser, NULL);

                                // will emit UL or OL, then LI
                                break;
                            }
                        }
                        else        // line stack size now exceeds scope stack size: must increase nesting level
                            break;  // will emit UL or OL, then LI
                    }
                    else
                    {
                        // not a OL or UL token!
                        if (j == i)
                            // must close existing LI and re-open new one
                            wiki_pop_from_stack(parser, NULL);
                        else if (j > i)
                        {
                            // item just pushed onto line does not match corresponding slot of scope!
                            for (; j >= i; j--)
                                // must pop back before emitting
                                wiki_pop_from_stack(parser, NULL);
                        }
                        break;
                    }
                    NEXT_TOKEN();
                }

                // will emit
                if (type == OL || type == UL)
                {
                    // if LI is at the top of a stack this is the start of a nested list
                    if (j > 0 && ary_entry(parser->scope, -1) == LI)
                    {
                        // so we should precede it with a CRLF, and indicate that it's a nested list
                        str_append(parser->output, parser->line_ending->ptr, parser->line_ending->len);
                        ary_push(parser->scope, NESTED_LIST);
                    }
                    else
                    {
                        // this is a new list
                        if (IN(BLOCKQUOTE_START))
                            wiki_pop_from_stack_up_to(parser, NULL, BLOCKQUOTE_START, false);
                        else
                            wiki_pop_from_stack_up_to(parser, NULL, BLOCKQUOTE, false);
                    }

                    // emit
                    wiki_indent(parser);
                    if (type == OL)
                        str_append(parser->output, ol_start, sizeof(ol_start) - 1);
                    else if (type == UL)
                        str_append(parser->output, ul_start, sizeof(ul_start) - 1);
                    ary_push(parser->scope, type);
                    str_append(parser->output, parser->line_ending->ptr, parser->line_ending->len);
                }
                else if (type == SPACE)
                    // silently throw away the optional SPACE token after final list marker
                    token = NULL;

                wiki_indent(parser);
                str_append(parser->output, li_start, sizeof(li_start) - 1);
                ary_push(parser->scope, LI);

                // any subsequent UL or OL tokens on this line are syntax errors and must be emitted literally
                if (type == OL || type == UL)
                {
                    k = 0;
                    while (k++, NEXT_TOKEN(), (type = token->type))
                    {
                        if (type == OL || type == UL)
                            str_append(parser->output, token->start, TOKEN_LEN(token));
                        else if (type == SPACE && k == 1)
                        {
                            // silently throw away the optional SPACE token after final list marker
                            token = NULL;
                            break;
                        }
                        else
                            break;
                    }
                }

                // jump to top of the loop to process token we scanned during lookahead
                continue;

            case H6_START:
            case H5_START:
            case H4_START:
            case H3_START:
            case H2_START:
            case H1_START:
                if (IN_EITHER_OF(NO_WIKI_START, PRE_START))
                {
                    // no need to check for PRE; can never appear inside it
                    str_append(parser->output, token->start, TOKEN_LEN(token));
                    break;
                }

                // pop up to but not including the last BLOCKQUOTE on the scope stack
                if (IN(BLOCKQUOTE_START))
                    wiki_pop_from_stack_up_to(parser, NULL, BLOCKQUOTE_START, false);
                else
                    wiki_pop_from_stack_up_to(parser, NULL, BLOCKQUOTE, false);

                // count number of BLOCKQUOTE tokens in line buffer and in scope stack
                ary_push(parser->line, type);
                i = ary_count(parser->line, BLOCKQUOTE);
                j = ary_count(parser->scope, BLOCKQUOTE);

                // decide whether we need to pop off excess BLOCKQUOTE tokens (will never need to push; that is handled above in the BLOCKQUOTE case itself)
                if (i < j)
                {
                    // must pop (reduce nesting level)
                    for (i = j - i; i > 0; i--)
                        wiki_pop_from_stack_up_to(parser, NULL, BLOCKQUOTE, true);
                }

                // discard any whitespace here (so that "== foo ==" will be translated to "<h2>foo</h2>" rather than "<h2> foo </h2")
                while (NEXT_TOKEN(), (token->type == SPACE))
                    ; // discard

                ary_push(parser->scope, type);
                wiki_indent(parser);

                // take base_heading_level into account
                type += base_heading_level;
                if (type > H6_START) // no need to check for underflow (base_heading_level never negative)
                    type = H6_START;

                // rather than repeat all that code for each kind of heading, share it and use a conditional here
                if (type == H6_START)
                    str_append(parser->output, h6_start, sizeof(h6_start) - 1);
                else if (type == H5_START)
                    str_append(parser->output, h5_start, sizeof(h5_start) - 1);
                else if (type == H4_START)
                    str_append(parser->output, h4_start, sizeof(h4_start) - 1);
                else if (type == H3_START)
                    str_append(parser->output, h3_start, sizeof(h3_start) - 1);
                else if (type == H2_START)
                    str_append(parser->output, h2_start, sizeof(h2_start) - 1);
                else if (type == H1_START)
                    str_append(parser->output, h1_start, sizeof(h1_start) - 1);

                // jump to top of the loop to process token we scanned during lookahead
                continue;

            case H6_END:
            case H5_END:
            case H4_END:
            case H3_END:
            case H2_END:
            case H1_END:
                if (IN_ANY_OF(NO_WIKI_START, PRE, PRE_START))
                {
                    wiki_emit_pending_crlf_if_necessary(parser);
                    str_append(parser->output, token->start, TOKEN_LEN(token));
                }
                else
                {
                    wiki_rollback_failed_external_link(parser); // if any
                    if ((type == H6_END && !IN(H6_START)) ||
                        (type == H5_END && !IN(H5_START)) ||
                        (type == H4_END && !IN(H4_START)) ||
                        (type == H3_END && !IN(H3_START)) ||
                        (type == H2_END && !IN(H2_START)) ||
                        (type == H1_END && !IN(H1_START)))
                    {
                        // literal output only if not in appropriate scope (we stay silent in that case)
                        wiki_start_para_if_necessary(parser);
                        str_append(parser->output, token->start, TOKEN_LEN(token));
                    }
                }
                break;

            case MAIL:
                if (IN_ANY_OF(NO_WIKI_START, PRE, PRE_START))
                {
                    wiki_emit_pending_crlf_if_necessary(parser);
                    str_append(parser->output, token->start, TOKEN_LEN(token));
                }
                else if (IN(EXT_LINK_START))
                    // must be capturing and this must be part of the link text
                    str_append(parser->capture, token->start, TOKEN_LEN(token));
                else
                {
                    wiki_pop_excess_elements(parser);
                    wiki_start_para_if_necessary(parser);
                    token_str->ptr = token->start;
                    token_str->len = TOKEN_LEN(token);
                    wiki_append_hyperlink(parser, rb_str_new2("mailto:"), token_str, NULL, mailto_class, Qnil, true);
                }
                break;

            case URI:
                if (IN(NO_WIKI_START))
                {
                    // user can temporarily suppress autolinking by using <nowiki></nowiki>
                    // note that unlike MediaWiki, we do allow autolinking inside PRE blocks
                    token_str->ptr = token->start;
                    token_str->len = TOKEN_LEN(token);
                    wiki_append_sanitized_link_target(token_str, parser->output, false);
                }
                else if (IN(LINK_START))
                {
                    // if the URI were allowed it would have been handled already in LINK_START
                    wiki_rollback_failed_internal_link(parser);
                    token_str->ptr = token->start;
                    token_str->len = TOKEN_LEN(token);
                    wiki_append_hyperlink(parser, Qnil, token_str, NULL, parser->external_link_class, parser->external_link_rel, true);
                }
                else if (IN(EXT_LINK_START))
                {
                    if (parser->link_target->len == 0)
                    {
                        // this must be our link target: look ahead to make sure we see the space we're expecting to see
                        token_str->ptr = token->start;
                        token_str->len = TOKEN_LEN(token);
                        NEXT_TOKEN();
                        if (token->type == SPACE)
                        {
                            ary_push(parser->scope, SPACE);
                            str_append_str(parser->link_target, token_str);
                            str_clear(parser->link_text);
                            parser->capture     = parser->link_text;
                            token               = NULL; // silently consume space
                        }
                        else
                        {
                            // didn't see the space! this must be an error
                            wiki_pop_from_stack(parser, NULL);
                            wiki_pop_excess_elements(parser);
                            wiki_start_para_if_necessary(parser);
                            str_append(parser->output, ext_link_start, sizeof(ext_link_start) - 1);
                            wiki_append_hyperlink(parser, Qnil, token_str, NULL, parser->external_link_class, parser->external_link_rel, true);
                            continue;
                        }
                    }
                    else
                    {
                        token_str->ptr = token->start;
                        token_str->len = TOKEN_LEN(token);
                        wiki_append_sanitized_link_target(token_str, parser->link_text, false);
                    }
                }
                else
                {
                    wiki_pop_excess_elements(parser);
                    wiki_start_para_if_necessary(parser);
                    token_str->ptr = token->start;
                    token_str->len = TOKEN_LEN(token);
                    wiki_append_hyperlink(parser, Qnil, token_str, NULL, parser->external_link_class, parser->external_link_rel, true);
                }
                break;

            case PATH:
                if (IN_ANY_OF(NO_WIKI_START, PRE, PRE_START))
                {
                    wiki_emit_pending_crlf_if_necessary(parser);
                    str_append(parser->output, token->start, TOKEN_LEN(token));
                }
                else if (IN(EXT_LINK_START))
                {
                    if (parser->link_target->len == 0)
                    {
                        // this must be our link target: look ahead to make sure we see the space we're expecting to see
                        token_str->ptr = token->start;
                        token_str->len = TOKEN_LEN(token);
                        NEXT_TOKEN();
                        if (token->type == SPACE)
                        {
                            ary_push(parser->scope, PATH);
                            ary_push(parser->scope, SPACE);
                            str_append_str(parser->link_target, token_str);
                            str_clear(parser->link_text);
                            parser->capture     = parser->link_text;
                            token               = NULL; // silently consume space
                        }
                        else
                        {
                            // didn't see the space! this must be an error
                            wiki_pop_from_stack(parser, NULL);
                            wiki_pop_excess_elements(parser);
                            wiki_start_para_if_necessary(parser);
                            str_append(parser->output, ext_link_start, sizeof(ext_link_start) - 1);
                            str_append_str(parser->output, token_str);
                            continue;
                        }
                    }
                    else
                        str_append(parser->link_text, token->start, TOKEN_LEN(token));
                }
                else
                {
                    output = parser->capture ? parser->capture : parser->output;
                    wiki_pop_excess_elements(parser);
                    wiki_start_para_if_necessary(parser);
                    str_append(output, token->start, TOKEN_LEN(token));
                }
                break;

            // internal links (links to other wiki articles) look like this:
            //      [[another article]] (would point at, for example, "/wiki/another_article")
            //      [[the other article|the link text we'll use for it]]
            //      [[the other article | the link text we'll use for it]]
            // MediaWiki has strict requirements about what it will accept as a link target:
            //      all wikitext markup is disallowed:
            //          example [[foo ''bar'' baz]]
            //          renders [[foo <em>bar</em> baz]]        (ie. not a link)
            //          example [[foo <em>bar</em> baz]]
            //          renders [[foo <em>bar</em> baz]]        (ie. not a link)
            //          example [[foo <nowiki>''</nowiki> baz]]
            //          renders [[foo '' baz]]                  (ie. not a link)
            //          example [[foo <bar> baz]]
            //          renders [[foo &lt;bar&gt; baz]]         (ie. not a link)
            //      HTML entities and non-ASCII, however, make it through:
            //          example [[foo &euro;]]
            //          renders <a href="/wiki/Foo_%E2%82%AC">foo &euro;</a>
            //          example [[foo €]]
            //          renders <a href="/wiki/Foo_%E2%82%AC">foo €</a>
            // we'll impose similar restrictions here for the link target; allowed tokens will be:
            //      SPACE, SPECIAL_URI_CHARS, PRINTABLE, PATH, ALNUM, DEFAULT, QUOT and AMP
            // everything else will be rejected
            case LINK_START:
                output = parser->capture ? parser->capture : parser->output;
                if (IN_ANY_OF(NO_WIKI_START, PRE, PRE_START))
                {
                    wiki_emit_pending_crlf_if_necessary(parser);
                    str_append(output, link_start, sizeof(link_start) - 1);
                }
                else if (IN(EXT_LINK_START))
                    // already in external link scope! (and in fact, must be capturing link_text right now)
                    str_append(output, link_start, sizeof(link_start) - 1);
                else if (IN(LINK_START))
                {
                    // already in internal link scope! this is a syntax error
                    wiki_rollback_failed_internal_link(parser);
                    str_append(parser->output, link_start, sizeof(link_start) - 1);
                }
                else if (IN(SEPARATOR))
                {
                    // scanning internal link text
                }
                else // not in internal link scope yet
                {
                    // will either emit a link, or the rollback of a failed link, so start the para now
                    wiki_pop_excess_elements(parser);
                    wiki_start_para_if_necessary(parser);
                    ary_push(parser->scope, LINK_START);

                    // look ahead and try to gobble up link target
                    while (NEXT_TOKEN(), (type = token->type))
                    {
                        if (type == SPACE               ||
                            type == SPECIAL_URI_CHARS   ||
                            type == PATH                ||
                            type == PRINTABLE           ||
                            type == ALNUM               ||
                            type == DEFAULT             ||
                            type == QUOT                ||
                            type == QUOT_ENTITY         ||
                            type == AMP                 ||
                            type == AMP_ENTITY          ||
                            type == IMG_START           ||
                            type == IMG_END             ||
                            type == LEFT_CURLY          ||
                            type == RIGHT_CURLY)
                        {
                            // accumulate these tokens into link_target
                            if (parser->link_target->len == 0)
                            {
                                str_clear(parser->link_target);
                                parser->capture = parser->link_target;
                            }
                            if (type == QUOT_ENTITY)
                                // don't insert the entity, insert the literal quote
                                str_append(parser->link_target, quote, sizeof(quote) - 1);
                            else if (type == AMP_ENTITY)
                                // don't insert the entity, insert the literal ampersand
                                str_append(parser->link_target, ampersand, sizeof(ampersand) - 1);
                            else
                                str_append(parser->link_target, token->start, TOKEN_LEN(token));
                        }
                        else if (type == LINK_END)
                        {
                            if (parser->link_target->len == 0) // bail for inputs like "[[]]"
                                wiki_rollback_failed_internal_link(parser);
                            break; // jump back to top of loop (will handle this in LINK_END case below)
                        }
                        else if (type == SEPARATOR)
                        {
                            if (parser->link_target->len == 0) // bail for inputs like "[[|"
                                wiki_rollback_failed_internal_link(parser);
                            else
                            {
                                ary_push(parser->scope, SEPARATOR);
                                str_clear(parser->link_text);
                                parser->capture     = parser->link_text;
                                token               = NULL;
                            }
                            break;
                        }
                        else // unexpected token (syntax error)
                        {
                            wiki_rollback_failed_internal_link(parser);
                            break; // jump back to top of loop to handle unexpected token
                        }
                    }

                    // jump to top of the loop to process token we scanned during lookahead (if any)
                    continue;
                }
                break;

            case LINK_END:
                output = parser->capture ? parser->capture : parser->output;
                if (IN_ANY_OF(NO_WIKI_START, PRE, PRE_START))
                {
                    wiki_emit_pending_crlf_if_necessary(parser);
                    str_append(output, link_end, sizeof(link_end) - 1);
                }
                else if (IN(EXT_LINK_START))
                    // already in external link scope! (and in fact, must be capturing link_text right now)
                    str_append(output, link_end, sizeof(link_end) - 1);
                else if (IN(LINK_START)) // in internal link scope!
                {
                    if (wiki_blank(parser->link_target))
                    {
                        // special case for inputs like "[[    ]]"
                        wiki_rollback_failed_internal_link(parser);
                        str_append(parser->output, link_end, sizeof(link_end) - 1);
                        break;
                    }
                    if (parser->link_text->len == 0 ||
                        wiki_blank(parser->link_text))
                    {
                        // use link target as link text
                        str_clear(parser->link_text);
                        wiki_append_sanitized_link_target(parser->link_target, parser->link_text, true);
                    }
                    else
                        wiki_trim_link_text(parser);

                    // perform "redlink" check before manipulating link_target
                    if (NIL_P(link_proc))
                        j = Qnil;
                    else
                    {
                        j = rb_funcall(link_proc, rb_intern("call"), 1, string_from_str(parser->link_target));
                        if (!NIL_P(j))
                        {
                            VALUE l = j; // can't cast inside StringValue macro
                            j = StringValue(l);
                        }
                    }
                    wiki_encode_link_target(parser);
                    wiki_pop_from_stack_up_to(parser, output, LINK_START, true);
                    parser->capture = NULL;
                    wiki_append_hyperlink(parser, prefix, parser->link_target, parser->link_text, j, Qnil, false);
                    str_clear(parser->link_target);
                    str_clear(parser->link_text);
                }
                else // wasn't in internal link scope
                {
                    wiki_pop_excess_elements(parser);
                    wiki_start_para_if_necessary(parser);
                    str_append(output, link_end, sizeof(link_end) - 1);
                }
                break;

            // external links look like this:
            //      [http://google.com/ the link text]
            //      [/other/page/on/site see this page]
            // strings in square brackets which don't match this syntax get passed through literally; eg:
            //      he was very angery [sic] about the turn of events
            case EXT_LINK_START:
                output = parser->capture ? parser->capture : parser->output;
                if (IN_ANY_OF(NO_WIKI_START, PRE, PRE_START))
                {
                    wiki_emit_pending_crlf_if_necessary(parser);
                    str_append(output, ext_link_start, sizeof(ext_link_start) - 1);
                }
                else if (IN(EXT_LINK_START))
                    // already in external link scope! (and in fact, must be capturing link_text right now)
                    str_append(output, ext_link_start, sizeof(ext_link_start) - 1);
                else if (IN(LINK_START))
                {
                    // already in internal link scope!
                    if (parser->link_target->len == 0 || !IN(SPACE))
                        str_append(parser->link_target, ext_link_start, sizeof(ext_link_start) - 1);
                    else // link target has already been scanned
                        str_append(parser->link_text, ext_link_start, sizeof(ext_link_start) - 1);
                }
                else // not in external link scope yet
                {
                    // will either emit a link, or the rollback of a failed link, so start the para now
                    wiki_pop_excess_elements(parser);
                    wiki_start_para_if_necessary(parser);

                    // look ahead: expect an absolute URI (with protocol) or "relative" (path) URI
                    NEXT_TOKEN();
                    if (token->type == URI || token->type == PATH)
                        ary_push(parser->scope, EXT_LINK_START);    // so far so good, jump back to the top of the loop
                    else
                        // only get here if there was a syntax error (missing URI)
                        str_append(parser->output, ext_link_start, sizeof(ext_link_start) - 1);
                    continue; // jump back to top of loop to handle token (either URI or whatever it is)
                }
                break;

            case EXT_LINK_END:
                output = parser->capture ? parser->capture : parser->output;
                if (IN_ANY_OF(NO_WIKI_START, PRE, PRE_START))
                {
                    wiki_emit_pending_crlf_if_necessary(parser);
                    str_append(output, ext_link_end, sizeof(ext_link_end) - 1);
                }
                else if (IN(EXT_LINK_START))
                {
                    if (parser->link_text->len == 0)
                        // syntax error: external link with no link text
                        wiki_rollback_failed_external_link(parser);
                    else
                    {
                        // success!
                        j = IN(PATH) ? Qnil : parser->external_link_class;
                        k = IN(PATH) ? Qnil : parser->external_link_rel;
                        wiki_pop_from_stack_up_to(parser, output, EXT_LINK_START, true);
                        parser->capture = NULL;
                        wiki_append_hyperlink(parser, Qnil, parser->link_target, parser->link_text, j, k, false);
                    }
                    str_clear(parser->link_target);
                    str_clear(parser->link_text);
                }
                else
                {
                    wiki_pop_excess_elements(parser);
                    wiki_start_para_if_necessary(parser);
                    str_append(parser->output, ext_link_end, sizeof(ext_link_end) - 1);
                }
                break;

            case SEPARATOR:
                output = parser->capture ? parser->capture : parser->output;
                wiki_pop_excess_elements(parser);
                wiki_start_para_if_necessary(parser);
                str_append(output, separator, sizeof(separator) - 1);
                break;

            case SPACE:
                output = parser->capture ? parser->capture : parser->output;
                if (IN_ANY_OF(NO_WIKI_START, PRE, PRE_START))
                {
                    wiki_emit_pending_crlf_if_necessary(parser);
                    str_append(output, token->start, TOKEN_LEN(token));
                }
                else
                {
                    // peek ahead to see next token
                    char    *token_ptr  = token->start;
                    long    token_len   = TOKEN_LEN(token);
                    NEXT_TOKEN();
                    type = token->type;
                    if ((type == H6_END && IN(H6_START)) ||
                        (type == H5_END && IN(H5_START)) ||
                        (type == H4_END && IN(H4_START)) ||
                        (type == H3_END && IN(H3_START)) ||
                        (type == H2_END && IN(H2_START)) ||
                        (type == H1_END && IN(H1_START)))
                    {
                        // will suppress emission of space (discard) if next token is a H6_END, H5_END etc and we are in the corresponding scope
                    }
                    else
                    {
                        // emit the space
                        wiki_pop_excess_elements(parser);
                        wiki_start_para_if_necessary(parser);
                        str_append(output, token_ptr, token_len);
                    }

                    // jump to top of the loop to process token we scanned during lookahead
                    continue;
                }
                break;

            case QUOT_ENTITY:
            case AMP_ENTITY:
            case NAMED_ENTITY:
            case DECIMAL_ENTITY:
                // pass these through unaltered as they are case sensitive
                output = parser->capture ? parser->capture : parser->output;
                wiki_pop_excess_elements(parser);
                wiki_start_para_if_necessary(parser);
                str_append(output, token->start, TOKEN_LEN(token));
                break;

            case HEX_ENTITY:
                // normalize hex entities (downcase them)
                output = parser->capture ? parser->capture : parser->output;
                wiki_pop_excess_elements(parser);
                wiki_start_para_if_necessary(parser);
                str_append(output, token->start, TOKEN_LEN(token));
                wiki_downcase_bang(output->ptr + output->len - TOKEN_LEN(token), TOKEN_LEN(token));
                break;

            case QUOT:
                output = parser->capture ? parser->capture : parser->output;
                wiki_pop_excess_elements(parser);
                wiki_start_para_if_necessary(parser);
                str_append(output, quot_entity, sizeof(quot_entity) - 1);
                break;

            case AMP:
                output = parser->capture ? parser->capture : parser->output;
                wiki_pop_excess_elements(parser);
                wiki_start_para_if_necessary(parser);
                str_append(output, amp_entity, sizeof(amp_entity) - 1);
                break;

            case LESS:
                output = parser->capture ? parser->capture : parser->output;
                wiki_pop_excess_elements(parser);
                wiki_start_para_if_necessary(parser);
                str_append(output, lt_entity, sizeof(lt_entity) - 1);
                break;

            case GREATER:
                output = parser->capture ? parser->capture : parser->output;
                wiki_pop_excess_elements(parser);
                wiki_start_para_if_necessary(parser);
                str_append(output, gt_entity, sizeof(gt_entity) - 1);
                break;

            case IMG_START:
                if (IN_ANY_OF(NO_WIKI_START, PRE, PRE_START))
                {
                    wiki_emit_pending_crlf_if_necessary(parser);
                    str_append(parser->output, token->start, TOKEN_LEN(token));
                }
                else if (parser->capture)
                    str_append(parser->capture, token->start, TOKEN_LEN(token));
                else
                {
                    // not currently capturing: will be emitting something on success or failure, so get ready
                    wiki_pop_excess_elements(parser);
                    wiki_start_para_if_necessary(parser);

                    // scan ahead consuming PATH, PRINTABLE, ALNUM and SPECIAL_URI_CHARS tokens
                    // will cheat here and abuse the link_target capture buffer to accumulate text
                    while (NEXT_TOKEN(), (type = token->type))
                    {
                        if (type == PATH || type == PRINTABLE || type == ALNUM || type == SPECIAL_URI_CHARS)
                            str_append(parser->link_target, token->start, TOKEN_LEN(token));
                        else if (type == IMG_END && parser->link_target->len > 0)
                        {
                            // success
                            wiki_append_img(parser, parser->link_target->ptr, parser->link_target->len);
                            token = NULL;
                            break;
                        }
                        else // unexpected token or zero-length target (syntax error)
                        {
                            // rollback
                            str_append(parser->output, literal_img_start, sizeof(literal_img_start) - 1);
                            if (parser->link_target->len > 0)
                                str_append(parser->output, parser->link_target->ptr, parser->link_target->len);
                            break;
                        }
                    }

                    // jump to top of the loop to process token we scanned during lookahead
                    str_clear(parser->link_target);
                    continue;
                }
                break;

            case CRLF:
                i = parser->pending_crlf;
                parser->pending_crlf = false;
                wiki_rollback_failed_link(parser); // if any
                if (IN_EITHER_OF(NO_WIKI_START, PRE_START))
                {
                    ary_clear(parser->line_buffer);
                    str_append_str(parser->output, parser->line_ending);
                    break;
                }
                else if (IN(PRE))
                {
                    // beware when BLOCKQUOTE on line buffer (not line stack!) prior to CRLF, that must be end of PRE block
                    if (ary_entry(parser->line_buffer, -2) == BLOCKQUOTE)
                        // don't emit in this case
                        wiki_pop_from_stack_up_to(parser, parser->output, PRE, true);
                    else
                    {
                        if (ary_entry(parser->line_buffer, -2) == PRE)
                        {
                             // only thing on line is the PRE: emit pending line ending (if we had one)
                             if (i)
                                 str_append_str(parser->output, parser->line_ending);
                        }

                        // clear these _before_ calling NEXT_TOKEN (NEXT_TOKEN adds to the line_buffer)
                        ary_clear(parser->line);
                        ary_clear(parser->line_buffer);

                        // peek ahead to see if this is definitely the end of the PRE block
                        NEXT_TOKEN();
                        type = token->type;
                        if (type != BLOCKQUOTE && type != PRE)
                            // this is definitely the end of the block, so don't emit
                            wiki_pop_from_stack_up_to(parser, parser->output, PRE, true);
                        else
                            // potentially will emit
                            parser->pending_crlf = true;

                        continue; // jump back to top of loop to handle token grabbed via lookahead
                    }
                }
                else
                {
                    parser->pending_crlf = true;

                    // count number of BLOCKQUOTE tokens in line buffer (can be zero) and pop back to that level
                    // as a side effect, this handles any open span-level elements and unclosed blocks
                    // (with special handling for P blocks and LI elements)
                    i = ary_count(parser->line, BLOCKQUOTE) + ary_count(parser->scope, BLOCKQUOTE_START);
                    for (j = parser->scope->count; j > i; j--)
                    {
                        if (parser->scope->count > 0 && ary_entry(parser->scope, -1) == LI)
                        {
                            parser->pending_crlf = false;
                            break;
                        }

                        // special handling on last iteration through the loop if the top item on the scope is a P block
                        if ((j - i == 1) && ary_entry(parser->scope, -1) == P)
                        {
                            // if nothing or BLOCKQUOTE on line buffer (not line stack!) prior to CRLF, this must be a paragraph break
                            // (note that we have to make sure we're not inside a BLOCKQUOTE_START block
                            // because in those blocks BLOCKQUOTE tokens have no special meaning)
                            if (NO_ITEM(ary_entry(parser->line_buffer, -2)) ||
                                (ary_entry(parser->line_buffer, -2) == BLOCKQUOTE && !IN(BLOCKQUOTE_START)))
                                // paragraph break
                                parser->pending_crlf = false;
                            else
                                // not a paragraph break!
                                continue;
                        }
                        wiki_pop_from_stack(parser, NULL);
                    }
                }

                // delete the entire contents of the line scope stack and buffer
                ary_clear(parser->line);
                ary_clear(parser->line_buffer);
                break;

            case SPECIAL_URI_CHARS:
            case PRINTABLE:
            case ALNUM:
            case IMG_END:
            case LEFT_CURLY:
            case RIGHT_CURLY:
                output = parser->capture ? parser->capture : parser->output;
                wiki_pop_excess_elements(parser);
                wiki_start_para_if_necessary(parser);
                str_append(output, token->start, TOKEN_LEN(token));
                break;

            case DEFAULT:
                output = parser->capture ? parser->capture : parser->output;
                wiki_pop_excess_elements(parser);
                wiki_start_para_if_necessary(parser);
                wiki_append_entity_from_utf32_char(output, token->code_point);
                break;

            case END_OF_FILE:
                // special case for input like " foo\n " (see pre_spec.rb)
                if (IN(PRE) &&
                    ary_entry(parser->line_buffer, -2) == PRE &&
                    parser->pending_crlf)
                    str_append(parser->output, parser->line_ending->ptr, parser->line_ending->len);

                // close any open scopes on hitting EOF
                wiki_rollback_failed_link(parser); // if any
                wiki_pop_all_from_stack(parser);
                goto return_output; // break not enough here (want to break out of outer while loop, not inner switch statement)

            default:
                break;
        }

        // reset current token; forcing lexer to return another token at the top of the loop
        token = NULL;
    } while (1);
return_output:
    // nasty hack to avoid re-allocating our return value
    str_append(parser->output, null_str, 1); // null-terminate
    len = parser->output->len - 1; // don't count null termination

#if defined(RUBY_1_9_x)
    VALUE out = rb_str_buf_new(RSTRING_EMBED_LEN_MAX + 1);
    free(RSTRING_PTR(out));
    RSTRING(out)->as.heap.aux.capa = len;
    RSTRING(out)->as.heap.ptr = parser->output->ptr;
    RSTRING(out)->as.heap.len = len;
#elif defined(RUBY_1_8_x)
    VALUE out = rb_str_new2("");
    free(RSTRING_PTR(out));
    RSTRING(out)->len = len;
    RSTRING(out)->aux.capa = len;
    RSTRING(out)->ptr = parser->output->ptr;
#else
#error unsupported RUBY_VERSION
#endif
    parser->output->ptr = NULL; // don't double-free
    return out;
}
