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

#include "wikitext_ragel.h"
#include "parser.h"

VALUE mWikitext              = 0;   // Wikitext
VALUE cWikitextParser        = 0;   // Wikitext::Parser
VALUE eWikitextParserError   = 0;   // Wikitext::Parser::Error
VALUE cWikitextParserToken   = 0;   // Wikitext::Parser::Token

void Init_wikitext()
{
    // Wikitext
    mWikitext = rb_define_module("Wikitext");

    // Wikitext::Parser
    cWikitextParser = rb_define_class_under(mWikitext, "Parser", rb_cObject);
    rb_define_method(cWikitextParser, "initialize", Wikitext_parser_initialize, 0);
    rb_define_method(cWikitextParser, "parse", Wikitext_parser_parse, 1);
    rb_define_method(cWikitextParser, "profiling_parse", Wikitext_parser_profiling_parse, 1);
    rb_define_method(cWikitextParser, "tokenize", Wikitext_parser_tokenize, 1);
    rb_define_method(cWikitextParser, "benchmarking_tokenize", Wikitext_parser_benchmarking_tokenize, 1);

    // sanitizes an internal link target for inclusion with the HTML stream; for example, a link target for the article titled:
    //      foo, "bar" & baz €
    // would be sanitized as:
    //      foo, &quot;bar&quot; &amp; baz &#x20ac;
    rb_define_singleton_method(cWikitextParser, "sanitize_link_target", Wikitext_parser_sanitize_link_target, 1);

    // encodes an internal link target for use as an anchor href; for example, the link target:
    //      foo, "bar" & baz €
    // would be encoded as:
    //      foo%2c%20%22bar%22%20%26%20baz%e2%82%ac
    // and used as follows (combined with the output of sanitize_link_target):
    //      <a href="foo%2c%20%22bar%22%20%26%20baz%e2%82%ac">foo, &quot;bar&quot; &amp; baz &#x20ac;</a>
    rb_define_singleton_method(cWikitextParser, "encode_link_target", Wikitext_parser_encode_link_target, 1);

    // override default line_ending
    // defaults to "\n"
    rb_define_attr(cWikitextParser, "line_ending", Qtrue, Qtrue);

    // the prefix to be prepended to internal links; defaults to "/wiki/"
    // for example, given an internal_link_prefix of "/wiki/"
    //      [[Apple]]
    // would be transformed into:
    //      <a href="/wiki/Apple">Apple</a>
    rb_define_attr(cWikitextParser, "internal_link_prefix", Qtrue, Qtrue);

    // CSS class to be applied to external links; defaults to "external"
    // for example, given an external_link_class of "external":
    //      [http://www.google.com/ the best search engine]
    // would be transformed into:
    //      <a class="external" href="http://www.google.com/">the best search engine</a>
    rb_define_attr(cWikitextParser, "external_link_class", Qtrue, Qtrue);

    // CSS class to be applied to external links; defaults to "mailto"
    // for example:
    //      [mailto:user@example.com user@example.com]
    // or if autolinking of email addresses is turned on (not yet implemented) just
    //      user@example.com
    // would be transformed into:
    //      <a class="mailto" href="mailto:user@example.com">user@example.com</a>
    rb_define_attr(cWikitextParser, "mailto_class", Qtrue, Qtrue);

    // whether to autolink URIs found in the plain scope
    // when true:
    //      http://apple.com/
    // will be transformed to:
    //      <a href="http://apple.com/">http://apple.com/</a>
    // and if an external_link_class is set (to "external", for example) then the transformation will be:
    //      <a class="external" href="http://apple.com/">http://apple.com/</a>
    rb_define_attr(cWikitextParser, "autolink", Qtrue, Qtrue);

    // whether "slash" in link text is treated specially
    // when true, any link containing a slash is considered to be a relative link within the current site, but outside the wiki
    // in other words, while:
    //      [[interesting article]]
    // is a wiki link (assuming the internal_link_prefix of "/wiki/"):
    //      <a href="/wiki/interesting+article">interesting article</a>
    // in contrast:
    //      [[issue/400]]
    // is interpreted as a link external to the wiki but internal to the site, and is converted into:
    //      <a href="/issue/400">issue/400</a>
    // this design is intended to work well with preprocessors, that can scan the input for things like:
    //      issue #400
    // and transform them before feeding them into the wikitext parser as:
    //      [[issue/400|issue #400]]
    // which in turn would be transformed into:
    //      <a href="/issue/400">issue #400</a>
    rb_define_attr(cWikitextParser, "treat_slash_as_special", Qtrue, Qtrue);

    // Wikitext::Parser::Error
    eWikitextParserError = rb_define_class_under(cWikitextParser, "Error", rb_eException);

    // Wikitext::Parser::Token
    cWikitextParserToken = rb_define_class_under(cWikitextParser, "Token", rb_cObject);
    rb_define_singleton_method(cWikitextParserToken, "types", Wikitext_parser_token_types, 0);
    rb_define_attr(cWikitextParserToken, "start", Qtrue, Qfalse);
    rb_define_attr(cWikitextParserToken, "stop", Qtrue, Qfalse);
    rb_define_attr(cWikitextParserToken, "line_start", Qtrue, Qfalse);
    rb_define_attr(cWikitextParserToken, "line_stop", Qtrue, Qfalse);
    rb_define_attr(cWikitextParserToken, "column_start", Qtrue, Qfalse);
    rb_define_attr(cWikitextParserToken, "column_stop", Qtrue, Qfalse);
    rb_define_attr(cWikitextParserToken, "code_point", Qtrue, Qfalse);
    rb_define_attr(cWikitextParserToken, "token_type", Qtrue, Qfalse);
    rb_define_attr(cWikitextParserToken, "string_value", Qtrue, Qfalse);
}
