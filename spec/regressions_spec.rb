#!/usr/bin/env ruby
# Copyright 2008 Wincent Colaiuta
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

require File.join(File.dirname(__FILE__), 'spec_helper.rb')
require 'wikitext'

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
  # PRINTABLE didn't only because it called _Wikitext_start_para_if_necessary(), which handled the pending CRLF
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

    # these tokens weren't affected by the bug, seeing as they either call _Wikitext_start_para_if_necessary()
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
end
