// Copyright 2007 Wincent Colaiuta

// header for this file
#include "wiki_text.h"

static VALUE mWikiText  = 0;     // WikiText
static VALUE cParser    = 0;     // WikiText::Parser
static VALUE rb_Iconv   = 0;

// exception types
enum {
    no_exception,
    input_stream_memory,
    input_stream_other,
    lexer_memory,
    lexer_other,
    token_stream_memory,
    token_stream_other,
};

// "string literals" (pre-prepared arrays in UCS-2 encoding)
static ANTLR3_UINT16 space_literal[]                = { ' ' };
static ANTLR3_UINT16 pre_start_literal[]            = { '<', 'p', 'r', 'e', '>' };
static ANTLR3_UINT16 pre_end_literal[]              = { '<', '/', 'p', 'r', 'e', '>' };
static ANTLR3_UINT16 blockquote_start_literal[]     = { '<', 'b', 'l', 'o', 'c', 'k', 'q', 'u', 'o', 't', 'e', '>' };
static ANTLR3_UINT16 blockquote_end_literal[]       = { '<', '/', 'b', 'l', 'o', 'c', 'k', 'q', 'u', 'o', 't', 'e', '>' };
static ANTLR3_UINT16 escaped_nowiki_start_literal[] = { '&', 'l', 't', ';', 'n', 'o', 'w', 'i', 'k', 'i', '&', 'g', 't', ';' };
static ANTLR3_UINT16 escaped_nowiki_end_literal[]   = { '&', 'l', 't', ';', '/', 'n', 'o', 'w', 'i', 'k', 'i', '&', 'g', 't', ';' };
static ANTLR3_UINT16 strong_start_literal[]         = { '<', 's', 't', 'r', 'o', 'n', 'g', '>' };
static ANTLR3_UINT16 strong_end_literal[]           = { '<', '/', 's', 't', 'r', 'o', 'n', 'g', '>' };
static ANTLR3_UINT16 escaped_strong_literal[]       = { '\'', '\'', '\'' };
static ANTLR3_UINT16 em_start_literal[]             = { '<', 'e', 'm', '>' };
static ANTLR3_UINT16 em_end_literal[]               = { '<', '/', 'e', 'm', '>' };
static ANTLR3_UINT16 escaped_em_literal[]           = { '\'', '\'' };
static ANTLR3_UINT16 strong_em_literal[]            = { '<', 's', 't', 'r', 'o', 'n', 'g', '>', '<', 'e', 'm', '>' };
static ANTLR3_UINT16 escaped_strong_em_literal[]    = { '\'', '\'', '\'', '\'', '\'' };
static ANTLR3_UINT16 tt_start_literal[]             = { '<', 't', 't', '>' };
static ANTLR3_UINT16 tt_end_literal[]               = { '<', '/', 't', 't', '>' };
static ANTLR3_UINT16 escaped_tt_start_literal[]     = { '&', 'l', 't', ';', 't', 't', '&', 'g', 't', ';' };
static ANTLR3_UINT16 escaped_tt_end_literal[]       = { '&', 'l', 't', ';', '/', 't', 't', '&', 'g', 't', ';' };
static ANTLR3_UINT16 ol_start_literal[]             = { '<', 'o', 'l', '>' };
static ANTLR3_UINT16 ol_end_literal[]               = { '<', '/', 'o', 'l', '>' };
static ANTLR3_UINT16 ul_start_literal[]             = { '<', 'u', 'l', '>' };
static ANTLR3_UINT16 ul_end_literal[]               = { '<', '/', 'u', 'l', '>' };
static ANTLR3_UINT16 li_start_literal[]             = { '<', 'l', 'i', '>' };
static ANTLR3_UINT16 li_end_literal[]               = { '<', '/', 'l', 'i', '>' };
static ANTLR3_UINT16 h6_start_literal[]             = { '<', 'h', '6', '>' };
static ANTLR3_UINT16 h6_end_literal[]               = { '<', '/', 'h', '6', '>' };
static ANTLR3_UINT16 escaped_h6_start_literal[]     = { '=', '=', '=', '=', '=', '=' };
static ANTLR3_UINT16 h5_start_literal[]             = { '<', 'h', '5', '>' };
static ANTLR3_UINT16 h5_end_literal[]               = { '<', '/', 'h', '5', '>' };
static ANTLR3_UINT16 escaped_h5_start_literal[]     = { '=', '=', '=', '=', '=' };
static ANTLR3_UINT16 h4_start_literal[]             = { '<', 'h', '4', '>' };
static ANTLR3_UINT16 h4_end_literal[]               = { '<', '/', 'h', '4', '>' };
static ANTLR3_UINT16 escaped_h4_start_literal[]     = { '=', '=', '=', '=' };
static ANTLR3_UINT16 h3_start_literal[]             = { '<', 'h', '3', '>' };
static ANTLR3_UINT16 h3_end_literal[]               = { '<', '/', 'h', '3', '>' };
static ANTLR3_UINT16 escaped_h3_start_literal[]     = { '=', '=', '=' };
static ANTLR3_UINT16 h2_start_literal[]             = { '<', 'h', '2', '>' };
static ANTLR3_UINT16 h2_end_literal[]               = { '<', '/', 'h', '2', '>' };
static ANTLR3_UINT16 escaped_h2_start_literal[]     = { '=', '=' };
static ANTLR3_UINT16 h1_start_literal[]             = { '<', 'h', '1', '>' };
static ANTLR3_UINT16 h1_end_literal[]               = { '<', '/', 'h', '1', '>' };
static ANTLR3_UINT16 escaped_h1_start_literal[]     = { '=' };
static ANTLR3_UINT16 p_start_literal[]              = { '<', 'p', '>' };
static ANTLR3_UINT16 p_end_literal[]                = { '<', '/', 'p', '>' };
static ANTLR3_UINT16 quot_entity_literal[]          = { '&', 'q', 'u', 'o', 't', ';' };
static ANTLR3_UINT16 amp_entity_literal[]           = { '&', 'a', 'm', 'p',';' };
static ANTLR3_UINT16 lt_entity_literal[]            = { '&', 'l', 't', ';' };
static ANTLR3_UINT16 gt_entity_literal[]            = { '&', 'g', 't', ';' };

