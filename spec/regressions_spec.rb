# Copyright 2008-present Greg Hurrell. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

require 'spec_helper'

# this is a general-purpose file in which I'll add specs for former bugs to make sure that they don't regress
describe Wikitext::Parser, 'regressions' do
  before do
    @parser = Wikitext::Parser.new
  end

  # turns out that this was never a bug in wikitext -- it was a bug in the host application -- but keeping the test does no harm
  it 'should correctly transform example #1' do
    input = dedent <<-END
      = Leopard =
      
      * punto 1
      * punto 2
      
      Y [[otro articulo]].
    END
    expected = dedent <<-END
      <h1>Leopard</h1>
      <ul>
        <li>punto 1</li>
        <li>punto 2</li>
      </ul>
      <p>Y <a href="/wiki/otro_articulo">otro articulo</a>.</p>
    END
    @parser.parse(input).should == expected
  end

  # discovered at: http://rails.wincent.com/wiki/nginx_log_rotation
  # fixed by 0a328f1
  it 'should allow empty lines in PRE blocks marked up with a leading space' do
    input = dedent <<-END
       # -d turns on debug mode: output is verbose, no actual changes are made to the log files
       sudo logrotate -d /etc/logrotate.d/nginx
       
       # if the debug output looks good, proceed with a real rotation (-v turns on verbose output)
       sudo logrotate -v /etc/logrotate.d/nginx
    END
    expected = dedent <<-END
      <pre># -d turns on debug mode: output is verbose, no actual changes are made to the log files
      sudo logrotate -d /etc/logrotate.d/nginx
      
      # if the debug output looks good, proceed with a real rotation (-v turns on verbose output)
      sudo logrotate -v /etc/logrotate.d/nginx</pre>
    END
    @parser.parse(input).should == expected
  end

  # discovered at: http://rails.wincent.com/wiki/Installing_Ragel_5.2.0_on_Mac_OS_X_Tiger
  # fixed by a616841
  it 'should handle PRE_START blocks which follow unordered lists' do
    input = dedent <<-END
      * Get link to latest source code from: http://www.cs.queensu.ca/~thurston/ragel/
      
      <pre>wget http://www.cs.queensu.ca/~thurston/ragel/ragel-5.20.tar.gz
      tar xzvf ragel-5.20.tar.gz
      cd ragel-5.20</pre>
    END
    expected = dedent <<-END
      <ul>
        <li>Get link to latest source code from: <a href="http://www.cs.queensu.ca/~thurston/ragel/" class="external">http://www.cs.queensu.ca/~thurston/ragel/</a></li>
      </ul>
      <pre>wget <a href="http://www.cs.queensu.ca/~thurston/ragel/ragel-5.20.tar.gz" class="external">http://www.cs.queensu.ca/~thurston/ragel/ragel-5.20.tar.gz</a>
      tar xzvf ragel-5.20.tar.gz
      cd ragel-5.20</pre>
    END
    @parser.parse(input).should == expected
  end

  # discovered at: http://rails.wincent.com/wiki/Movable_Type_security_notes
  # fixed by a616841
  it 'should handle PRE_START blocks which follow ordered lists' do
    input = dedent <<-END
      # Turn off the [[Movable Type]] search function; use Google instead (it's better anyway) with a form something like this:
      
      <pre><form method="get"...></pre>
    END
    expected = dedent <<-END
      <ol>
        <li>Turn off the <a href="/wiki/Movable_Type">Movable Type</a> search function; use Google instead (it's better anyway) with a form something like this:</li>
      </ol>
      <pre>&lt;form method=&quot;get&quot;...&gt;</pre>
    END
    @parser.parse(input).should == expected
  end

  # discovered at: http://rails.wincent.com/wiki/Movable_Type_security_notes
  # fixed by 191b75d
  it 'should respect additional indentation found inside PRE blocks' do
    # note the two extra spaces on each line
    input = dedent <<-END
         <input type="text" name="q" size="20" maxlength="255" value="" />
         <input type="hidden" name="hl" value="en" />
    END

    # problem is the spaces were being emitted _before_ the CRLF
    expected = dedent <<-END
      <pre>  &lt;input type=&quot;text&quot; name=&quot;q&quot; size=&quot;20&quot; maxlength=&quot;255&quot; value=&quot;&quot; /&gt;
        &lt;input type=&quot;hidden&quot; name=&quot;hl&quot; value=&quot;en&quot; /&gt;</pre>
    END
    @parser.parse(input).should == expected
  end

  # this is the general case of the bug covered in the previous spec
  # any token that appears as the first token after a PRE token can manifest this bug
  # PRINTABLE didn't only because it called wiki_start_para_if_necessary(), which handled the pending CRLF
  it 'should emit pending newlines for all token types found inside PRE and PRE_START blocks' do
    # PRE_START
    input = dedent <<-END
       foo
       <pre>bar
    END
    expected = dedent <<-END
      <pre>foo
      &lt;pre&gt;bar</pre>
    END
    @parser.parse(input).should == expected

    # PRE_END
    input = dedent <<-END
       foo
       </pre>bar
    END
    expected = dedent <<-END
      <pre>foo
      &lt;/pre&gt;bar</pre>
    END
    @parser.parse(input).should == expected

    # BLOCKQUOTE_START
    input = dedent <<-END
       foo
       <blockquote>bar
    END
    expected = dedent <<-END
      <pre>foo
      &lt;blockquote&gt;bar</pre>
    END
    @parser.parse(input).should == expected

    # BLOCKQUOTE_END
    input = dedent <<-END
       foo
       </blockquote>bar
    END
    expected = dedent <<-END
      <pre>foo
      &lt;/blockquote&gt;bar</pre>
    END
    @parser.parse(input).should == expected

    # NO_WIKI_START
    input = dedent <<-END
       foo
       <nowiki>bar
    END
    expected = dedent <<-END
      <pre>foo
      &lt;nowiki&gt;bar</pre>
    END
    @parser.parse(input).should == expected

    # STRONG_EM
    input = dedent <<-END
       foo
       '''''bar
    END
    expected = dedent <<-END
      <pre>foo
      '''''bar</pre>
    END
    @parser.parse(input).should == expected

    # STRONG
    input = dedent <<-END
       foo
       '''bar
    END
    expected = dedent <<-END
      <pre>foo
      '''bar</pre>
    END
    @parser.parse(input).should == expected

    # STRONG_START
    input = dedent <<-END
       foo
       <strong>bar
    END
    expected = dedent <<-END
      <pre>foo
      &lt;strong&gt;bar</pre>
    END
    @parser.parse(input).should == expected

    # STRONG_END
    input = dedent <<-END
       foo
       </strong>bar
    END
    expected = dedent <<-END
      <pre>foo
      &lt;/strong&gt;bar</pre>
    END
    @parser.parse(input).should == expected

    # EM
    input = dedent <<-END
       foo
       ''bar
    END
    expected = dedent <<-END
      <pre>foo
      ''bar</pre>
    END
    @parser.parse(input).should == expected

    # EM_START
    input = dedent <<-END
       foo
       <em>bar
    END
    expected = dedent <<-END
      <pre>foo
      &lt;em&gt;bar</pre>
    END
    @parser.parse(input).should == expected

    # EM_END
    input = dedent <<-END
       foo
       </em>bar
    END
    expected = dedent <<-END
      <pre>foo
      &lt;/em&gt;bar</pre>
    END
    @parser.parse(input).should == expected

    # TT
    input = dedent <<-END
       foo
       `bar
    END
    expected = dedent <<-END
      <pre>foo
      `bar</pre>
    END
    @parser.parse(input).should == expected

    # TT_START
    input = dedent <<-END
       foo
       <tt>bar
    END
    expected = dedent <<-END
      <pre>foo
      &lt;tt&gt;bar</pre>
    END
    @parser.parse(input).should == expected

    # TT_END
    input = dedent <<-END
       foo
       </tt>bar
    END
    expected = dedent <<-END
      <pre>foo
      &lt;/tt&gt;bar</pre>
    END
    @parser.parse(input).should == expected

    # H6_END
    input = dedent <<-END
       foo
       ======
       bar
    END
    expected = dedent <<-END
      <pre>foo
      ======
      bar</pre>
    END
    @parser.parse(input).should == expected

    # H5_END
    input = dedent <<-END
       foo
       =====
       bar
    END
    expected = dedent <<-END
      <pre>foo
      =====
      bar</pre>
    END
    @parser.parse(input).should == expected

    # H4_END
    input = dedent <<-END
       foo
       ====
       bar
    END
    expected = dedent <<-END
      <pre>foo
      ====
      bar</pre>
    END
    @parser.parse(input).should == expected

    # H3_END
    input = dedent <<-END
       foo
       ===
       bar
    END
    expected = dedent <<-END
      <pre>foo
      ===
      bar</pre>
    END
    @parser.parse(input).should == expected

    # H2_END
    input = dedent <<-END
       foo
       ==
       bar
    END
    expected = dedent <<-END
      <pre>foo
      ==
      bar</pre>
    END
    @parser.parse(input).should == expected

    # H1_END
    input = dedent <<-END
       foo
       =
       bar
    END
    expected = dedent <<-END
      <pre>foo
      =
      bar</pre>
    END
    @parser.parse(input).should == expected

    # MAIL
    input = dedent <<-END
       foo
       bar@baz.com
    END
    expected = dedent <<-END
      <pre>foo
      bar@baz.com</pre>
    END
    @parser.parse(input).should == expected

    # LINK_START
    input = dedent <<-END
       foo
       [[bar
    END
    expected = dedent <<-END
      <pre>foo
      [[bar</pre>
    END
    @parser.parse(input).should == expected

    # LINK_END
    input = dedent <<-END
       foo
       ]]bar
    END
    expected = dedent <<-END
      <pre>foo
      ]]bar</pre>
    END
    @parser.parse(input).should == expected

    # EXT_LINK_START
    input = dedent <<-END
       foo
       [bar
    END
    expected = dedent <<-END
      <pre>foo
      [bar</pre>
    END
    @parser.parse(input).should == expected

    # EXT_LINK_END
    input = dedent <<-END
       foo
       ]bar
    END
    expected = dedent <<-END
      <pre>foo
      ]bar</pre>
    END
    @parser.parse(input).should == expected

    # IMG_START
    input = dedent <<-END
       foo
       {{bar
    END
    expected = dedent <<-END
      <pre>foo
      {{bar</pre>
    END
    @parser.parse(input).should == expected

    # these tokens weren't affected by the bug, seeing as they either call wiki_start_para_if_necessary()
    # or they can only appear in PRE_START (not PRE) thanks to the tokenizer
    # but we add specs for them to make sure that the issue never crops up for them in the future

    # PRE (in PRE_START)
    input = dedent <<-END
      <pre>foo
       bar</pre>
    END
    expected = dedent <<-END
      <pre>foo
       bar</pre>
    END
    @parser.parse(input).should == expected

    # BLOCKQUOTE (in PRE_START)
    input = dedent <<-END
      <pre>foo
      > bar</pre>
    END
    expected = dedent <<-END
      <pre>foo
      &gt; bar</pre>
    END
    @parser.parse(input).should == expected

    # OL (in PRE_START)
    input = dedent <<-END
      <pre># foo
      # bar</pre>
    END
    expected = dedent <<-END
      <pre># foo
      # bar</pre>
    END
    @parser.parse(input).should == expected

    # UL (in PRE_START)
    input = dedent <<-END
      <pre>* foo
      * bar</pre>
    END
    expected = dedent <<-END
      <pre>* foo
      * bar</pre>
    END
    @parser.parse(input).should == expected

    # H6_START (in PRE_START)
    input = dedent <<-END
      <pre>foo
      ====== bar
      baz</pre>
    END
    expected = dedent <<-END
      <pre>foo
      ====== bar
      baz</pre>
    END
    @parser.parse(input).should == expected

    # H5_START (in PRE_START)
    input = dedent <<-END
      <pre>foo
      ===== bar
      baz</pre>
    END
    expected = dedent <<-END
      <pre>foo
      ===== bar
      baz</pre>
    END
    @parser.parse(input).should == expected

    # H4_START (in PRE_START)
    input = dedent <<-END
      <pre>foo
      ==== bar
      baz</pre>
    END
    expected = dedent <<-END
      <pre>foo
      ==== bar
      baz</pre>
    END
    @parser.parse(input).should == expected

    # H3_START (in PRE_START)
    input = dedent <<-END
      <pre>foo
      === bar
      baz</pre>
    END
    expected = dedent <<-END
      <pre>foo
      === bar
      baz</pre>
    END
    @parser.parse(input).should == expected

    # H2_START (in PRE_START)
    input = dedent <<-END
      <pre>foo
      == bar
      baz</pre>
    END
    expected = dedent <<-END
      <pre>foo
      == bar
      baz</pre>
    END
    @parser.parse(input).should == expected

    # H1_START (in PRE_START)
    input = dedent <<-END
      <pre>foo
      = bar
      baz</pre>
    END
    expected = dedent <<-END
      <pre>foo
      = bar
      baz</pre>
    END
    @parser.parse(input).should == expected

    # NO_WIKI_END
    input = dedent <<-END
       foo
       </nowiki>bar
    END
    expected = dedent <<-END
      <pre>foo
      &lt;/nowiki&gt;bar</pre>
    END
    @parser.parse(input).should == expected

    # SEPARATOR
    input = dedent <<-END
       foo
       |bar
    END
    expected = dedent <<-END
      <pre>foo
      |bar</pre>
    END
    @parser.parse(input).should == expected

    # QUOT_ENTITY
    input = dedent <<-END
       foo
       &quot;bar
    END
    expected = dedent <<-END
      <pre>foo
      &quot;bar</pre>
    END
    @parser.parse(input).should == expected

    # AMP_ENTITY
    input = dedent <<-END
       foo
       &amp;bar
    END
    expected = dedent <<-END
      <pre>foo
      &amp;bar</pre>
    END
    @parser.parse(input).should == expected

    # NAMED_ENTITY
    input = dedent <<-END
       foo
       &raquo;bar
    END
    expected = dedent <<-END
      <pre>foo
      &raquo;bar</pre>
    END
    @parser.parse(input).should == expected

    # DECIMAL_ENTITY
    input = dedent <<-END
       foo
       &#1234;bar
    END
    expected = dedent <<-END
      <pre>foo
      &#1234;bar</pre>
    END
    @parser.parse(input).should == expected

    # HEX_ENTITY
    input = dedent <<-END
       foo
       &#x2022;bar
    END
    expected = dedent <<-END
      <pre>foo
      &#x2022;bar</pre>
    END
    @parser.parse(input).should == expected

    # QUOT
    input = dedent <<-END
       foo
       "bar
    END
    expected = dedent <<-END
      <pre>foo
      &quot;bar</pre>
    END
    @parser.parse(input).should == expected

    # AMP
    input = dedent <<-END
       foo
       &bar
    END
    expected = dedent <<-END
      <pre>foo
      &amp;bar</pre>
    END
    @parser.parse(input).should == expected

    # LESS
    input = dedent <<-END
       foo
       <bar
    END
    expected = dedent <<-END
      <pre>foo
      &lt;bar</pre>
    END
    @parser.parse(input).should == expected

    # GREATER
    input = dedent <<-END
       foo
       >bar
    END
    expected = dedent <<-END
      <pre>foo
      &gt;bar</pre>
    END
    @parser.parse(input).should == expected

    # URI
    input = dedent <<-END
       foo
       http://example.com/
    END
    expected = dedent <<-END
      <pre>foo
      <a href="http://example.com/" class="external">http://example.com/</a></pre>
    END
    @parser.parse(input).should == expected

    # PRINTABLE
    input = dedent <<-END
       foo
       bar
    END
    expected = dedent <<-END
      <pre>foo
      bar</pre>
    END
    @parser.parse(input).should == expected

    # IMG_END
    input = dedent <<-END
       foo
       }}bar
    END
    expected = dedent <<-END
      <pre>foo
      }}bar</pre>
    END
    @parser.parse(input).should == expected

    # LEFT_CURLY
    input = dedent <<-END
       foo
       {bar
    END
    expected = dedent <<-END
      <pre>foo
      {bar</pre>
    END
    @parser.parse(input).should == expected

    # RIGHT_CURLY
    input = dedent <<-END
       foo
       }bar
    END
    expected = dedent <<-END
      <pre>foo
      }bar</pre>
    END
    @parser.parse(input).should == expected

    # DEFAULT
    input = dedent <<-END
       foo
       €bar
    END
    expected = dedent <<-END
      <pre>foo
      &#x20ac;bar</pre>
    END
    @parser.parse(input).should == expected
  end

  # discovered at: http://rails.wincent.com/wiki/Testing_cookies_in_Rails
  it 'should handle BLOCKQUOTE_START blocks which follow lists' do
    # example text taken from wiki article and edited for brevity
    input = dedent <<-END
      * This article
      <blockquote>The cookies</blockquote>
    END
    expected = dedent <<-END
      <ul>
        <li>This article</li>
      </ul>
      <blockquote>
        <p>The cookies</p>
      </blockquote>
    END
    @parser.parse(input).should == expected
  end

  # https://wincent.com/issues/818
  it 'should handle BLOCKQUOTE_START blocks which follow BLOCKQUOTE shorthand' do
    input = dedent <<-END
      > foo
      <blockquote>bar</blockquote>
    END
    expected = dedent <<-END
      <blockquote>
        <p>foo</p>
      </blockquote>
      <blockquote>
        <p>bar</p>
      </blockquote>
    END
    @parser.parse(input).should == expected
  end

  # https://wincent.com/issues/818
  it 'should handle PRE_START blocks which follow BLOCKQUOTE shorthand' do
    input = dedent <<-END
      > foo
      <pre>bar</pre>
    END
    expected = dedent <<-END
      <blockquote>
        <p>foo</p>
      </blockquote>
      <pre>bar</pre>
    END
    @parser.parse(input).should == expected
  end

  # https://wincent.com/issues/818
  it 'should handle BLOCKQUOTE_START blocks which follow nested BLOCKQUOTE shorthand' do
    input = dedent <<-END
      >>> foo
      <blockquote>bar</blockquote>
    END
    expected = dedent <<-END
      <blockquote>
        <blockquote>
          <blockquote>
            <p>foo</p>
          </blockquote>
        </blockquote>
      </blockquote>
      <blockquote>
        <p>bar</p>
      </blockquote>
    END
    @parser.parse(input).should == expected
  end

  # https://wincent.com/issues/818
  it 'should handle PRE_START blocks which follow nested BLOCKQUOTE shorthand' do
    input = dedent <<-END
      >>> foo
      <pre>bar</pre>
    END
    expected = dedent <<-END
      <blockquote>
        <blockquote>
          <blockquote>
            <p>foo</p>
          </blockquote>
        </blockquote>
      </blockquote>
      <pre>bar</pre>
    END
    @parser.parse(input).should == expected
  end

  # https://wincent.com/issues/1289
  it 'should handle empty (zero-width) link targets' do
    # these were badly broken (caused exceptions to be raised)
    @parser.parse('[[]]').should == "<p>[[]]</p>\n"
    @parser.parse('[[|]]').should == "<p>[[|]]</p>\n"
    @parser.parse('[[|foo]]').should == "<p>[[|foo]]</p>\n"

    # was working, but check here anyway to guard against regressions
    @parser.parse('[[foo|]]').should == %Q{<p><a href="/wiki/foo">foo</a></p>\n}
  end

  it 'should handle empty (whitespace only) link targets' do
    # no exception raised, but clearly not desirable behaviour
    # formerly these all returned: <p><a href="/wiki/"></a></p>\n
    @parser.parse('[[ ]]').should == "<p>[[ ]]</p>\n"
    @parser.parse('[[  ]]').should == "<p>[[  ]]</p>\n"
    @parser.parse('[[  |]]').should == "<p>[[  |]]</p>\n"

    # and this one returned: <p><a href="/wiki/">foo</a></p>\n
    @parser.parse('[[  |foo]]').should == "<p>[[  |foo]]</p>\n"
  end

  # first manifested itself in this comment: https://wincent.com/comments/6427
  it 'handles "`[/`"' do
    # This is, of course, an invalid link, but it could be handled more
    # gracefully (we were opening a <code> span and instead of just rolling
    # back the failed link and then proceeding with the parse and closing
    # the span, we skipped the second backtick, causing the remainder of
    # the input to appear inside the <code> span).
    #
    # Although the bug manifested itself with backticks, it could also have
    # happened with any non-space token appearing after a EXT_LINK_START +
    # PATH sequence (any such token would just be dropped on the floor).
    @parser.parse('with `[/` then').should == "<p>with <code>[/</code> then</p>\n"

    # related example from the original ticket: https://wincent.com/issues/1726
    @parser.parse('[/bar?baz=bat link]').should == "<p>[/bar?baz=bat link]</p>\n"
  end

  it 'handles "[http://foo.com]"' do
    # Same bug as above, but with a EXT_LINK_START + URI rather than
    # EXT_LINK_START + PATH; again we expect to see a SPACE, but when we
    # see something else, we roll back and drop the unexpected token on
    # the floor.
    expected = %Q{<p>[<a href="http://foo.com" class="external">http://foo.com</a>]</p>\n}
    @parser.parse('[http://foo.com]').should == expected
  end

  # https://wincent.com/issues/1891
  it 'handles shorthand PRE blocks containing lines starting with slashes' do
    expected = "<pre>/a\n/b\n/c</pre>\n"
    @parser.parse(" /a\n /b\n /c").should == expected
  end
end
