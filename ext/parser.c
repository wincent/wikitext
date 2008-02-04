// Copyright 2007-2008 Wincent Colaiuta
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

#include "parser.h"
#include "wikitext.h"
#include "wikitext_ragel.h"

// inline functions for inserting literals
// have the option here of caching these in global variables, rb_obj_freeze

inline VALUE escaped_no_wiki_start(void)
{
    return rb_str_new2("&lt;nowiki&gt;");
}

inline VALUE escaped_no_wiki_end(void)
{
    return rb_str_new2("&lt;/nowiki&gt;");
}

inline VALUE literal_strong_em(void)
{
    return rb_str_new2("'''''");
}

inline VALUE literal_strong(void)
{
    return rb_str_new2("'''");
}

inline VALUE literal_em(void)
{
    return rb_str_new2("''");
}

inline VALUE escaped_tt_start(void)
{
    return rb_str_new2("&lt;tt&gt;");
}

inline VALUE escaped_tt_end(void)
{
    return rb_str_new2("&lt;/tt&gt;");
}

inline VALUE literal_h6(void)
{
    return rb_str_new2("======");
}

inline VALUE literal_h5(void)
{
    return rb_str_new2("=====");
}

inline VALUE literal_h4(void)
{
    return rb_str_new2("====");
}

inline VALUE literal_h3(void)
{
    return rb_str_new2("===");
}

inline VALUE literal_h2(void)
{
    return rb_str_new2("==");
}

inline VALUE literal_h1(void)
{
    return rb_str_new2("=");
}

inline VALUE pre_start(void)
{
    return rb_str_new2("<pre>");
}

inline VALUE pre_end(void)
{
    return rb_str_new2("</pre>");
}

inline VALUE blockquote_start(void)
{
    return rb_str_new2("<blockquote>");
}

inline VALUE blockquote_end(void)
{
    return rb_str_new2("</blockquote>");
}

inline VALUE strong_em_start(void)
{
    return rb_str_new2("<strong><em>");
}

inline VALUE strong_start(void)
{
    return rb_str_new2("<strong>");
}

inline VALUE strong_end(void)
{
    return rb_str_new2("</strong>");
}

inline VALUE em_start(void)
{
    return rb_str_new2("<em>");
}

inline VALUE em_end(void)
{
    return rb_str_new2("</em>");
}

inline VALUE tt_start(void)
{
    return rb_str_new2("<tt>");
}

inline VALUE tt_end(void)
{
    return rb_str_new2("</tt>");
}

inline VALUE ol_start(void)
{
    return rb_str_new2("<ol>");
}

inline VALUE ol_end(void)
{
    return rb_str_new2("</ol>");
}

inline VALUE ul_start(void)
{
    return rb_str_new2("<ul>");
}

inline VALUE ul_end(void)
{
    return rb_str_new2("</ul>");
}

inline VALUE li_start(void)
{
    return rb_str_new2("<li>");
}

inline VALUE li_end(void)
{
    return rb_str_new2("</li>");
}

inline VALUE h6_start(void)
{
    return rb_str_new2("<h6>");
}

inline VALUE h6_end(void)
{
    return rb_str_new2("</h6>");
}

inline VALUE h5_start(void)
{
    return rb_str_new2("<h5>");
}

inline VALUE h5_end(void)
{
    return rb_str_new2("</h5>");
}

inline VALUE h4_start(void)
{
    return rb_str_new2("<h4>");
}

inline VALUE h4_end(void)
{
    return rb_str_new2("</h4>");
}

inline VALUE h3_start(void)
{
    return rb_str_new2("<h3>");
}

inline VALUE h3_end(void)
{
    return rb_str_new2("</h3>");
}

inline VALUE h2_start(void)
{
    return rb_str_new2("<h2>");
}

inline VALUE h2_end(void)
{
    return rb_str_new2("</h2>");
}

inline VALUE h1_start(void)
{
    return rb_str_new2("<h1>");
}

inline VALUE h1_end(void)
{
    return rb_str_new2("</h1>");
}

inline VALUE p_start(void)
{
    return rb_str_new2("<p>");
}

inline VALUE p_end(void)
{
    return rb_str_new2("</p>");
}

inline VALUE space(void)
{
    return rb_str_new2(" ");
}

inline VALUE a_start(void)
{
    return rb_str_new2("<a href=\"");
}

inline VALUE a_class(void)
{
    return rb_str_new2("\" class=\"");
}

inline VALUE a_start_close(void)
{
    return rb_str_new2("\">");
}

inline VALUE a_end(void)
{
    return rb_str_new2("</a>");
}

inline VALUE link_start(void)
{
    return rb_str_new2("[[");
}

inline VALUE link_end(void)
{
    return rb_str_new2("]]");
}

inline VALUE separator(void)
{
    return rb_str_new2("|");
}

inline VALUE ext_link_start(void)
{
    return rb_str_new2("[");
}

inline VALUE backtick(void)
{
    return rb_str_new2("`");
}

inline VALUE quote(void)
{
    return rb_str_new2("\"");
}

inline VALUE ampersand(void)
{
    return rb_str_new2("&");
}

inline VALUE quot_entity(void)
{
    return rb_str_new2("&quot;");
}

inline VALUE amp_entity(void)
{
    return rb_str_new2("&amp;");
}

inline VALUE lt_entity(void)
{
    return rb_str_new2("&lt;");
}

inline VALUE gt_entity(void)
{
    return rb_str_new2("&gt;");
}