// Returns the text encoding that should be used as a conversion target for iconv when preparing the input stream for ANTLR.
// When running on little-endian systems returns UCS-2LE.
// Otherwise returns UCS-2BE.
const char ANTLR3_INLINE *_wiki_text_parser_target_encoding(void)
{
#if defined(__LITTLE_ENDIAN__)
    return "UCS-2LE";
#elif define(__BIG_ENDIAN__)
    return "UCS-2BE";
#else
    // try to figure out endianness dynamically
    int i = 0;
    ((char *)(&i))[0] = 1;
    return (i == 1) ? "UCS-2LE" : "UCS-2LE";
#endif
}

VALUE _wiki_text_setup_iconv()
{
    // no attempt at thread safety here (TODO: add it?)
    if (rb_Iconv == 0)
    {
        rb_require("iconv");
        rb_Iconv = rb_path2class("Iconv");
        if (NIL_P(rb_Iconv))
            rb_raise(rb_eLoadError, "failed to load Iconv class");
        else
            return Qtrue;
    }
    return Qfalse;
}

VALUE _wiki_text_ucs2_to_utf8(VALUE in)
{
    VALUE ucs2 = StringValue(in); // duck typing: raises TypeError if passed object is not String-like

    // converted_array = Iconv.iconv("UTF-8", "UCS-2LE", ucs2)
    // converted_array = Iconv.iconv("UTF-8", "UCS-2BE", ucs2)
    VALUE converted_array = rb_funcall(rb_Iconv, rb_intern("iconv"), 3, rb_str_new2("UTF-8"), rb_str_new2(_wiki_text_parser_target_encoding()), ucs2);

    // utf8 = converted_array.shift
    VALUE utf8 = rb_funcall(converted_array, rb_intern("shift"), 0);
    if (NIL_P(utf8))
      rb_raise(rb_eRuntimeError, "failed to convert input string to UTF-8 encoding");
    return utf8;
}

VALUE _wiki_text_utf8_to_ucs2(VALUE in)
{
    VALUE utf8 = StringValue(in); // duck typing: raises TypeError if passed object is not String-like

    // converted_array = Iconv.iconv("UCS-2LE", "UTF-8", utf8)
    // converted_array = Iconv.iconv("UCS-2BE", "UTF-8", utf8)
    VALUE converted_array = rb_funcall(rb_Iconv, rb_intern("iconv"), 3, rb_str_new2(_wiki_text_parser_target_encoding()), rb_str_new2("UTF-8"), utf8);

    // ucs2 = converted_array.shift
    VALUE ucs2 = rb_funcall(converted_array, rb_intern("shift"), 0);
    if (NIL_P(ucs2))
      rb_raise(rb_eRuntimeError, "failed to convert input string to UCS-2 encoding");
    return ucs2;
}

// necessary for overriding the generated nextToken function
pANTLR3_COMMON_TOKEN (*original_next_token)(struct ANTLR3_TOKEN_SOURCE_struct * tokenSource);

// Simple override of generated nextToken function that remembers the last-matched token, providing a small amount of context sensitivity.
static pANTLR3_COMMON_TOKEN _wiki_text_next_token(pANTLR3_TOKEN_SOURCE toksource)
{
    // grab token using the generated function
    pANTLR3_COMMON_TOKEN token = original_next_token(toksource);

    // store it in last-matched field
    pANTLR3_LEXER lexer = (pANTLR3_LEXER)(toksource->super);
    lexer->rec->userp = (void *)token->type;

    // return token as per usual
    return token;
}

// Returns a count indicating the number of times the token appears in the collection.
long _wiki_text_count(VALUE token, VALUE collection)
{
    long count = 0;
    for (long i = 0, max = RARRAY(collection)->len; i < max; i++)
    {
        if (FIX2INT(rb_ary_entry(collection, i)) == FIX2INT(token))
            count++;
    }
    return count;
}

// Pops a single item off the stack.
// A corresponding closing tag is written to the target string.
void _wiki_text_pop_from_stack(VALUE stack, VALUE target, VALUE line_ending)
{
    VALUE top = rb_ary_entry(stack, -1);
    if (NIL_P(top))
        return;
    switch (FIX2INT(top))
    {
        case PRE:
            rb_str_append(target, rb_str_new((const char *)pre_end_literal, sizeof(pre_end_literal)));
            rb_str_append(target, line_ending);
            break;

        case BLOCKQUOTE:
            rb_str_append(target, rb_str_new((const char *)blockquote_end_literal, sizeof(blockquote_end_literal)));
            rb_str_append(target, line_ending);
            break;

        case NO_WIKI_START:
            // not a real HTML tag; so nothing to pop
            break;

        case STRONG:
            rb_str_append(target, rb_str_new((const char *)strong_end_literal, sizeof(strong_end_literal)));
            break;

        case EM:
            rb_str_append(target, rb_str_new((const char *)em_end_literal, sizeof(em_end_literal) ));
            break;

        case TT_START:
            rb_str_append(target, rb_str_new((const char *)tt_end_literal, sizeof(tt_end_literal)));
            break;

        case OL:
            rb_str_append(target, rb_str_new((const char *)ol_end_literal, sizeof(ol_end_literal)));
            rb_str_append(target, line_ending);
            break;

        case UL:
            rb_str_append(target, rb_str_new((const char *)ul_end_literal, sizeof(ul_end_literal)));
            rb_str_append(target, line_ending);
            break;

        case LI:
            rb_str_append(target, rb_str_new((const char *)li_end_literal, sizeof(li_end_literal)));
            rb_str_append(target, line_ending);
            break;

        case H6_START:
            rb_str_append(target, rb_str_new((const char *)h6_end_literal, sizeof(h6_end_literal)));
            rb_str_append(target, line_ending);
            break;

        case H5_START:
            rb_str_append(target, rb_str_new((const char *)h5_end_literal, sizeof(h5_end_literal)));
            rb_str_append(target, line_ending);
            break;

        case H4_START:
            rb_str_append(target, rb_str_new((const char *)h4_end_literal, sizeof(h4_end_literal)));
            rb_str_append(target, line_ending);
            break;

        case H3_START:
            rb_str_append(target, rb_str_new((const char *)h3_end_literal, sizeof(h3_end_literal)));
            rb_str_append(target, line_ending);
            break;

        case H2_START:
            rb_str_append(target, rb_str_new((const char *)h2_end_literal, sizeof(h2_end_literal)));
            rb_str_append(target, line_ending);
            break;

        case H1_START:
            rb_str_append(target, rb_str_new((const char *)h1_end_literal, sizeof(h1_end_literal)));
            rb_str_append(target, line_ending);
            break;

        case P:
            rb_str_append(target, rb_str_new((const char *)p_end_literal, sizeof(p_end_literal)));
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
void _wiki_text_pop_from_stack_up_to(VALUE stack, VALUE target, VALUE item, VALUE including, VALUE line_ending)
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
        _wiki_text_pop_from_stack(stack, target, line_ending);
    } while (continue_looping);
}

void ANTLR3_INLINE _wiki_text_start_para_if_necessary(VALUE scope, VALUE line, VALUE output, VALUE *pending_crlf)
{
    // if no block open yet, or top of stack is BLOCKQUOTE (w¡th nothing in it yet)
    if ((RARRAY(scope)->len == 0) || (FIX2INT(rb_ary_entry(scope, -1)) == BLOCKQUOTE))
    {
        rb_str_append(output, rb_str_new((const char *)p_start_literal, sizeof(p_start_literal)));
        rb_ary_push(scope, INT2FIX(P));
        rb_ary_push(line, INT2FIX(P));
    }
    else if (rb_ary_includes(scope, INT2FIX(P)) && *pending_crlf == Qtrue)
        // already in a paragraph block; convert pending CRLF into a space
        rb_str_append(output, rb_str_new((const char *)space_literal, sizeof(space_literal)));
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
void ANTLR3_INLINE _wiki_text_pop_excess_elements(VALUE scope, VALUE line, VALUE output, VALUE line_ending)
{
    for (long i = RARRAY(scope)->len, j = RARRAY(line)->len; i > j; i--)
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
                _wiki_text_pop_from_stack(scope, output, line_ending);
            }
        }
        _wiki_text_pop_from_stack(scope, output, line_ending);
    }
}

VALUE wiki_text_parser_initialize(VALUE self)
{
    rb_iv_set(self, "@line_ending", rb_str_new2("\n"));
    return self;
}