inline VALUE ext_link_end(void)
{
    return rb_str_new2("]");
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
    rb_ary_push(tokens, _Wikitext_token(&token));
    while (token.type != END_OF_FILE)
    {
        next_token(&token, &token, NULL, pe);
        rb_ary_push(tokens, _Wikitext_token(&token));
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

// we downcase "in place", overwriting the original contents of the buffer and returning the same string
inline VALUE _Wikitext_downcase(VALUE string)
{
    char *ptr   = RSTRING_PTR(string);
    long len    = RSTRING_LEN(string);
    for (long i = 0; i < len; i++)
    {
        if (ptr[i] >= 'A' && ptr[i] <= 'Z')
            ptr[i] += 32;
    }
    return string;
}

inline VALUE _Wikitext_hyperlink(VALUE link_prefix, VALUE link_target, VALUE link_text, VALUE link_class)
{
    VALUE string = a_start();               // <a href="
    if (!NIL_P(link_prefix))
        rb_str_append(string, link_prefix);
    rb_str_append(string, link_target);
    if (link_class != Qnil)
    {
        rb_str_append(string, a_class());   // " class="
        rb_str_append(string, link_class);
    }
    rb_str_append(string, a_start_close()); // ">
    rb_str_append(string, link_text);
    rb_str_append(string, a_end());
    return string;
}

// Returns a count indicating the number of times the token appears in the collection.
inline long _Wikitext_count(VALUE token, VALUE collection)
{
    long count = 0;
    for (long i = 0, max = RARRAY_LEN(collection); i < max; i++)
    {
        if (FIX2INT(rb_ary_entry(collection, i)) == FIX2INT(token))
            count++;
    }
    return count;
}

// Pops a single item off the stack.
// A corresponding closing tag is written to the target string.
void _Wikitext_pop_from_stack(VALUE stack, VALUE target, VALUE line_ending)
{
    VALUE top = rb_ary_entry(stack, -1);
    if (NIL_P(top))
        return;
    switch (FIX2INT(top))
    {
        case PRE:
            rb_str_append(target, pre_end());
            rb_str_append(target, line_ending);
            break;

        case BLOCKQUOTE:
            rb_str_append(target, blockquote_end());
            rb_str_append(target, line_ending);
            break;

        case NO_WIKI_START:
            // not a real HTML tag; so nothing to pop
            break;

        case STRONG:
            rb_str_append(target, strong_end());
            break;

        case EM:
            rb_str_append(target, em_end());
            break;

        case TT:
        case TT_START:
            rb_str_append(target, tt_end());
            break;

        case OL:
            rb_str_append(target, ol_end());
            rb_str_append(target, line_ending);
            break;

        case UL:
            rb_str_append(target, ul_end());
            rb_str_append(target, line_ending);
            break;

        case LI:
            rb_str_append(target, li_end());
            rb_str_append(target, line_ending);
            break;

        case H6_START:
            rb_str_append(target, h6_end());
            rb_str_append(target, line_ending);
            break;

        case H5_START:
            rb_str_append(target, h5_end());
            rb_str_append(target, line_ending);
            break;

        case H4_START:
            rb_str_append(target, h4_end());
            rb_str_append(target, line_ending);
            break;

        case H3_START:
            rb_str_append(target, h3_end());
            rb_str_append(target, line_ending);
            break;

        case H2_START:
            rb_str_append(target, h2_end());
            rb_str_append(target, line_ending);
            break;

        case H1_START:
            rb_str_append(target, h1_end());
            rb_str_append(target, line_ending);
            break;

        case EXT_LINK_START:
            // not an HTML tag; so nothing to emit
            break;

        case SPACE:
            // not an HTML tag (only used to separate an external link target from the link text); so nothing to emit
            break;

        case SEPARATOR:
            // not an HTML tag (only used to separate an external link target from the link text); so nothing to emit
            break;

        case P:
            rb_str_append(target, p_end());
            rb_str_append(target, line_ending);
            break;

        default:
            // should probably raise an exception here
            break;
    }
    rb_ary_delete_at(stack, -1);
}

// Pops items off top of stack, accumulating closing tags for them into the target string, until item is reached.
// If including is Qtrue then the item itself is also popped.
void _Wikitext_pop_from_stack_up_to(VALUE stack, VALUE target, VALUE item, VALUE including, VALUE line_ending)
{
    int continue_looping = 1;
    do
    {
        VALUE top = rb_ary_entry(stack, -1);
        if (NIL_P(top))
            return;
        if (FIX2INT(top) == FIX2INT(item))
        {
            if (including != Qtrue)
                return;
            continue_looping = 0;
        }
        _Wikitext_pop_from_stack(stack, target, line_ending);
    } while (continue_looping);
}

inline void _Wikitext_start_para_if_necessary(VALUE capture, VALUE scope, VALUE line, VALUE output,
    VALUE *pending_crlf)
{
    if (!NIL_P(capture)) // we don't do anything if capturing mode
        return;
    // if no block open yet, or top of stack is BLOCKQUOTE (with nothing in it yet)
    if ((RARRAY_LEN(scope) == 0) || (FIX2INT(rb_ary_entry(scope, -1)) == BLOCKQUOTE))
    {
        rb_str_append(output, p_start());
        rb_ary_push(scope, INT2FIX(P));
        rb_ary_push(line, INT2FIX(P));
    }
    else if (rb_ary_includes(scope, INT2FIX(P)) && *pending_crlf == Qtrue)
        // already in a paragraph block; convert pending CRLF into a space
        rb_str_append(output, space());
    *pending_crlf = Qfalse;
}

// Helper function that pops any excess elements off scope (pushing is already handled in the respective rules).
// For example, given input like:
//      > > foo
//      bar
// Upon seeing "bar", we want to pop two BLOCKQUOTE elements from the scope.
// The reverse case (shown below) is handled from inside the BLOCKQUOTE rule itself:
//      foo
//      > > bar
void inline _Wikitext_pop_excess_elements(VALUE capture, VALUE scope, VALUE line, VALUE output, VALUE line_ending)
{
    if (!NIL_P(capture)) // we don't pop anything if capturing mode
        return;
    for (long i = RARRAY_LEN(scope), j = RARRAY_LEN(line); i > j; i--)
    {
        // special case for last item on scope
        if (i - j == 1)
        {
            // don't auto-pop P if it is only item on scope
            long k = FIX2INT(rb_ary_entry(scope, -1));
            if (k == P)
                continue;
            else if (k != FIX2INT(rb_ary_entry(line, -1)))
            {
                // pop off one more item in cases like this:
                // * foo
                //   pre
                // seems necessary in the PRE case becase there's something braindead with my PRE implementation
                // other rules (eg BLOCKQUOTE, H6 etc) seem to handle this fine
                _Wikitext_pop_from_stack(scope, output, line_ending);
            }
        }
        _Wikitext_pop_from_stack(scope, output, line_ending);
    }
}

inline VALUE _Wikitext_utf32_char_to_entity(uint32_t character)
{
    // TODO: consider special casing some entities (ie. quot, amp, lt, gt etc)?
    char hex_string[8]  = { '&', '#', 'x', 0, 0, 0, 0, ';' };
    char scratch        = (character & 0xf000) >> 12;
    hex_string[3]       = (scratch <= 9 ? scratch + 48 : scratch + 87);
    scratch             = (character & 0x0f00) >> 8;
    hex_string[4]       = (scratch <= 9 ? scratch + 48 : scratch + 87);
    scratch             = (character & 0x00f0) >> 4;
    hex_string[5]       = (scratch <= 9 ? scratch + 48 : scratch + 87);
    scratch             = character & 0x000f;
    hex_string[6]       = (scratch <= 9 ? scratch + 48 : scratch + 87);
    return rb_str_new((const char *)hex_string, sizeof(hex_string));
}

// - non-printable (non-ASCII) characters converted to numeric entities
// - QUOT and AMP characters converted to named entities
inline VALUE _Wikitext_sanitize_link_target(VALUE self, VALUE string)
{
    string              = StringValue(string);  // raises if string is nil or doesn't quack like a string
    char    *src        = RSTRING_PTR(string);
    long    len         = RSTRING_LEN(string);
    char    *end        = src + (len / sizeof(uint16_t));

    // start with a destination buffer twice the size of the source, will realloc if necessary
    // slop = (len / 8) * 8 (ie. one in every 8 characters can be converted into an entity, each entity requires 8 bytes)
    // this efficiently handles the most common case (where the size of the buffer doesn't change much)
    char    *dest       = ALLOC_N(char, len * 2);
    char    *dest_ptr   = dest; // hang on to this so we can pass it to free() later

    while (src < end)
    {
        // need at most 8 characters (8 bytes) to display each character
        if (dest + 8 > dest_ptr + len)                      // outgrowing buffer, must reallocate
        {
            char *old_dest      = dest;
            char *old_dest_ptr  = dest_ptr;
            len                 = len + (end - src) * 8;    // allocate enough for worst case
            dest                = realloc(dest_ptr, len);   // will never have to realloc more than once
            if (dest == NULL)
            {
                // would have used reallocf, but this has to run on Linux too, not just Darwin
                free(dest_ptr);
                rb_raise(rb_eNoMemError, "failed to re-allocate temporary storage (memory allocation error)");
            }
            dest_ptr    = dest;
            dest        = dest_ptr + (old_dest - old_dest_ptr);
        }

        if (*src == '"')                 // QUOT
        {
            char *quot_entity_literal = "&quot;";
            memcpy(dest, quot_entity_literal, sizeof(quot_entity_literal));
            dest += sizeof(quot_entity_literal);
        }
        else if (*src == '&')            // AMP
        {
            char *amp_entity_literal = "&amp;";
            memcpy(dest, amp_entity_literal, sizeof(amp_entity_literal));
            dest += sizeof(amp_entity_literal);
        }
        else if (*src == '<')           // LESS_THAN
        {
            free(dest_ptr);
            rb_raise(rb_eRangeError, "invalid link text (\"<\" may not appear in link text)");
        }
        else if (*src == '>')           // GREATER_THAN
        {
            free(dest_ptr);
            rb_raise(rb_eRangeError, "invalid link text (\">\" may not appear in link text)");
        }
        else if (*src >= 0x20 && *src <= 0x7e)    // printable ASCII
        {
            *dest = *src;
            dest++;
        }
        else    // all others: must convert to entities
        {
            VALUE       entity      = _Wikitext_utf32_char_to_entity(*src);
            char        *entity_src = RSTRING_PTR(entity);
            long        entity_len  = RSTRING_LEN(entity); // should always be 8 characters (8 bytes)
            memcpy(dest, entity_src, entity_len);
            dest += entity_len;
        }
        src++;
    }
    VALUE out = rb_str_new(dest_ptr, dest - dest_ptr);
    free(dest_ptr);
    return out;
}

VALUE Wikitext_sanitize_link_target(VALUE self, VALUE string)
{
    return (_Wikitext_sanitize_link_target(self, string));
}

// encodes the input string according to RFCs 2396 and 2718
// input is the pointer to the string, and len is its length in characters (not in bytes)
// note that the first character of the target link is not case-sensitive
// (this is a recommended application-level constraint; it is not imposed at this level)
// this is to allow links like:
//         ...the [[foo]] is...
// to be equivalent to:
//         thing. [[Foo]] was...
// TODO: this is probably the right place to check if treat_slash_as_special is true and act accordingly
inline static VALUE _Wikitext_encode_link_target(VALUE self, VALUE in)
{
    char        *input  = RSTRING_PTR(in);
    long        len     = RSTRING_LEN(in);
    static char hex[]   = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

    // to avoid most reallocations start with a destination buffer twice the size of the source
    // this handles the most common case (where most chars are in the ASCII range and don't require more storage, but there are
    // often quite a few spaces, which are encoded as "%20" and occupy 3 bytes)
    // the worst case is where _every_ byte must be written out using 3 bytes
    long dest_len   = len * 2;
    char *dest      = ALLOC_N(char, dest_len);
    char *dest_ptr  = dest; // hang on to this so we can pass it to free() later

    for (long i = 0; i < len; input++)
    {
        if ((dest + 3) > (dest_ptr + dest_len))     // worst case: a single character may grow to 3 characters once encoded
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
        }

        // pass through unreserved characters
        if (((*input >= 'a') && (*input <= 'z')) ||
            ((*input >= 'A') && (*input <= 'Z')) ||
            ((*input >= '0') && (*input <= '9')) ||
            (*input == '-') ||
            (*input == '_') ||
            (*input == '.') ||
            (*input == '~'))
            *dest++ = *input;
        else    // everything else gets URL-encoded
        {
            *dest++ = '%';
            *dest++ = hex[(*input) / 16];   // left
            *dest++ = hex[(*input) % 16];   // right
        }
    }
    VALUE out = rb_str_new(dest_ptr, dest - dest_ptr);
    free(dest_ptr);
    return out;
}

VALUE Wikitext_encode_link_target(VALUE self, VALUE in)
{
    return _Wikitext_encode_link_target(self, in);
}

// not sure whether these rollback functions should be inline: could refactor them into a single non-inlined function
inline void _Wikitext_rollback_failed_link(VALUE output, VALUE scope, VALUE line, VALUE link_target, VALUE link_text,
    VALUE link_class, VALUE line_ending)
{
    // I'd like to remove this paragraph creation from here and instead put it where the scope is first entered: would be cleaner
    // same for the method below
    // basically we can create a paragraph at that point because we know we'll either be emitting a valid link or the residue
    // left behind by an invalid one
    int scope_includes_separator = rb_ary_includes(scope, INT2FIX(SEPARATOR));
    _Wikitext_pop_from_stack_up_to(scope, output, INT2FIX(LINK_START), Qtrue, line_ending);
    if (!rb_ary_includes(scope, INT2FIX(P)) &&
        !rb_ary_includes(scope, INT2FIX(H6_START)) &&
        !rb_ary_includes(scope, INT2FIX(H5_START)) &&
        !rb_ary_includes(scope, INT2FIX(H4_START)) &&
        !rb_ary_includes(scope, INT2FIX(H3_START)) &&
        !rb_ary_includes(scope, INT2FIX(H2_START)) &&
        !rb_ary_includes(scope, INT2FIX(H1_START)))
    {
        // create a paragraph if necessary
        rb_str_append(output, p_start());
        rb_ary_push(scope, INT2FIX(P));
        rb_ary_push(line, INT2FIX(P));
    }
    rb_str_append(output, link_start());
    if (!NIL_P(link_target))
    {
        VALUE sanitized = Wikitext_sanitize_link_target(Qnil, link_target);
        rb_str_append(output, sanitized);
        if (scope_includes_separator)
        {
            rb_str_append(output, separator());
            if (!NIL_P(link_text))
                rb_str_append(output, link_text);
        }
    }
}

inline void _Wikitext_rollback_failed_external_link(VALUE output, VALUE scope, VALUE line, VALUE link_target,
    VALUE link_text, VALUE link_class, VALUE autolink, VALUE line_ending)
{
    int scope_includes_space = rb_ary_includes(scope, INT2FIX(SPACE));
    _Wikitext_pop_from_stack_up_to(scope, output, INT2FIX(EXT_LINK_START), Qtrue, line_ending);
    if (!rb_ary_includes(scope, INT2FIX(P)) &&
        !rb_ary_includes(scope, INT2FIX(H6_START)) &&
        !rb_ary_includes(scope, INT2FIX(H5_START)) &&
        !rb_ary_includes(scope, INT2FIX(H4_START)) &&
        !rb_ary_includes(scope, INT2FIX(H3_START)) &&
        !rb_ary_includes(scope, INT2FIX(H2_START)) &&
        !rb_ary_includes(scope, INT2FIX(H1_START)))
    {
        // create a paragraph if necessary
        rb_str_append(output, p_start());
        rb_ary_push(scope, INT2FIX(P));
        rb_ary_push(line, INT2FIX(P));
    }
    rb_str_append(output, ext_link_start());
    if (!NIL_P(link_target))
    {
        if (autolink == Qtrue)
            link_target = _Wikitext_hyperlink(Qnil, link_target, link_target, link_class); // link target, link text, link class
        rb_str_append(output, link_target);
        if (scope_includes_space)
        {
            rb_str_append(output, space());
            if (!NIL_P(link_text))
                rb_str_append(output, link_text);
        }
    }
}

VALUE Wikitext_parser_initialize(VALUE self)
{
    // no need to call super here; rb_call_super()
    rb_iv_set(self, "@autolink",                Qtrue);
    rb_iv_set(self, "@line_ending",             rb_str_new2("\n"));
    rb_iv_set(self, "@external_link_class",     rb_str_new2("external"));
    rb_iv_set(self, "@mailto_class",            rb_str_new2("mailto"));
    rb_iv_set(self, "@internal_link_prefix",    rb_str_new2("/wiki/"));
    return self;
}

VALUE Wikitext_parser_parse(VALUE self, VALUE string)
{
    // process arguments
    if (NIL_P(string))
        return Qnil;
    string = StringValue(string);

    // set up scanner
    char *p = RSTRING_PTR(string);
    long len = RSTRING_LEN(string);
    char *pe = p + len;

    // house-keeping
    VALUE output        = rb_str_new2("");
    VALUE capture       = Qnil;             // sometimes we want to capture output rather than send it to the output variable
    VALUE scope         = rb_ary_new();     // stack for tracking scope
    VALUE line          = rb_ary_new();     // stack for tracking scope as implied by current line
    VALUE line_buffer   = rb_ary_new();     // stack for tracking raw tokens (not scope) on current line
    VALUE pending_crlf  = Qfalse;
    VALUE link_target   = Qnil;             // need some short term "memory" for parsing links
    VALUE link_text     = Qnil;             // need some short term "memory" for parsing links

    // access these once per parse
    VALUE line_ending   = rb_iv_get(self, "@line_ending");
    line_ending         = StringValue(line_ending);
    VALUE autolink      = rb_iv_get(self, "@autolink");
    VALUE link_class    = rb_iv_get(self, "@external_link_class");
    link_class          = NIL_P(link_class) ? Qnil : StringValue(link_class);
    VALUE mailto_class  = rb_iv_get(self, "@mailto_class");
    mailto_class        = StringValue(mailto_class);
    VALUE prefix        = rb_iv_get(self, "@internal_link_prefix");

    token_t _token;
    _token.type = NO_TOKEN;
    token_t *token = NULL;
    do
    {
        // check to see if we have a token hanging around from a previous iteration of this loop
        if (token == NULL)
        {
            token = &_token;
            if (_token.type == NO_TOKEN)
                // first time here (haven't started scanning yet)
                next_token(token, NULL, p, pe);
            else
                // already scanning
#define NEXT_TOKEN()    next_token(token, token, NULL, pe)
                NEXT_TOKEN();
        }
        int type = token->type;

        // many restrictions depend on what is at the top of the stack
        VALUE top = rb_ary_entry(scope, -1);

        if (type != END_OF_FILE)
        {
            // push current token into line buffer (but not EOF as it won't fit inside a Fixnum)
            // provides us with context-sensitive "memory" of what's been seen so far on this line
            VALUE current = INT2FIX(type);

            // for lines with *lots* of consecutive PRINTABLES this could be quite wasteful, so only store one
            //if (type != PRINTABLE || NIL_P(top) || FIX2INT(top) != PRINTABLE)
                rb_ary_push(line_buffer, current);
        }

        // can't declare new variables inside a switch statement, so predeclare them here
        long remove_strong          = -1;
        long remove_em              = -1;

        // general purpose counters and flags
        long i                      = 0;
        long j                      = 0;
        long k                      = 0;

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
                if (rb_ary_includes(scope, INT2FIX(NO_WIKI_START)))
                {
                    // already in <nowiki> span (no need to check for <pre>; can never appear inside it)
                    rb_str_append(output, space());
                    break;
                }

                // count number of BLOCKQUOTE tokens in line buffer and in scope stack
                rb_ary_push(line, INT2FIX(PRE));
                i = _Wikitext_count(INT2FIX(BLOCKQUOTE), line);
                j = _Wikitext_count(INT2FIX(BLOCKQUOTE), scope);

                if (i < j)
                {
                    // must pop (reduce nesting level)
                    for (i = j - i; i > 0; i--)
                        _Wikitext_pop_from_stack_up_to(scope, output, INT2FIX(BLOCKQUOTE), Qtrue, line_ending);
                }

                if (!rb_ary_includes(scope, INT2FIX(PRE)))
                {
                    _Wikitext_pop_excess_elements(capture, scope, line, output, line_ending);
                    rb_str_append(output, pre_start());
                    rb_ary_push(scope, INT2FIX(PRE));
                }
                break;

            case BLOCKQUOTE:
                if (rb_ary_includes(scope, INT2FIX(NO_WIKI_START)))
                    // already in <nowiki> span (no need to check for <pre>; can never appear inside it)
                    rb_str_append(output, TOKEN_TEXT(token));
                else
                {
                    rb_ary_push(line, INT2FIX(BLOCKQUOTE));

                    // count number of BLOCKQUOTE tokens in line buffer and in scope stack
                    i = _Wikitext_count(INT2FIX(BLOCKQUOTE), line);
                    j = _Wikitext_count(INT2FIX(BLOCKQUOTE), scope);

                    // given that BLOCKQUOTE tokens can be nested, peek ahead and see if there are any more which might affect the decision to push or pop
                    while (NEXT_TOKEN(), (token->type == BLOCKQUOTE))
                    {
                        rb_ary_push(line, INT2FIX(BLOCKQUOTE));
                        i++;
                    }

                    // now decide whether to push, pop or do nothing
                    if (i > j)
                    {
                        // must push (increase nesting level)
                        _Wikitext_pop_from_stack_up_to(scope, output, INT2FIX(BLOCKQUOTE), Qfalse, line_ending);
                        for (i = i - j; i > 0; i--)
                        {
                            rb_str_append(output, blockquote_start());
                            rb_ary_push(scope, INT2FIX(BLOCKQUOTE));
                        }
                    }
                    else if (i < j)
                    {
                        // must pop (reduce nesting level)
                        for (i = j - i; i > 0; i--)
                            _Wikitext_pop_from_stack_up_to(scope, output, INT2FIX(BLOCKQUOTE), Qtrue, line_ending);
                    }

                    // jump to top of the loop to process token we scanned during lookahead
                    continue;
                }
                break;

            case NO_WIKI_START:
                if (rb_ary_includes(scope, INT2FIX(NO_WIKI_START)) || rb_ary_includes(scope, INT2FIX(PRE)))
                    // already in <nowiki> span or <pre> block
                    rb_str_append(output, escaped_no_wiki_start());
                else
                {
                    i = NIL_P(capture) ? output : capture;
                    _Wikitext_pop_excess_elements(capture, scope, line, i, line_ending);
                    _Wikitext_start_para_if_necessary(capture, scope, line, i, &pending_crlf);
                    rb_ary_push(scope, INT2FIX(NO_WIKI_START));
                    rb_ary_push(line, INT2FIX(NO_WIKI_START));
                }
                break;

            case NO_WIKI_END:
                if (rb_ary_includes(scope, INT2FIX(NO_WIKI_START)))
                    // <nowiki> should always only ever be the last item in the stack, but use the helper routine just in case
                    _Wikitext_pop_from_stack_up_to(scope, output, INT2FIX(NO_WIKI_START), Qtrue, line_ending);
                else
                {
                    i = NIL_P(capture) ? output : capture;
                    _Wikitext_pop_excess_elements(capture, scope, line, i, line_ending);
                    _Wikitext_start_para_if_necessary(capture, scope, line, i, &pending_crlf);
                    rb_str_append(output, escaped_no_wiki_end());
                }
                break;

            case STRONG_EM:
                if (rb_ary_includes(scope, INT2FIX(NO_WIKI_START)) || rb_ary_includes(scope, INT2FIX(PRE)))
                {
                    // already in <nowiki> span or <pre> block
                    rb_str_append(output, literal_strong_em());
                    break;
                }

                i = NIL_P(capture) ? output : capture;
                _Wikitext_pop_excess_elements(capture, scope, line, i, line_ending);

                // if you've seen STRONG or EM, must close them in the reverse order that you saw them! otherwise, must open them
                remove_strong  = -1;
                remove_em      = -1;
                j              = RARRAY_LEN(scope);
                for (j = j - 1; j >= 0; j--)
                {
                    long val = FIX2INT(rb_ary_entry(scope, j));
                    if (val == STRONG)
                    {
                        rb_str_append(i, strong_end());
                        remove_strong = j;
                    }
                    else if (val == EM)
                    {
                        rb_str_append(i, em_end());
                        remove_em = j;
                    }
                }

                if (remove_strong > remove_em)      // must remove strong first
                {
                    rb_ary_delete_at(scope, remove_strong);
                    if (remove_em > -1)
                        rb_ary_delete_at(scope, remove_em);
                    else    // there was no em to remove!, so consider this an opening em tag
                    {
                        rb_str_append(i, em_start());
                        rb_ary_push(scope, INT2FIX(EM));
                        rb_ary_push(line, INT2FIX(EM));
                    }
                }
                else if (remove_em > remove_strong) // must remove em first
                {
                    rb_ary_delete_at(scope, remove_em);
                    if (remove_strong > -1)
                        rb_ary_delete_at(scope, remove_strong);
                    else    // there was no strong to remove!, so consider this an opening strong tag
                    {
                        rb_str_append(i, strong_start());
                        rb_ary_push(scope, INT2FIX(STRONG));
                        rb_ary_push(line, INT2FIX(STRONG));
                    }
                }
                else    // no strong or em to remove, so this must be a new opening of both
                {
                    _Wikitext_start_para_if_necessary(capture, scope, line, i, &pending_crlf);
                    rb_str_append(i, strong_em_start());
                    rb_ary_push(scope, INT2FIX(STRONG));
                    rb_ary_push(line, INT2FIX(STRONG));
                    rb_ary_push(scope, INT2FIX(EM));
                    rb_ary_push(line, INT2FIX(EM));
                }
                break;

            case STRONG:
                if (rb_ary_includes(scope, INT2FIX(NO_WIKI_START)) || rb_ary_includes(scope, INT2FIX(PRE)))
                    // already in <nowiki> span or <pre> block
                    rb_str_append(output, literal_strong());
                else
                {
                    i = NIL_P(capture) ? output : capture;
                    if (rb_ary_includes(scope, INT2FIX(STRONG)))
                        // STRONG already seen, this is a closing tag
                        _Wikitext_pop_from_stack_up_to(scope, i, INT2FIX(STRONG), Qtrue, line_ending);
                    else
                    {
                        // this is a new opening
                        _Wikitext_pop_excess_elements(capture, scope, line, i, line_ending);
                        _Wikitext_start_para_if_necessary(capture, scope, line, i, &pending_crlf);
                        rb_str_append(i, strong_start());
                        rb_ary_push(scope, INT2FIX(STRONG));
                        rb_ary_push(line, INT2FIX(STRONG));
                    }
                }
                break;

            case EM:
                if (rb_ary_includes(scope, INT2FIX(NO_WIKI_START)) || rb_ary_includes(scope, INT2FIX(PRE)))
                    // already in <nowiki> span or <pre> block
                    rb_str_append(output, literal_em());
                else
                {
                    i = NIL_P(capture) ? output : capture;
                    if (rb_ary_includes(scope, INT2FIX(EM)))
                        // EM already seen, this is a closing tag
                        _Wikitext_pop_from_stack_up_to(scope, i, INT2FIX(EM), Qtrue, line_ending);
                    else
                    {
                        // this is a new opening
                        _Wikitext_pop_excess_elements(capture, scope, line, i, line_ending);
                        _Wikitext_start_para_if_necessary(capture, scope, line, i, &pending_crlf);
                        rb_str_append(i, em_start());
                        rb_ary_push(scope, INT2FIX(EM));
                        rb_ary_push(line, INT2FIX(EM));
                    }
                }
                break;

            case TT:
                if (rb_ary_includes(scope, INT2FIX(NO_WIKI_START)) || rb_ary_includes(scope, INT2FIX(PRE)))
                    // already in <nowiki> span or <pre> block
                    rb_str_append(output, backtick());
                else
                {
                    i = NIL_P(capture) ? output : capture;
                    if (rb_ary_includes(scope, INT2FIX(TT_START)))
                        // already in span started with <tt>, no choice but to emit this literally
                        rb_str_append(output, backtick());
                    else if (rb_ary_includes(scope, INT2FIX(TT)))
                        // TT (`) already seen, this is a closing tag
                        _Wikitext_pop_from_stack_up_to(scope, i, INT2FIX(TT), Qtrue, line_ending);
                    else
                    {
                        // this is a new opening
                        _Wikitext_pop_excess_elements(capture, scope, line, i, line_ending);
                        _Wikitext_start_para_if_necessary(capture, scope, line, i, &pending_crlf);
                        rb_str_append(i, tt_start());
                        rb_ary_push(scope, INT2FIX(TT));
                        rb_ary_push(line, INT2FIX(TT));
                    }
                }
                break;

            case TT_START:
                if (rb_ary_includes(scope, INT2FIX(NO_WIKI_START)) || rb_ary_includes(scope, INT2FIX(PRE)))
                    // already in <nowiki> span, <pre> block
                    rb_str_append(output, escaped_tt_start());
                else
                {
                    i = NIL_P(capture) ? output : capture;
                    if (rb_ary_includes(scope, INT2FIX(TT_START)) || rb_ary_includes(scope, INT2FIX(TT)))
                        // already in TT_START (<tt>) or TT (`) span)
                        rb_str_append(output, escaped_tt_start());
                    else
                    {
                        _Wikitext_pop_excess_elements(capture, scope, line, i, line_ending);
                        _Wikitext_start_para_if_necessary(capture, scope, line, i, &pending_crlf);
                        rb_str_append(i, tt_start());
                        rb_ary_push(scope, INT2FIX(TT_START));
                        rb_ary_push(line, INT2FIX(TT_START));
                    }
                }
                break;

            case TT_END:
                if (rb_ary_includes(scope, INT2FIX(NO_WIKI_START)) || rb_ary_includes(scope, INT2FIX(PRE)))
                    // already in <nowiki> span or <pre> block
                    rb_str_append(output, escaped_tt_end());
                else
                {
                    i = NIL_P(capture) ? output : capture;
                    if (rb_ary_includes(scope, INT2FIX(TT_START)))
                        _Wikitext_pop_from_stack_up_to(scope, i, INT2FIX(TT_START), Qtrue, line_ending);
                    else
                    {
                        // no TT_START in scope, so must interpret the TT_END without any special meaning
                        _Wikitext_pop_excess_elements(capture, scope, line, i, line_ending);
                        _Wikitext_start_para_if_necessary(capture, scope, line, i, &pending_crlf);
                        rb_str_append(i, escaped_tt_end());
                    }
                }
                break;

            case OL:
            case UL:
                if (rb_ary_includes(scope, INT2FIX(NO_WIKI_START)))
                {
                    // already in <nowiki> span (no need to check for <pre>; can never appear inside it)
                    rb_str_append(output, TOKEN_TEXT(token));
                    break;
                }

                // count number of tokens in line and scope stacks
                i = RARRAY_LEN(line);
                j = RARRAY_LEN(scope);

                // list tokens can be nested so look ahead for any more which might affect the decision to push or pop
                for (;;)
                {
                    NEXT_TOKEN();
                    type = token->type;
                    if (type == OL || type == UL)
                    {
                        token = NULL;
                        rb_ary_push(line, INT2FIX(type));
                        rb_ary_push(line, INT2FIX(LI));
                        i += 2;

                        // want to compare line with scope but can only do so if scope has enough items on it
                        if (j >= i)
                        {
                            if ((FIX2INT(rb_ary_entry(scope, i - 2)) == type) && (FIX2INT(rb_ary_entry(scope, i - 1)) == LI))
                            {
                                // line and scope match at this point: do nothing yet
                            }
                            else
                            {
                                // item just pushed onto line does not match corresponding slot of scope!
                                for (; j >= i - 2; j--)
                                    // must pop back before emitting
                                    _Wikitext_pop_from_stack(scope, output, line_ending);

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
                            _Wikitext_pop_from_stack(scope, output, line_ending);
                        else if (j > i)
                        {
                            // item just pushed onto line does not match corresponding slot of scope!
                            for (; j >= i; j--)
                                // must pop back before emitting
                                _Wikitext_pop_from_stack(scope, output, line_ending);
                        }
                        break;
                    }
                }

                // TODO: consider adding indentation here... wouldn't be too hard...
                if (type == OL || type == UL)
                {
                    // if LI is at the top of a stack this is the start of a nested list
                    if (FIX2INT(rb_ary_entry(scope, -1)) == LI)
                        // so we should precede it with a CRLF
                        rb_str_append(output, line_ending);
                }

                // emit
                if (type == OL)
                    rb_str_append(output, ol_start());
                else if (type == UL)
                    rb_str_append(output, ul_start());

                if (type == OL || type == UL)
                {
                    rb_ary_push(scope, INT2FIX(type));
                    rb_str_append(output, line_ending);
                }
                else if (type == SPACE)
                    // silently throw away the optional SPACE token after final list marker
                    token = NULL;

                rb_str_append(output, li_start());
                rb_ary_push(scope, INT2FIX(LI));

                // any subsequent UL or OL tokens on this line are syntax errors and must be emitted literally
                if (type == OL || type == UL)
                {
                    k = 0;
                    while (NEXT_TOKEN(), (type = token->type))
                    {
                        if (k == 0 && type == SPACE)
                            // silently throw away the optional SPACE token after final list marker
                            token = NULL;
                        k++;
                        if (type == OL || type == UL)
                        {
                            rb_str_append(output, TOKEN_TEXT(token));
                            token = NULL;
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
                if (rb_ary_includes(scope, INT2FIX(NO_WIKI_START)))
                {
                    // already in <nowiki> span (no need to check for <pre>; can never appear inside it)
                    rb_str_append(output, TOKEN_TEXT(token));
                    break;
                }

                // pop up to but not including the last BLOCKQUOTE on the scope stack
                _Wikitext_pop_from_stack_up_to(scope, output, INT2FIX(BLOCKQUOTE), Qfalse, line_ending);

                // count number of BLOCKQUOTE tokens in line buffer and in scope stack
                rb_ary_push(line, INT2FIX(type));
                i = _Wikitext_count(INT2FIX(BLOCKQUOTE), line);
                j = _Wikitext_count(INT2FIX(BLOCKQUOTE), scope);

                // decide whether we need to pop off excess BLOCKQUOTE tokens (will never need to push; that is handled above in the BLOCKQUOTE case itself)
                if (i < j)
                {
                    // must pop (reduce nesting level)
                    for (i = j - i; i > 0; i--)
                        _Wikitext_pop_from_stack_up_to(scope, output, INT2FIX(BLOCKQUOTE), Qtrue, line_ending);
                }

                // discard any whitespace here (so that "== foo ==" will be translated to "<h2>foo</h2>" rather than "<h2> foo </h2")
                while (NEXT_TOKEN(), (token->type == SPACE))
                    ; // discard

                rb_ary_push(scope, INT2FIX(type));

                // rather than repeat all that code for each kind of heading, share it and use a conditional here
                if (type == H6_START)
                    rb_str_append(output, h6_start());
                else if (type == H5_START)
                    rb_str_append(output, h5_start());
                else if (type == H4_START)
                    rb_str_append(output, h4_start());
                else if (type == H3_START)
                    rb_str_append(output, h3_start());
                else if (type == H2_START)
                    rb_str_append(output, h2_start());
                else if (type == H1_START)
                    rb_str_append(output, h1_start());

                // jump to top of the loop to process token we scanned during lookahead
                continue;

            case H6_END:
                if (rb_ary_includes(scope, INT2FIX(NO_WIKI_START)) || rb_ary_includes(scope, INT2FIX(PRE)))
                    // already in <nowiki> span or <pre> block
                    rb_str_append(output, literal_h6());
                else
                {
                    if (rb_ary_includes(scope, INT2FIX(EXT_LINK_START)))
                    {
                        // this is a syntax error; an unclosed external link
                        _Wikitext_rollback_failed_external_link(output, scope, line, link_target, link_text, link_class, autolink,
                            line_ending);
                        link_target = Qnil;
                        link_text   = Qnil;
                        capture     = Qnil;
                    }

                    if (!rb_ary_includes(scope, INT2FIX(H6_START)))
                    {
                        // literal output only if not in h6 scope (we stay silent in that case)
                        _Wikitext_start_para_if_necessary(capture, scope, line, output, &pending_crlf);
                        rb_str_append(output, literal_h6());
                    }
                }
                break;

            case H5_END:
                if (rb_ary_includes(scope, INT2FIX(NO_WIKI_START)) || rb_ary_includes(scope, INT2FIX(PRE)))
                    // already in <nowiki> span or <pre> block
                    rb_str_append(output, literal_h5());
                else
                {
                    if (rb_ary_includes(scope, INT2FIX(EXT_LINK_START)))
                    {
                        // this is a syntax error; an unclosed external link
                        _Wikitext_rollback_failed_external_link(output, scope, line, link_target, link_text, link_class, autolink,
                            line_ending);
                        link_target = Qnil;
                        link_text   = Qnil;
                        capture     = Qnil;
                    }

                    if (!rb_ary_includes(scope, INT2FIX(H5_START)))
                    {
                        // literal output only if not in h5 scope (we stay silent in that case)
                        _Wikitext_start_para_if_necessary(capture, scope, line, output, &pending_crlf);
                        rb_str_append(output, literal_h5());
                    }
                }
                break;

            case H4_END:
                if (rb_ary_includes(scope, INT2FIX(NO_WIKI_START)) || rb_ary_includes(scope, INT2FIX(PRE)))
                    // already in <nowiki> span or <pre> block
                    rb_str_append(output, literal_h4());
                else
                {
                    if (rb_ary_includes(scope, INT2FIX(EXT_LINK_START)))
                    {
                        // this is a syntax error; an unclosed external link
                        _Wikitext_rollback_failed_external_link(output, scope, line, link_target, link_text, link_class, autolink,
                            line_ending);
                        link_target = Qnil;
                        link_text   = Qnil;
                        capture     = Qnil;
                    }

                    if (!rb_ary_includes(scope, INT2FIX(H4_START)))
                    {
                        // literal output only if not in h4 scope (we stay silent in that case)
                        _Wikitext_start_para_if_necessary(capture, scope, line, output, &pending_crlf);
                        rb_str_append(output, literal_h4());
                    }
                }
                break;

            case H3_END:
                if (rb_ary_includes(scope, INT2FIX(NO_WIKI_START)) || rb_ary_includes(scope, INT2FIX(PRE)))
                    // already in <nowiki> span or <pre> block
                    rb_str_append(output, literal_h3());
                else
                {
                    if (rb_ary_includes(scope, INT2FIX(EXT_LINK_START)))
                    {
                        // this is a syntax error; an unclosed external link
                        _Wikitext_rollback_failed_external_link(output, scope, line, link_target, link_text, link_class, autolink,
                            line_ending);
                        link_target = Qnil;
                        link_text   = Qnil;
                        capture     = Qnil;
                    }

                    if (!rb_ary_includes(scope, INT2FIX(H3_START)))
                    {
                        // literal output only if not in h3 scope (we stay silent in that case)
                        _Wikitext_start_para_if_necessary(capture, scope, line, output, &pending_crlf);
                        rb_str_append(output, literal_h3());
                    }
                }
                break;

            case H2_END:
                if (rb_ary_includes(scope, INT2FIX(NO_WIKI_START)) || rb_ary_includes(scope, INT2FIX(PRE)))
                    // already in <nowiki> span or <pre> block
                    rb_str_append(output, literal_h2());
                else
                {
                    if (rb_ary_includes(scope, INT2FIX(EXT_LINK_START)))
                    {
                        // this is a syntax error; an unclosed external link
                        _Wikitext_rollback_failed_external_link(output, scope, line, link_target, link_text, link_class, autolink,
                            line_ending);
                        link_target = Qnil;
                        link_text   = Qnil;
                        capture     = Qnil;
                    }

                    if (!rb_ary_includes(scope, INT2FIX(H2_START)))
                    {
                        // literal output only if not in h2 scope (we stay silent in that case)
                        _Wikitext_start_para_if_necessary(capture, scope, line, output, &pending_crlf);
                        rb_str_append(output, literal_h2());
                    }
                }
                break;

            case H1_END:
                if (rb_ary_includes(scope, INT2FIX(NO_WIKI_START)) || rb_ary_includes(scope, INT2FIX(PRE)))
                    // already in <nowiki> span or <pre> block
                    rb_str_append(output, literal_h1());
                else
                {
                    if (rb_ary_includes(scope, INT2FIX(EXT_LINK_START)))
                    {
                        // this is a syntax error; an unclosed external link
                        _Wikitext_rollback_failed_external_link(output, scope, line, link_target, link_text, link_class, autolink,
                            line_ending);
                        link_target = Qnil;
                        link_text   = Qnil;
                        capture     = Qnil;
                    }

                    if (!rb_ary_includes(scope, INT2FIX(H1_START)))
                    {
                        // literal output only if not in h1 scope (we stay silent in that case)
                        _Wikitext_start_para_if_necessary(capture, scope, line, output, &pending_crlf);
                        rb_str_append(output, literal_h1());
                    }
                }
                break;

            case URI:
                i = TOKEN_TEXT(token); // the URI
                if (rb_ary_includes(scope, INT2FIX(NO_WIKI_START)))
                    // user can temporarily suppress autolinking by using <nowiki></nowiki>
                    // note that unlike MediaWiki, we do allow autolinking inside PRE blocks
                    rb_str_append(output, i);
                else if (rb_ary_includes(scope, INT2FIX(LINK_START)))
                {
                    // not yet implemented
                    // TODO: implement
                }
                else if (rb_ary_includes(scope, INT2FIX(EXT_LINK_START)))
                {
                    if (NIL_P(link_target))
                    {
                        // this must be our link target: look ahead to make sure we see the space we're expecting to see
                        NEXT_TOKEN();
                        if (token->type == SPACE)
                        {
                            rb_ary_push(scope, INT2FIX(SPACE));
                            link_target = i;
                            link_text   = rb_str_new2("");
                            capture     = link_text;
                            token       = NULL; // silently consume space
                        }
                        else
                        {
                            // didn't see the space! this must be an error
                            _Wikitext_pop_from_stack(scope, output, line_ending);
                            _Wikitext_pop_excess_elements(Qnil, scope, line, output, line_ending);
                            _Wikitext_start_para_if_necessary(Qnil, scope, line, output, &pending_crlf);
                            rb_str_append(output, ext_link_start());
                            if (autolink == Qtrue)
                                i = _Wikitext_hyperlink(Qnil, i, i, link_class); // link target, link text, link class
                            rb_str_append(output, i);
                        }
                    }
                    else
                    {
                        if (NIL_P(link_text))
                            // this must be the first part of our link text
                            link_text = i;
                        else
                            // add to existing link text
                            rb_str_append(link_text, i);
                    }
                }
                else
                {
                    // in plain scope, will turn into autolink (with appropriate, user-configurable CSS)
                    _Wikitext_pop_excess_elements(capture, scope, line, output, line_ending);
                    _Wikitext_start_para_if_necessary(capture, scope, line, output, &pending_crlf);
                    if (autolink == Qtrue)
                        i = _Wikitext_hyperlink(Qnil, i, i, link_class); // link target, link text, link class
                    rb_str_append(output, i);
                }
                break;

            // internal links (links to other wiki articles) look like this:
            //      [[another article]] (would point at, for example, "/wiki/another_article")
            //      [[the other article|the link text we'll use for it]]
            //      [[the other article | the link text we'll use for it]]
            // note that the forward slash is a reserved character which changes the meaning of an internal link;
            // this is a link that is external to the wiki but internal to the site as a whole:
            //      [[bug/12]] (a relative link to "/bug/12")
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
            //      SPACE, PRINTABLE, DEFAULT, QUOT and AMP
            // everything else will be rejected
            case LINK_START:
                i = NIL_P(capture) ? output : capture;
                if (rb_ary_includes(scope, INT2FIX(NO_WIKI_START)) || rb_ary_includes(scope, INT2FIX(PRE)))
                    // already in <nowiki> span or <pre> block
                    rb_str_append(i, link_start());
                else if (rb_ary_includes(scope, INT2FIX(EXT_LINK_START)))
                    // already in external link scope! (and in fact, must be capturing link_text right now)
                    rb_str_append(i, link_start());
                else if (rb_ary_includes(scope, INT2FIX(LINK_START)))
                {
                    // already in internal link scope! this is a syntax error
                    _Wikitext_rollback_failed_link(output, scope, line, link_target, link_text, link_class, line_ending);
                    link_target = Qnil;
                    link_text   = Qnil;
                    capture     = Qnil;
                    rb_str_append(output, link_start());
                }
                else if (rb_ary_includes(scope, INT2FIX(SEPARATOR)))
                {
                    // scanning internal link text
                }
                else // not in internal link scope yet
                {
                    rb_ary_push(scope, INT2FIX(LINK_START));

                    // look ahead and try to gobble up link target
                    while (NEXT_TOKEN(), (type = token->type))
                    {
                        if (type == SPACE       ||
                            type == PRINTABLE   ||
                            type == DEFAULT     ||
                            type == QUOT        ||
                            type == QUOT_ENTITY ||
                            type == AMP         ||
                            type == AMP_ENTITY)
                        {
                            // accumulate these tokens into link_target
                            if (NIL_P(link_target))
                            {
                                link_target = rb_str_new2("");
                                capture     = link_target;
                            }
                            if (type == QUOT_ENTITY)
                                // don't insert the entity, insert the literal quote
                                rb_str_append(link_target, quote());
                            else if (type == AMP_ENTITY)
                                // don't insert the entity, insert the literal ampersand
                                rb_str_append(link_target, ampersand());
                            else
                                rb_str_append(link_target, TOKEN_TEXT(token));
                        }
                        else if (type == LINK_END)
                            break; // jump back to top of loop (will handle this in LINK_END case below)
                        else if (type == SEPARATOR)
                        {
                            rb_ary_push(scope, INT2FIX(SEPARATOR));
                            link_text   = rb_str_new2("");
                            capture     = link_text;
                            token       = NULL;
                            break;
                        }
                        else // unexpected token (syntax error)
                        {
                            _Wikitext_rollback_failed_link(output, scope, line, link_target, link_text, link_class, line_ending);
                            link_target = Qnil;
                            link_text   = Qnil;
                            capture     = Qnil;
                            break; // jump back to top of loop to handle unexpected token
                        }
                    }

                    // jump to top of the loop to process token we scanned during lookahead (if any)
                    continue;
                }
                break;

            case LINK_END:
                i = NIL_P(capture) ? output : capture;
                if (rb_ary_includes(scope, INT2FIX(NO_WIKI_START)) || rb_ary_includes(scope, INT2FIX(PRE)))
                    // already in <nowiki> span or <pre> block
                    rb_str_append(i, link_end());
                else if (rb_ary_includes(scope, INT2FIX(EXT_LINK_START)))
                    // already in external link scope! (and in fact, must be capturing link_text right now)
                    rb_str_append(i, link_end());
                else if (rb_ary_includes(scope, INT2FIX(LINK_START)))
                {
                    // in internal link scope!
                    if (NIL_P(link_text) || RSTRING_LEN(link_text) == 0)
                        // use link target as link text
                        link_text = Wikitext_sanitize_link_target(self, link_target);
                    link_target = Wikitext_encode_link_target(self, link_target);
                    _Wikitext_pop_from_stack_up_to(scope, i, INT2FIX(LINK_START), Qtrue, line_ending);
                    _Wikitext_pop_excess_elements(Qnil, scope, line, output, line_ending);
                    _Wikitext_start_para_if_necessary(Qnil, scope, line, output, &pending_crlf);
                    i = _Wikitext_hyperlink(prefix, link_target, link_text, Qnil); // link target, link text, link class
                    rb_str_append(output, i);
                    link_target = Qnil;
                    link_text   = Qnil;
                    capture     = Qnil;
                }
                else // wasn't in internal link scope
                {
                    _Wikitext_pop_excess_elements(capture, scope, line, output, line_ending);
                    _Wikitext_start_para_if_necessary(capture, scope, line, output, &pending_crlf);
                    rb_str_append(i, link_end());
                }
                break;

            // external links look like this:
            //      [http://google.com/ the link text]
            // strings in square brackets which don't match this syntax get passed through literally; eg:
            //      he was very angery [sic] about the turn of events
            case EXT_LINK_START:
                i = NIL_P(capture) ? output : capture;
                if (rb_ary_includes(scope, INT2FIX(NO_WIKI_START)) || rb_ary_includes(scope, INT2FIX(PRE)))
                    // already in <nowiki> span or <pre> block
                    rb_str_append(i, ext_link_start());
                else if (rb_ary_includes(scope, INT2FIX(EXT_LINK_START)))
                    // already in external link scope! (and in fact, must be capturing link_text right now)
                    rb_str_append(i, ext_link_start());
                else if (rb_ary_includes(scope, INT2FIX(LINK_START)))
                {
                    // already in internal link scope!
                    i = ext_link_start();
                    if (NIL_P(link_target))
                        // this must be the first character of our link target
                        link_target = i;
                    else if (rb_ary_includes(scope, INT2FIX(SPACE)))
                    {
                        // link target has already been scanned
                        if (NIL_P(link_text))
                            // this must be the first character of our link text
                            link_text = i;
                        else
                            // add to existing link text
                            rb_str_append(link_text, i);
                    }
                    else
                        // add to existing link target
                        rb_str_append(link_target, i);
                }
                else // not in external link scope yet
                {
                    // look ahead: expect a URI
                    NEXT_TOKEN();
                    if (token->type == URI)
                        rb_ary_push(scope, INT2FIX(EXT_LINK_START));    // so far so good, jump back to the top of the loop
                    else
                    {
                        // only get here if there was a syntax error (missing URI)
                        _Wikitext_pop_excess_elements(capture, scope, line, output, line_ending);
                        _Wikitext_start_para_if_necessary(capture, scope, line, output, &pending_crlf);
                        rb_str_append(output, ext_link_start());
                    }
                    continue; // jump back to top of loop to handle token (either URI or whatever it is)
                }
                break;

            case EXT_LINK_END:
                i = NIL_P(capture) ? output : capture;
                if (rb_ary_includes(scope, INT2FIX(NO_WIKI_START)) || rb_ary_includes(scope, INT2FIX(PRE)))
                    // already in <nowiki> span or <pre> block
                    rb_str_append(i, ext_link_end());
                else if (rb_ary_includes(scope, INT2FIX(EXT_LINK_START)))
                {
                    if (NIL_P(link_text))
                        // this is a syntax error; external link with no link text
                        _Wikitext_rollback_failed_external_link(output, scope, line, link_target, link_text, link_class, autolink,
                            line_ending);
                    else
                    {
                        // success!
                        _Wikitext_pop_from_stack_up_to(scope, i, INT2FIX(EXT_LINK_START), Qtrue, line_ending);
                        _Wikitext_pop_excess_elements(Qnil, scope, line, output, line_ending);
                        _Wikitext_start_para_if_necessary(Qnil, scope, line, output, &pending_crlf);
                        i = _Wikitext_hyperlink(Qnil, link_target, link_text, link_class); // link target, link text, link class
                        rb_str_append(output, i);
                    }
                    link_target = Qnil;
                    link_text   = Qnil;
                    capture     = Qnil;
                }
                else
                {
                    _Wikitext_pop_excess_elements(Qnil, scope, line, output, line_ending);
                    _Wikitext_start_para_if_necessary(Qnil, scope, line, output, &pending_crlf);
                    rb_str_append(output, ext_link_end());
                }
                break;

            case SEPARATOR:
                i = NIL_P(capture) ? output : capture;
                _Wikitext_pop_excess_elements(capture, scope, line, i, line_ending);
                _Wikitext_start_para_if_necessary(capture, scope, line, i, &pending_crlf);
                rb_str_append(i, separator());
                break;

            case SPACE:
                i = NIL_P(capture) ? output : capture;
                j = TOKEN_TEXT(token); // SPACE token may actually be a run of spaces
                if (rb_ary_includes(scope, INT2FIX(NO_WIKI_START)) || rb_ary_includes(scope, INT2FIX(PRE)))
                    // already in <nowiki> span or <pre> block
                    rb_str_append(i, j);
                else
                {
                    // peek ahead to see next token
                    NEXT_TOKEN();
                    type = token->type;
                    if (((type == H6_END) && rb_ary_includes(scope, INT2FIX(H6_START))) ||
                        ((type == H5_END) && rb_ary_includes(scope, INT2FIX(H5_START))) ||
                        ((type == H4_END) && rb_ary_includes(scope, INT2FIX(H4_START))) ||
                        ((type == H3_END) && rb_ary_includes(scope, INT2FIX(H3_START))) ||
                        ((type == H2_END) && rb_ary_includes(scope, INT2FIX(H2_START))) ||
                        ((type == H1_END) && rb_ary_includes(scope, INT2FIX(H1_START))))
                    {
                        // will suppress emission of space (discard) if next token is a H6_END, H5_END etc and we are in the corresponding scope
                    }
                    else
                    {
                        // emit the space
                        _Wikitext_pop_excess_elements(capture, scope, line, i, line_ending);
                        _Wikitext_start_para_if_necessary(capture, scope, line, i, &pending_crlf);
                        rb_str_append(i, j);
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
                i = NIL_P(capture) ? output : capture;
                _Wikitext_pop_excess_elements(capture, scope, line, i, line_ending);
                _Wikitext_start_para_if_necessary(capture, scope, line, i, &pending_crlf);
                rb_str_append(i, TOKEN_TEXT(token));
                break;

            case HEX_ENTITY:
                // normalize hex entities (downcase them)
                i = NIL_P(capture) ? output : capture;
                _Wikitext_pop_excess_elements(capture, scope, line, i, line_ending);
                _Wikitext_start_para_if_necessary(capture, scope, line, i, &pending_crlf);
                rb_str_append(i, _Wikitext_downcase(TOKEN_TEXT(token)));
                break;

            case QUOT:
                i = NIL_P(capture) ? output : capture;
                _Wikitext_pop_excess_elements(capture, scope, line, i, line_ending);
                _Wikitext_start_para_if_necessary(capture, scope, line, i, &pending_crlf);
                rb_str_append(i, quot_entity());
                break;

            case AMP:
                i = NIL_P(capture) ? output : capture;
                _Wikitext_pop_excess_elements(capture, scope, line, i, line_ending);
                _Wikitext_start_para_if_necessary(capture, scope, line, i, &pending_crlf);
                rb_str_append(i, amp_entity());
                break;

            case LESS:
                i = NIL_P(capture) ? output : capture;
                _Wikitext_pop_excess_elements(capture, scope, line, i, line_ending);
                _Wikitext_start_para_if_necessary(capture, scope, line, i, &pending_crlf);
                rb_str_append(i, lt_entity());
                break;

            case GREATER:
                i = NIL_P(capture) ? output : capture;
                _Wikitext_pop_excess_elements(capture, scope, line, i, line_ending);
                _Wikitext_start_para_if_necessary(capture, scope, line, i, &pending_crlf);
                rb_str_append(i, gt_entity());
                break;

            case CRLF:
                if (rb_ary_includes(scope, INT2FIX(LINK_START)))
                {
                    // this is a syntax error; an unclosed external link
                    _Wikitext_rollback_failed_link(output, scope, line, link_target, link_text, link_class, line_ending);
                    link_target = Qnil;
                    link_text   = Qnil;
                    capture     = Qnil;
                }
                else if (rb_ary_includes(scope, INT2FIX(EXT_LINK_START)))
                {
                    // this is a syntax error; an unclosed external link
                    _Wikitext_rollback_failed_external_link(output, scope, line, link_target, link_text, link_class, autolink,
                        line_ending);
                    link_target = Qnil;
                    link_text   = Qnil;
                    capture     = Qnil;
                }

                if (rb_ary_includes(scope, INT2FIX(NO_WIKI_START)))
                {
                    // <nowiki> spans are unique; CRLFs are blindly echoed
                    while (!NIL_P(rb_ary_delete_at(line_buffer, -1)));
                    rb_str_append(output, line_ending);
                    pending_crlf = Qfalse;
                    break;
                }
                else if (rb_ary_includes(scope, INT2FIX(PRE)))
                {
                    // beware when nothing or BLOCKQUOTE on line buffer (not line stack!) prior to CRLF, that must be end of PRE block
                    if (NIL_P(rb_ary_entry(line_buffer, -2)) || (FIX2INT(rb_ary_entry(line_buffer, -2)) == BLOCKQUOTE))
                    {
                        // don't emit in this case
                    }
                    else
                        rb_str_append(output, line_ending);
                    pending_crlf = Qfalse;
                }
                else
                {
                    pending_crlf = Qtrue;

                    // count number of BLOCKQUOTE tokens in line buffer (can be zero) and pop back to that level
                    // as a side effect, this handles any open span-level elements and unclosed blocks (with special handling for P blocks and LI elements)
                    i = _Wikitext_count(INT2FIX(BLOCKQUOTE), line);
                    for (j = RARRAY_LEN(scope); j > i; j--)
                    {
                        if (FIX2INT(rb_ary_entry(line, -1)) == LI)
                        {
                            pending_crlf = Qfalse;
                            break;
                        }

                        // special handling on last iteration through the loop if the top item on the scope is a P block
                        if ((j - i == 1) && (FIX2INT(rb_ary_entry(scope, -1)) == P))
                        {
                            // if nothing or BLOCKQUOTE on line buffer (not line stack!) prior to CRLF, this must be a paragraph break
                            if (NIL_P(rb_ary_entry(line_buffer, -2)) || (FIX2INT(rb_ary_entry(line_buffer, -2)) == BLOCKQUOTE))
                                // paragraph break
                                pending_crlf = Qfalse;
                            else
                                // not a paragraph break!
                                continue;
                        }
                        _Wikitext_pop_from_stack(scope, output, line_ending);
                    }
                }

                // delete the entire contents of the line scope stack and buffer
                while (!NIL_P(rb_ary_delete_at(line, -1)));
                while (!NIL_P(rb_ary_delete_at(line_buffer, -1)));
                break;

            case PRINTABLE:
                i = NIL_P(capture) ? output : capture;
                _Wikitext_pop_excess_elements(capture, scope, line, i, line_ending);
                _Wikitext_start_para_if_necessary(capture, scope, line, i, &pending_crlf);
                rb_str_append(i, TOKEN_TEXT(token));
                break;

            case DEFAULT:
                i = NIL_P(capture) ? output : capture;
                _Wikitext_pop_excess_elements(capture, scope, line, i, line_ending);
                _Wikitext_start_para_if_necessary(capture, scope, line, i, &pending_crlf);
                rb_str_append(i, _Wikitext_utf32_char_to_entity(token->code_point));    // convert to entity
                break;

            case END_OF_FILE:
                // close any open scopes on hitting EOF
                if (rb_ary_includes(scope, INT2FIX(EXT_LINK_START)))
                    // this is a syntax error; an unclosed external link
                    _Wikitext_rollback_failed_external_link(output, scope, line, link_target, link_text, link_class, autolink,
                        line_ending);
                else if (rb_ary_includes(scope, INT2FIX(LINK_START)))
                    // this is a syntax error; an unclosed internal link
                    _Wikitext_rollback_failed_link(output, scope, line, link_target, link_text, link_class, line_ending);
                for (i = 0, j = RARRAY_LEN(scope); i < j; i++)
                    _Wikitext_pop_from_stack(scope, output, line_ending);
                goto return_output; // break not enough here (want to break out of outer while loop, not inner switch statement)

            default:
                break;
        }

        // reset current token; forcing lexer to return another token at the top of the loop
        token = NULL;
    } while (1);
return_output:
    return output;
}