// Input string must be UTF-8 encoded.
// Note that ANTLR expects a UCS-2 encoded string, so the input is converted automatically.
// If the input string contains characters not encodable in UCS-2 an exception will be raised.
// The output string is converted back to UTF-8 after parsing is complete.
VALUE wiki_text_parser_parse(int argc, VALUE *argv, VALUE self)
{
    // preliminaries
    int     exception       = no_exception;
    int     exception_info  = 0;
    VALUE   output          = rb_str_new2("");  // although not explicitly UCS-2 encoded, a zero-length C string will work fine

    // process arguments
    VALUE string, options;
    if (rb_scan_args(argc, argv, "11", &string, &options) == 1) // 1 mandatory argument, 1 optional argument
        options = rb_hash_new();                                // default to an empty hash if no argument passed

    // convert string from UTF-8 to UCS-2LE or UCS-2BE
    _wiki_text_setup_iconv();
    VALUE ucs2input = _wiki_text_utf8_to_ucs2(string);

    // set up lexer
    pANTLR3_UINT16          pointer = (pANTLR3_UINT16)StringValuePtr(ucs2input);
    ANTLR3_UINT64           count   = (ANTLR3_UINT64)RSTRING(ucs2input)->len / 2;

    pANTLR3_INPUT_STREAM    stream  = antlr3NewUCS2StringInPlaceStream(pointer, count, NULL);
    if ((ANTLR3_UINT64)stream == (ANTLR3_UINT64)ANTLR3_ERR_NOMEM)
    {
        exception = input_stream_memory;
        goto finalize;
    }
    else if ((ANTLR3_UINT64)stream < 0)
    {
        exception = input_stream_other;
        goto finalize;
    }

    pWikiTextLexer lexer = WikiTextLexerNew(stream);
    if ((ANTLR3_UINT64)lexer == (ANTLR3_UINT64)ANTLR3_ERR_NOMEM)
    {
        exception = lexer_memory;
        goto clean_up_input_stream;
    }
    else if ((ANTLR3_UINT64)lexer < 0)
    {
        exception = lexer_other;
        goto clean_up_input_stream;
    }

    // install custom nextToken() override
    original_next_token = lexer->pLexer->tokSource->nextToken;
    lexer->pLexer->tokSource->nextToken = _wiki_text_next_token;
    lexer->pLexer->rec->userp = NULL;

    pANTLR3_COMMON_TOKEN_STREAM tstream = antlr3CommonTokenStreamSourceNew(ANTLR3_SIZE_HINT, lexer->pLexer->tokSource);
    if ((ANTLR3_UINT64)tstream == ANTLR3_ERR_NOMEM)
    {
        exception = token_stream_memory;
        goto clean_up_lexer;
    }
    else if ((ANTLR3_UINT64)tstream < 0)
    {
        exception = token_stream_other;
        goto clean_up_lexer;
    }

    VALUE scope         = rb_ary_new(); // stack for tracking scope
    VALUE line          = rb_ary_new(); // stack for tracking scope as implied by current line
    VALUE line_buffer   = rb_ary_new(); // stack for tracking raw tokens (not scope) on current line
    VALUE pending_crlf  = Qfalse;

    // access this once per parse
    VALUE line_ending = _wiki_text_utf8_to_ucs2(rb_iv_get(self, "@line_ending"));

    pANTLR3_COMMON_TOKEN token = NULL;
    do
    {
        // check to see if we have a token hanging around from a previous iteration of this loop
        if (token == NULL)
            // no token hanging around, must get a new one
            token = lexer->pLexer->tokSource->nextToken(lexer->pLexer->tokSource);
        if (token == NULL)
            // will keep going until hit EOF
            continue;
        ANTLR3_UINT32 type = token->getType(token);

        // many restrictions depend on what is at the top of the stack
        VALUE top = rb_ary_entry(scope, -1);

        if (type != ANTLR3_TOKEN_EOF)
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

        // for converting UCS-2 character to a hex string when generating a numeric entity
        ANTLR3_UINT16 hex_string[8] = { '&', '#', 'x', 0, 0, 0, 0, ';' };

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
        // or converting illegal (unexpected) tokens to their plain text equivalents (providing visual feedback to wikitext author).
        switch (type)
        {
            case PRE:
                if (rb_ary_includes(scope, INT2FIX(NO_WIKI_START)))
                {
                    // already in <nowiki> span (no need to check for <pre>; can never appear inside it)
                    rb_str_append(output, rb_str_new((const char *)space_literal, sizeof(space_literal)));
                    break;
                }

                // count number of BLOCKQUOTE tokens in line buffer and in scope stack
                rb_ary_push(line, INT2FIX(PRE));
                i = _wiki_text_count(INT2FIX(BLOCKQUOTE), line);
                j = _wiki_text_count(INT2FIX(BLOCKQUOTE), scope);

                if (i < j)
                {
                    // must pop (reduce nesting level)
                    for (i = j - i; i > 0; i--)
                        _wiki_text_pop_from_stack_up_to(scope, output, INT2FIX(BLOCKQUOTE), Qtrue, line_ending);
                }

                if (!rb_ary_includes(scope, INT2FIX(PRE)))
                {
                    _wiki_text_pop_excess_elements(scope, line, output, line_ending);
                    rb_str_append(output, rb_str_new((const char *)pre_start_literal, sizeof(pre_start_literal)));
                    rb_ary_push(scope, INT2FIX(PRE));
                }
                break;

            case BLOCKQUOTE:
                if (rb_ary_includes(scope, INT2FIX(NO_WIKI_START)))
                    // already in <nowiki> span (no need to check for <pre>; can never appear inside it)
                    rb_str_append(output, rb_str_new((const char *)token->start, (token->stop + 1 - token->start)));
                else
                {
                    rb_ary_push(line, INT2FIX(BLOCKQUOTE));

                    // count number of BLOCKQUOTE tokens in line buffer and in scope stack
                    i = _wiki_text_count(INT2FIX(BLOCKQUOTE), line);
                    j = _wiki_text_count(INT2FIX(BLOCKQUOTE), scope);

                    // given that BLOCKQUOTE tokens can be nested, peek ahead and see if there are any more which might affect the decision to push or pop
                    token = NULL;
                    while ((token = lexer->pLexer->tokSource->nextToken(lexer->pLexer->tokSource)) && (token->getType(token) == BLOCKQUOTE))
                    {
                        rb_ary_push(line, INT2FIX(BLOCKQUOTE));
                        i++;
                        token = NULL;
                    }

                    // now decide whether to push, pop or do nothing
                    if (i > j)
                    {
                        // must push (increase nesting level)
                        _wiki_text_pop_from_stack_up_to(scope, output, INT2FIX(BLOCKQUOTE), Qfalse, line_ending);
                        for (i = i - j; i > 0; i--)
                        {
                            rb_str_append(output, rb_str_new((const char *)blockquote_start_literal, sizeof(blockquote_start_literal)));
                            rb_ary_push(scope, INT2FIX(BLOCKQUOTE));
                        }
                    }
                    else if (i < j)
                    {
                        // must pop (reduce nesting level)
                        for (i = j - i; i > 0; i--)
                            _wiki_text_pop_from_stack_up_to(scope, output, INT2FIX(BLOCKQUOTE), Qtrue, line_ending);
                    }

                    if (token != NULL)
                        // we got a non-BLOCKQUOTE token from the lexer, which still needs to be processed, so jump to top of loop
                        continue;
                }

                break;

            case NO_WIKI_START:
                if (rb_ary_includes(scope, INT2FIX(NO_WIKI_START)) || rb_ary_includes(scope, INT2FIX(PRE)))
                    // already in <nowiki> span or <pre> block
                    rb_str_append(output, rb_str_new((const char *)escaped_nowiki_start_literal, sizeof(escaped_nowiki_start_literal)));
                else
                {
                    _wiki_text_pop_excess_elements(scope, line, output, line_ending);
                    _wiki_text_start_para_if_necessary(scope, line, output, &pending_crlf);
                    rb_ary_push(scope, INT2FIX(NO_WIKI_START));
                    rb_ary_push(line, INT2FIX(NO_WIKI_START));
                }
                break;

            case NO_WIKI_END:
                if (rb_ary_includes(scope, INT2FIX(NO_WIKI_START)))
                    // <nowiki> should always only ever be the last item in the stack, but use the helper routine just in case
                    _wiki_text_pop_from_stack_up_to(scope, output, INT2FIX(NO_WIKI_START), Qtrue, line_ending);
                else
                {
                    _wiki_text_pop_excess_elements(scope, line, output, line_ending);
                    _wiki_text_start_para_if_necessary(scope, line, output, &pending_crlf);
                    rb_str_append(output, rb_str_new((const char *)escaped_nowiki_end_literal, sizeof(escaped_nowiki_end_literal)));
                }
                break;

            case STRONG_EM:
                if (rb_ary_includes(scope, INT2FIX(NO_WIKI_START)) || rb_ary_includes(scope, INT2FIX(PRE)))
                {
                    // already in <nowiki> span or <pre> block
                    rb_str_append(output, rb_str_new((const char *)escaped_strong_em_literal, sizeof(escaped_strong_em_literal)));
                    break;
                }

                _wiki_text_pop_excess_elements(scope, line, output, line_ending);

                // if you've seen STRONG or EM, must close them in the reverse order that you saw them! otherwise, must open them
                remove_strong  = -1;
                remove_em      = -1;
                i              = RARRAY(scope)->len;
                for (i = i - 1; i >= 0; i--)
                {
                    long val = FIX2INT(rb_ary_entry(scope, i));
                    if (val == STRONG)
                    {
                        rb_str_append(output, rb_str_new((const char *)strong_end_literal, sizeof(strong_end_literal)));
                        remove_strong = i;
                    }
                    else if (val == EM)
                    {
                        rb_str_append(output, rb_str_new((const char *)em_end_literal, sizeof(em_end_literal)));
                        remove_em = i;
                    }
                }

                if (remove_strong > remove_em)      // must remove strong first
                {
                    rb_ary_delete_at(scope, remove_strong);
                    if (remove_em > -1)
                        rb_ary_delete_at(scope, remove_em);
                    else    // there was no em to remove!, so consider this an opening em tag
                    {
                        rb_str_append(output, rb_str_new((const char *)em_start_literal, sizeof(em_start_literal)));
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
                        rb_str_append(output, rb_str_new((const char *)strong_start_literal, sizeof(strong_start_literal)));
                        rb_ary_push(scope, INT2FIX(STRONG));
                        rb_ary_push(line, INT2FIX(STRONG));
                    }
                }
                else    // no strong or em to remove, so this must be a new opening of both
                {
                    _wiki_text_start_para_if_necessary(scope, line, output, &pending_crlf);
                    rb_str_append(output, rb_str_new((const char *)strong_em_literal, sizeof(strong_em_literal)));
                    rb_ary_push(scope, INT2FIX(STRONG));
                    rb_ary_push(line, INT2FIX(STRONG));
                    rb_ary_push(scope, INT2FIX(EM));
                    rb_ary_push(line, INT2FIX(EM));
                }
                break;

            case STRONG:
                if (rb_ary_includes(scope, INT2FIX(NO_WIKI_START)) || rb_ary_includes(scope, INT2FIX(PRE)))
                    // already in <nowiki> span or <pre> block
                    rb_str_append(output, rb_str_new((const char *)escaped_strong_literal, sizeof(escaped_strong_literal)));
                else if (rb_ary_includes(scope, INT2FIX(STRONG)))
                    // STRONG already seen, this is a closing tag
                    _wiki_text_pop_from_stack_up_to(scope, output, INT2FIX(STRONG), Qtrue, line_ending);
                else
                {
                    // this is a new opening
                    _wiki_text_pop_excess_elements(scope, line, output, line_ending);
                    _wiki_text_start_para_if_necessary(scope, line, output, &pending_crlf);
                    rb_str_append(output, rb_str_new((const char *)strong_start_literal, sizeof(strong_start_literal)));
                    rb_ary_push(scope, INT2FIX(STRONG));
                    rb_ary_push(line, INT2FIX(STRONG));
                }
                break;

            case EM:
                if (rb_ary_includes(scope, INT2FIX(NO_WIKI_START)) || rb_ary_includes(scope, INT2FIX(PRE)))
                    // already in <nowiki> span or <pre> block
                    rb_str_append(output, rb_str_new((const char *)escaped_em_literal, sizeof(escaped_em_literal)));
                else if (rb_ary_includes(scope, INT2FIX(EM)))
                    // EM already seen, this is a closing tag
                    _wiki_text_pop_from_stack_up_to(scope, output, INT2FIX(EM), Qtrue, line_ending);
                else
                {
                    // this is a new opening
                    _wiki_text_pop_excess_elements(scope, line, output, line_ending);
                    _wiki_text_start_para_if_necessary(scope, line, output, &pending_crlf);
                    rb_str_append(output, rb_str_new((const char *)em_start_literal, sizeof(em_start_literal)));
                    rb_ary_push(scope, INT2FIX(EM));
                    rb_ary_push(line, INT2FIX(EM));
                }
                break;

            case TT_START:
                if (rb_ary_includes(scope, INT2FIX(NO_WIKI_START)) || rb_ary_includes(scope, INT2FIX(PRE)) || rb_ary_includes(scope, INT2FIX(TT_START)))
                    // already in <nowiki> span, <pre> block, or TT_START span
                    rb_str_append(output, rb_str_new((const char *)escaped_tt_start_literal, sizeof(escaped_tt_start_literal)));
                else
                {
                    _wiki_text_pop_excess_elements(scope, line, output, line_ending);
                    _wiki_text_start_para_if_necessary(scope, line, output, &pending_crlf);
                    rb_str_append(output, rb_str_new((const char *)tt_start_literal, sizeof(tt_start_literal)));
                    rb_ary_push(scope, INT2FIX(TT_START));
                    rb_ary_push(line, INT2FIX(TT_START));
                }
                break;

            case TT_END:
                if (rb_ary_includes(scope, INT2FIX(NO_WIKI_START)) || rb_ary_includes(scope, INT2FIX(PRE)))
                    // already in <nowiki> span or <pre> block
                    rb_str_append(output, rb_str_new((const char *)escaped_tt_end_literal, sizeof(escaped_tt_end_literal)));
                else if (rb_ary_includes(scope, INT2FIX(TT_START)))
                    _wiki_text_pop_from_stack_up_to(scope, output, INT2FIX(TT_START), Qtrue, line_ending);
                else
                {
                    // no TT_START in scope, so must interpret the TT_END without any special meaning
                    _wiki_text_pop_excess_elements(scope, line, output, line_ending);
                    _wiki_text_start_para_if_necessary(scope, line, output, &pending_crlf);
                    rb_str_append(output, rb_str_new((const char *)escaped_tt_end_literal, sizeof(escaped_tt_end_literal)));
                }
                break;

            case OL:
            case UL:
                if (rb_ary_includes(scope, INT2FIX(NO_WIKI_START)))
                {
                    // already in <nowiki> span (no need to check for <pre>; can never appear inside it)
                    rb_str_append(output, rb_str_new((const char *)token->start, (token->stop + 1 - token->start)));
                    break;
                }

                // count number of tokens in line and scope stacks
                i = RARRAY(line)->len;
                j = RARRAY(scope)->len;

                // list tokens can be nested so look ahead for any more which might affect the decision to push or pop
                for (;;)
                {
                    if (!token)
                        token = lexer->pLexer->tokSource->nextToken(lexer->pLexer->tokSource);
                    if (!token)
                        continue;

                    type = token->getType(token);
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
                                    _wiki_text_pop_from_stack(scope, output, line_ending);

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
                            _wiki_text_pop_from_stack(scope, output, line_ending);
                        else if (j > i)
                        {
                            // item just pushed onto line does not match corresponding slot of scope!
                            for (; j >= i; j--)
                                // must pop back before emitting
                                _wiki_text_pop_from_stack(scope, output, line_ending);
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
                    rb_str_append(output, rb_str_new((const char *)ol_start_literal, sizeof(ol_start_literal)));
                else if (type == UL)
                    rb_str_append(output, rb_str_new((const char *)ul_start_literal, sizeof(ul_start_literal)));

                if (type == OL || type == UL)
                {
                    rb_ary_push(scope, INT2FIX(type));
                    rb_str_append(output, line_ending);
                }
                else if (type == SPACE)
                    // silently throw away the optional SPACE token after final list marker
                    token = NULL;

                rb_str_append(output, rb_str_new((const char *)li_start_literal, sizeof(li_start_literal)));
                rb_ary_push(scope, INT2FIX(LI));

                // any subsequent UL or OL tokens on this line are syntax errors and must be emitted literally
                if (type == OL || type == UL)
                {
                    k = 0;
                    while (token = lexer->pLexer->tokSource->nextToken(lexer->pLexer->tokSource))
                    {
                        type = token->getType(token);
                        if (k == 0 && type == SPACE)
                            // silently throw away the optional SPACE token after final list marker
                            token = NULL;
                        k++;
                        if (type == OL || type == UL)
                        {
                            rb_str_append(output, rb_str_new((const char *)token->start, (token->stop + 1 - token->start)));
                            token = NULL;
                        }
                        else
                            break;
                    }
                }

                if (token != NULL)
                    // we got a non-list token from the lexer; still needs to be processed, so jump to top of loop
                    continue;

                break;

            case H6_START:
            case H5_START:
            case H4_START:
            case H3_START:
            case H2_START:
            case H1_START:
                if (rb_ary_includes(scope, INT2FIX(NO_WIKI_START)))
                {
                    // already in <nowiki> span (no need to check for <pre>; can never appear inside it)
                    rb_str_append(output, rb_str_new((const char *)token->start, (token->stop + 1 - token->start)));
                    break;
                }

                // pop up to but not including the last BLOCKQUOTE on the scope stack
                _wiki_text_pop_from_stack_up_to(scope, output, INT2FIX(BLOCKQUOTE), Qfalse, line_ending);

                // count number of BLOCKQUOTE tokens in line buffer and in scope stack
                rb_ary_push(line, INT2FIX(type));
                i = _wiki_text_count(INT2FIX(BLOCKQUOTE), line);
                j = _wiki_text_count(INT2FIX(BLOCKQUOTE), scope);

                // decide whether we need to pop off excess BLOCKQUOTE tokens (will never need to push; that is handled above in the BLOCKQUOTE case itself)
                if (i < j)
                {
                    // must pop (reduce nesting level)
                    for (i = j - i; i > 0; i--)
                        _wiki_text_pop_from_stack_up_to(scope, output, INT2FIX(BLOCKQUOTE), Qtrue, line_ending);
                }

                // discard any whitespace here (so that "== foo ==" will be translated to "<h2>foo</h2>" rather than "<h2> foo </h2")
                token = NULL;
                while ((token = lexer->pLexer->tokSource->nextToken(lexer->pLexer->tokSource)) && (token->getType(token) == SPACE))
                    token = NULL; // discard

                rb_ary_push(scope, INT2FIX(type));

                // rather than repeat all that code for each kind of heading, share it and use a conditional here
                if (type == H6_START)
                    rb_str_append(output, rb_str_new((const char *)h6_start_literal, sizeof(h6_start_literal)));
                else if (type == H5_START)
                    rb_str_append(output, rb_str_new((const char *)h5_start_literal, sizeof(h5_start_literal)));
                else if (type == H4_START)
                    rb_str_append(output, rb_str_new((const char *)h4_start_literal, sizeof(h4_start_literal)));
                else if (type == H3_START)
                    rb_str_append(output, rb_str_new((const char *)h3_start_literal, sizeof(h3_start_literal)));
                else if (type == H2_START)
                    rb_str_append(output, rb_str_new((const char *)h2_start_literal, sizeof(h2_start_literal)));
                else if (type == H1_START)
                    rb_str_append(output, rb_str_new((const char *)h1_start_literal, sizeof(h1_start_literal)));

                if (token != NULL)
                    // we got a non-SPACE token from the lexer, which still needs to be processed, so jump to top of loop
                    continue;
                break;

            case H6_END:
                if (rb_ary_includes(scope, INT2FIX(NO_WIKI_START)) || rb_ary_includes(scope, INT2FIX(PRE)))
                    // already in <nowiki> span or <pre> block
                    rb_str_append(output, rb_str_new((const char *)escaped_h6_start_literal, sizeof(escaped_h6_start_literal)));
                else if (!rb_ary_includes(scope, INT2FIX(H6_START)))
                {
                    // literal output only if not in h6 scope (we stay silent in that case)
                    _wiki_text_start_para_if_necessary(scope, line, output, &pending_crlf);
                    rb_str_append(output, rb_str_new((const char *)escaped_h6_start_literal, sizeof(escaped_h6_start_literal)));
                }
                break;

            case H5_END:
                if (rb_ary_includes(scope, INT2FIX(NO_WIKI_START)) || rb_ary_includes(scope, INT2FIX(PRE)))
                    // already in <nowiki> span or <pre> block
                    rb_str_append(output, rb_str_new((const char *)escaped_h5_start_literal, sizeof(escaped_h5_start_literal)));
                else if (!rb_ary_includes(scope, INT2FIX(H5_START)))
                {
                    // literal output only if not in h5 scope (we stay silent in that case)
                    _wiki_text_start_para_if_necessary(scope, line, output, &pending_crlf);
                    rb_str_append(output, rb_str_new((const char *)escaped_h5_start_literal, sizeof(escaped_h5_start_literal)));
                }
                break;

            case H4_END:
                if (rb_ary_includes(scope, INT2FIX(NO_WIKI_START)) || rb_ary_includes(scope, INT2FIX(PRE)))
                    // already in <nowiki> span or <pre> block
                    rb_str_append(output, rb_str_new((const char *)escaped_h4_start_literal, sizeof(escaped_h4_start_literal)));
                else if (!rb_ary_includes(scope, INT2FIX(H4_START)))
                {
                    // literal output only if not in h4 scope (we stay silent in that case)
                    _wiki_text_start_para_if_necessary(scope, line, output, &pending_crlf);
                    rb_str_append(output, rb_str_new((const char *)escaped_h4_start_literal, sizeof(escaped_h4_start_literal)));
                }
                break;

            case H3_END:
                if (rb_ary_includes(scope, INT2FIX(NO_WIKI_START)) || rb_ary_includes(scope, INT2FIX(PRE)))
                    // already in <nowiki> span or <pre> block
                    rb_str_append(output, rb_str_new((const char *)escaped_h3_start_literal, sizeof(escaped_h3_start_literal)));
                else if (!rb_ary_includes(scope, INT2FIX(H3_START)))
                {
                    // literal output only if not in h3 scope (we stay silent in that case)
                    _wiki_text_start_para_if_necessary(scope, line, output, &pending_crlf);
                    rb_str_append(output, rb_str_new((const char *)escaped_h3_start_literal, sizeof(escaped_h3_start_literal)));
                }
                break;

            case H2_END:
                if (rb_ary_includes(scope, INT2FIX(NO_WIKI_START)) || rb_ary_includes(scope, INT2FIX(PRE)))
                    // already in <nowiki> span or <pre> block
                    rb_str_append(output, rb_str_new((const char *)escaped_h2_start_literal, sizeof(escaped_h2_start_literal)));
                else if (!rb_ary_includes(scope, INT2FIX(H2_START)))
                {
                    // literal output only if not in h2 scope (we stay silent in that case)
                    _wiki_text_start_para_if_necessary(scope, line, output, &pending_crlf);
                    rb_str_append(output, rb_str_new((const char *)escaped_h2_start_literal, sizeof(escaped_h2_start_literal)));
                }
                break;

            case H1_END:
                if (rb_ary_includes(scope, INT2FIX(NO_WIKI_START)) || rb_ary_includes(scope, INT2FIX(PRE)))
                    // already in <nowiki> span or <pre> block
                    rb_str_append(output, rb_str_new((const char *)escaped_h1_start_literal, sizeof(escaped_h1_start_literal)));
                else if (!rb_ary_includes(scope, INT2FIX(H1_START)))
                {
                    // literal output only if not in h1 scope (we stay silent in that case)
                    _wiki_text_start_para_if_necessary(scope, line, output, &pending_crlf);
                    rb_str_append(output, rb_str_new((const char *)escaped_h1_start_literal, sizeof(escaped_h1_start_literal)));
                }
                break;

            case URI:
                break;

            case LINK_START:
                break;

            case LINK_END:
                break;

            case EXT_LINK_START:
                break;

            case EXT_LINK_END:
                break;

            case SEPARATOR:
                break;

            case SPACE:
                _wiki_text_pop_excess_elements(scope, line, output, line_ending);

                // check for runs of spaces
                i       = (long)token->start;   // initial starting position (pointer into input stream)
                j       = 2;                    // initial length (one char, two bytes)
                token   = NULL;
                while ((token = lexer->pLexer->tokSource->nextToken(lexer->pLexer->tokSource)) && (token->type == SPACE))
                {
                    j += 2;                     // two bytes per char
                    token = NULL;
                }

                if (rb_ary_includes(scope, INT2FIX(NO_WIKI_START)))
                    // already in <nowiki> span
                    rb_str_append(output, rb_str_new((const char *)i, j));
                else if (token &&
                    (((token->type == H6_END) && rb_ary_includes(scope, INT2FIX(H6_START))) ||
                    ((token->type == H5_END) && rb_ary_includes(scope, INT2FIX(H5_START))) ||
                    ((token->type == H4_END) && rb_ary_includes(scope, INT2FIX(H4_START))) ||
                    ((token->type == H3_END) && rb_ary_includes(scope, INT2FIX(H3_START))) ||
                    ((token->type == H2_END) && rb_ary_includes(scope, INT2FIX(H2_START))) ||
                    ((token->type == H1_END) && rb_ary_includes(scope, INT2FIX(H1_START)))))
                {
                    // will suppress emission of space (discard) if next token is a H6_END, H5_END etc and we are in the corresponding scope
                }
                else
                {
                    _wiki_text_start_para_if_necessary(scope, line, output, &pending_crlf);
                    rb_str_append(output, rb_str_new((const char *)i, j));
                }

                if (token != NULL)
                    // we got a non-SPACE token from the lexer, which still needs to be processed, so jump to top of loop
                    continue;

                break;

            case ENTITY:
                _wiki_text_pop_excess_elements(scope, line, output, line_ending);
                _wiki_text_start_para_if_necessary(scope, line, output, &pending_crlf);
                rb_str_append(output, rb_str_new((const char *)token->start, (token->stop + 1 - token->start)));
                break;

            case QUOT:
                _wiki_text_pop_excess_elements(scope, line, output, line_ending);
                _wiki_text_start_para_if_necessary(scope, line, output, &pending_crlf);
                rb_str_append(output, rb_str_new((const char *)quot_entity_literal, sizeof(quot_entity_literal)));
                break;

            case AMP:
                _wiki_text_pop_excess_elements(scope, line, output, line_ending);
                _wiki_text_start_para_if_necessary(scope, line, output, &pending_crlf);
                rb_str_append(output, rb_str_new((const char *)amp_entity_literal, sizeof(amp_entity_literal)));
                break;

            case LESS:
                _wiki_text_pop_excess_elements(scope, line, output, line_ending);
                _wiki_text_start_para_if_necessary(scope, line, output, &pending_crlf);
                rb_str_append(output, rb_str_new((const char *)lt_entity_literal, sizeof(lt_entity_literal)));
                break;

            case GREATER:
                _wiki_text_pop_excess_elements(scope, line, output, line_ending);
                _wiki_text_start_para_if_necessary(scope, line, output, &pending_crlf);
                rb_str_append(output, rb_str_new((const char *)gt_entity_literal, sizeof(gt_entity_literal)));
                break;

            case CRLF:
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
                    i = _wiki_text_count(INT2FIX(BLOCKQUOTE), line);
                    for (j = RARRAY(scope)->len; j > i; j--)
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
                        _wiki_text_pop_from_stack(scope, output, line_ending);
                    }
                }

                // delete the entire contents of the line scope stack and buffer
                while (!NIL_P(rb_ary_delete_at(line, -1)));
                while (!NIL_P(rb_ary_delete_at(line_buffer, -1)));

                break;

            case PRINTABLE:
                _wiki_text_pop_excess_elements(scope, line, output, line_ending);
                // given that PRINTABLE tokens will often come in runs, we peek ahead and see if there are any more so as to handle them all at once
                i       = (long)token->start;   // initial starting position (pointer into input stream)
                j       = 2;                    // initial length (one char, two bytes)
                token   = NULL;
                while ((token = lexer->pLexer->tokSource->nextToken(lexer->pLexer->tokSource)) && (token->getType(token) == PRINTABLE))
                {
                    j += 2;                     // two bytes per char
                    token = NULL;
                }

                _wiki_text_start_para_if_necessary(scope, line, output, &pending_crlf);
                rb_str_append(output, rb_str_new((const char *)i, j));

                if (token != NULL)
                    // we got a non-PRINTABLE token from the lexer, which still needs to be processed, so jump to top of loop
                    continue;

                break;

            case DEFAULT:
                _wiki_text_pop_excess_elements(scope, line, output, line_ending);
                _wiki_text_start_para_if_necessary(scope, line, output, &pending_crlf);

                // convert to hexadecimal numeric entity
                i               = *((pANTLR3_UINT16)token->start);  // the character
                j               = (i & 0xf000) >> 12;               // start at most significant nibble
                hex_string[3]   = (j <= 9 ? j + 48 : j + 87);       // convert 0-15 to UCS-2: '0'-'9', 'a'-'f'
                j               = (i & 0x0f00) >> 8;
                hex_string[4]   = (j <= 9 ? j + 48 : j + 87);
                j               = (i & 0x00f0) >> 4;
                hex_string[5]   = (j <= 9 ? j + 48 : j + 87);
                j               = (i & 0x000f);
                hex_string[6]   = (j <= 9 ? j + 48 : j + 87);
                rb_str_append(output, rb_str_new((const char *)hex_string, sizeof(hex_string)));
                break;

            case ANTLR3_TOKEN_EOF:
                // close any open scopes on hitting EOF
                for (i = 0, j = RARRAY(scope)->len; i < j; i++)
                    _wiki_text_pop_from_stack(scope, output, line_ending);
                goto clean_up_token_stream; // break not enough here (want to break out of outer while loop, not inner switch statement)

            default:
                break;
        }

        // reset current token; forcing lexer to return another token at the top of the loop
        token = NULL;
    } while (1);

clean_up_token_stream:
    tstream->free(tstream);
clean_up_lexer:
    lexer->free(lexer);
clean_up_input_stream:
    stream->close(stream);
finalize:   // can raise exceptions only after all clean-up is done
    switch (exception)
    {
        case input_stream_memory:
            rb_raise(rb_eNoMemError, "failed to create input stream (memory allocation error)");
        case input_stream_other:
            rb_raise(rb_eSystemCallError, "failed to create input stream (antlr3NewUCS2StringInPlaceStream error %d)", (ANTLR3_UINT64)stream);
        case lexer_memory:
            rb_raise(rb_eNoMemError, "failed to create lexer (memory allocation error)");
        case lexer_other:
            rb_raise(rb_eSystemCallError, "failed to create lexer (WikiTextLexerNew error %d)", (ANTLR3_UINT64)lexer);
        case token_stream_memory:
            rb_raise(rb_eNoMemError, "failed to create token stream (memory allocation error)");
        case token_stream_other:
            rb_raise(rb_eSystemCallError, "failed to create token stream (antlr3CommonTokenStreamSourceNew error %d)", (ANTLR3_UINT64)tstream);
        default:
            break;
    }

    return _wiki_text_ucs2_to_utf8(output);
}

void Init_wiki_text()
{
    // modules
    mWikiText   = rb_define_module("WikiText");

    // classes
    cParser     = rb_define_class_under(mWikiText, "Parser", rb_cObject);

    // methods
    rb_define_method(cParser, "initialize", wiki_text_parser_initialize, 0);
    rb_define_method(cParser, "parse", wiki_text_parser_parse, -1);

    // accessors
    rb_define_attr(cParser, "line_ending", Qtrue, Qtrue);
    rb_define_attr(cParser, "internal_link_prefix", Qtrue, Qtrue);

    // TODO: add instance variables that tell the parser how to generate internal links (prefix)
    // eg. given [[foo bar]] and prefix "http://example.com/wiki/"
    // generate <a href="http://example.com/wiki/foo_bar">
    // unlike MediaWiki, the first char of the URL is to be case-insensitive?
    // or require the user to set up a redirect?
}
