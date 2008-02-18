#!/usr/bin/env ruby
# Copyright 2007-2008 Wincent Colaiuta
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

describe Wikitext::Parser, 'internal links (space to underscore off)' do
  before do
    @parser = Wikitext::Parser.new
  end

  it 'should pass through unexpected link end tokens literally' do
    @parser.parse('foo ]] bar').should == "<p>foo ]] bar</p>\n"                                     # in plain scope
    @parser.parse("foo '']]'' bar").should == "<p>foo <em>]]</em> bar</p>\n"                        # in EM scope
    @parser.parse("foo ''']]''' bar").should == "<p>foo <strong>]]</strong> bar</p>\n"              # in STRONG scope
    @parser.parse("foo ''''']]''''' bar").should == "<p>foo <strong><em>]]</em></strong> bar</p>\n" # in STRONG_EM scope
    @parser.parse('foo <tt>]]</tt> bar').should == "<p>foo <tt>]]</tt> bar</p>\n"                   # in TT scope
    @parser.parse('= foo ]] bar =').should == "<h1>foo ]] bar</h1>\n"                               # in H1 scope
    @parser.parse('== foo ]] bar ==').should == "<h2>foo ]] bar</h2>\n"                             # in H2 scope
    @parser.parse('=== foo ]] bar ===').should == "<h3>foo ]] bar</h3>\n"                           # in H3 scope
    @parser.parse('==== foo ]] bar ====').should == "<h4>foo ]] bar</h4>\n"                         # in H4 scope
    @parser.parse('===== foo ]] bar =====').should == "<h5>foo ]] bar</h5>\n"                       # in H5 scope
    @parser.parse('====== foo ]] bar ======').should == "<h6>foo ]] bar</h6>\n"                     # in H6 scope
    @parser.parse('> ]]').should == "<blockquote>\n  <p>]]</p>\n</blockquote>\n"                    # in BLOCKQUOTE scope
  end

  it 'should turn single words into links' do
    @parser.parse('[[foo]]').should == %Q{<p><a href="/wiki/foo">foo</a></p>\n}
  end

  it 'should turn multiple words into links' do
    @parser.parse('[[foo bar]]').should == %Q{<p><a href="/wiki/foo%20bar">foo bar</a></p>\n}
  end

  it 'should trim leading whitespace' do
    @parser.parse('[[ foo]]').should == %Q{<p><a href="/wiki/foo">foo</a></p>\n}
    @parser.parse('[[  foo]]').should == %Q{<p><a href="/wiki/foo">foo</a></p>\n}
    @parser.parse('[[   foo]]').should == %Q{<p><a href="/wiki/foo">foo</a></p>\n}
    @parser.parse('[[    foo]]').should == %Q{<p><a href="/wiki/foo">foo</a></p>\n}
  end

  it 'should trim trailing whitespace' do
    @parser.parse('[[foo ]]').should == %Q{<p><a href="/wiki/foo">foo</a></p>\n}
    @parser.parse('[[foo  ]]').should == %Q{<p><a href="/wiki/foo">foo</a></p>\n}
    @parser.parse('[[foo   ]]').should == %Q{<p><a href="/wiki/foo">foo</a></p>\n}
    @parser.parse('[[foo    ]]').should == %Q{<p><a href="/wiki/foo">foo</a></p>\n}   # was a bug (exception)
    @parser.parse('[[foo     ]]').should == %Q{<p><a href="/wiki/foo">foo</a></p>\n}  # was a bug (crash)
  end

  it 'should trim leading and trailing whitespace (combined)' do
    @parser.parse('[[ foo    ]]').should == %Q{<p><a href="/wiki/foo">foo</a></p>\n}
    @parser.parse('[[  foo   ]]').should == %Q{<p><a href="/wiki/foo">foo</a></p>\n}
    @parser.parse('[[   foo  ]]').should == %Q{<p><a href="/wiki/foo">foo</a></p>\n}
    @parser.parse('[[    foo ]]').should == %Q{<p><a href="/wiki/foo">foo</a></p>\n}
  end

  it 'should leave embedded whitespace intact' do
    @parser.parse('[[ foo bar ]]').should == %Q{<p><a href="/wiki/foo%20bar">foo bar</a></p>\n}
    @parser.parse('[[foo bar ]]').should == %Q{<p><a href="/wiki/foo%20bar">foo bar</a></p>\n}
    @parser.parse('[[ foo bar ]]').should == %Q{<p><a href="/wiki/foo%20bar">foo bar</a></p>\n}
  end

  it 'should encode and sanitize quotes' do
    # note how percent encoding is used in the href, and named entities in the link text
    @parser.parse('[[hello "world"]]').should == %Q{<p><a href="/wiki/hello%20%22world%22">hello &quot;world&quot;</a></p>\n}
  end

  it 'should encode and sanitize ampersands' do
    @parser.parse('[[a & b]]').should == %Q{<p><a href="/wiki/a%20%26%20b">a &amp; b</a></p>\n}
  end

  it 'should allow ampersand entities (special exception)' do
    @parser.parse('[[a &amp; b]]').should == %Q{<p><a href="/wiki/a%20%26%20b">a &amp; b</a></p>\n}
  end

  it 'should allow quote entities (special exception)' do
    @parser.parse('[[a &quot; b]]').should == %Q{<p><a href="/wiki/a%20%22%20b">a &quot; b</a></p>\n}
  end

  it 'should handle mixed scenarios (quotes, ampersands, non-ASCII characers)' do
    expected = %Q{<p><a href="/wiki/foo%2c%20%22bar%22%20%26%20baz%20%e2%82%ac">foo, &quot;bar&quot; &amp; baz &#x20ac;</a></p>\n}
    @parser.parse('[[foo, "bar" & baz €]]').should == expected
  end

  it 'should handle links in paragraph flows' do
    expected = %Q{<p>foo <a href="/wiki/bar">bar</a> baz</p>\n}
    @parser.parse('foo [[bar]] baz').should == expected # was a bug
  end

  describe 'custom link text' do
    it 'should recognize link text placed after the separator' do
      @parser.parse('[[foo|bar]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
    end

    it 'should trim whitespace to the left of the separator' do
      @parser.parse('[[foo |bar]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
      @parser.parse('[[foo  |bar]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
      @parser.parse('[[foo   |bar]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
      @parser.parse('[[foo    |bar]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
      @parser.parse('[[foo     |bar]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
      @parser.parse('[[foo      |bar]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
    end

    it 'should trim whitespace to the right of the separator' do
      @parser.parse('[[foo| bar]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
      @parser.parse('[[foo|  bar]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
      @parser.parse('[[foo|   bar]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
      @parser.parse('[[foo|    bar]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
      @parser.parse('[[foo|     bar]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
      @parser.parse('[[foo|      bar]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
    end

    it 'should trim whitespace on both sides of the separator (at the same time)' do
      @parser.parse('[[foo      | bar]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
      @parser.parse('[[foo     |  bar]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
      @parser.parse('[[foo    |   bar]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
      @parser.parse('[[foo   |    bar]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
      @parser.parse('[[foo  |     bar]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
      @parser.parse('[[foo |      bar]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
    end

    it 'should trim trailing whitespace from the link text' do
      @parser.parse('[[foo|bar ]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
      @parser.parse('[[foo|bar  ]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
      @parser.parse('[[foo|bar   ]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
      @parser.parse('[[foo|bar    ]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
      @parser.parse('[[foo|bar     ]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
      @parser.parse('[[foo|bar      ]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
    end

    it 'should trim leading and trailing whitespace from the link text' do
      @parser.parse('[[foo|      bar ]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
      @parser.parse('[[foo|     bar  ]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
      @parser.parse('[[foo|    bar   ]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
      @parser.parse('[[foo|   bar    ]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
      @parser.parse('[[foo|  bar     ]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
      @parser.parse('[[foo| bar      ]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
    end

    it 'should treat a separator inside the link text as part of the link text' do
      @parser.parse('[[foo|bar|baz]]').should == %Q{<p><a href="/wiki/foo">bar|baz</a></p>\n}
    end

    it 'should treat separators outside of links as normal text' do
      @parser.parse('foo|bar').should == %Q{<p>foo|bar</p>\n}
    end

    it 'should allow em markup in the custom link text' do
      expected = %Q{<p><a href="/wiki/foo">bar <em>baz</em></a></p>\n}
      @parser.parse("[[foo|bar ''baz'']]").should == expected
    end

    it 'should automatically close unclosed em markup in the custom link text' do
      expected = %Q{<p><a href="/wiki/foo">bar <em>baz</em></a></p>\n}
      @parser.parse("[[foo|bar ''baz]]").should == expected
    end

    it 'should allow strong markup in the custom link text' do
      expected = %Q{<p><a href="/wiki/foo">bar <strong>baz</strong></a></p>\n}
      @parser.parse("[[foo|bar '''baz''']]").should == expected
    end

    it 'should automatically close unclosed strong markup in the custom link text' do
      expected = %Q{<p><a href="/wiki/foo">bar <strong>baz</strong></a></p>\n}
      @parser.parse("[[foo|bar '''baz]]").should == expected
    end

    it 'should allow strong/em markup in the custom link text' do
      expected = %Q{<p><a href="/wiki/foo">bar <strong><em>baz</em></strong></a></p>\n}
      @parser.parse("[[foo|bar '''''baz''''']]").should == expected
    end

    it 'should automatically close unclosed strong/em markup in the custom link text' do
      expected = %Q{<p><a href="/wiki/foo">bar <strong><em>baz</em></strong></a></p>\n}
      @parser.parse("[[foo|bar '''''baz]]").should == expected
    end

    it 'should allow tt markup in the custom link text' do
      expected = %Q{<p><a href="/wiki/foo">bar <tt>baz</tt></a></p>\n}
      @parser.parse('[[foo|bar <tt>baz</tt>]]').should == expected
    end

    it 'should automatically close unclosed tt markup in the custom link text' do
      expected = %Q{<p><a href="/wiki/foo">bar <tt>baz</tt></a></p>\n}
      @parser.parse('[[foo|bar <tt>baz]]').should == expected
    end

    it 'should allow named entities in the custom link text' do
      expected = %Q{<p><a href="/wiki/foo">bar &copy;</a></p>\n}
      @parser.parse('[[foo|bar &copy;]]').should == expected

      # explicitly test &quot; because it is tokenized separately from the other named entities
      expected = %Q{<p><a href="/wiki/foo">bar &quot;</a></p>\n}
      @parser.parse('[[foo|bar &quot;]]').should == expected

      # explicitly test &amp; because it is tokenized separately from the other named entities
      expected = %Q{<p><a href="/wiki/foo">bar &amp;</a></p>\n}
      @parser.parse('[[foo|bar &amp;]]').should == expected
    end

    it 'should allow decimal entities in the custom link text' do
      expected = %Q{<p><a href="/wiki/foo">bar &#8364;</a></p>\n}
      @parser.parse('[[foo|bar &#8364;]]').should == expected
    end

    it 'should allow hexadecimal entities in the custom link text' do
      expected = %Q{<p><a href="/wiki/foo">bar &#x20ac;</a></p>\n}
      @parser.parse('[[foo|bar &#x20ac;]]').should == expected
    end

    it 'should sanitize non-ASCII characters in the custom link text' do
      expected = %Q{<p><a href="/wiki/foo">bar &#x20ac;</a></p>\n}
      @parser.parse('[[foo|bar €]]').should == expected
    end

    it 'should sanitize characters that have special meaning in HTML in the custom link text' do
      expected = %Q{<p><a href="/wiki/foo">bar &lt;</a></p>\n}
      @parser.parse('[[foo|bar <]]').should == expected

      expected = %Q{<p><a href="/wiki/foo">bar &gt;</a></p>\n}
      @parser.parse('[[foo|bar >]]').should == expected

      expected = %Q{<p><a href="/wiki/foo">bar &amp;</a></p>\n}
      @parser.parse('[[foo|bar &]]').should == expected

      expected = %Q{<p><a href="/wiki/foo">bar &quot;baz&quot;</a></p>\n}
      @parser.parse('[[foo|bar "baz"]]').should == expected
    end

    it 'should allow nowiki markup in the custom link text' do
      expected = %Q{<p><a href="/wiki/foo">bar [[</a></p>\n}
      @parser.parse("[[foo|bar <nowiki>[[</nowiki>]]").should == expected

      expected = %Q{<p><a href="/wiki/foo">bar [</a></p>\n}
      @parser.parse("[[foo|bar <nowiki>[</nowiki>]]").should == expected

      expected = %Q{<p><a href="/wiki/foo">bar ]]</a></p>\n}
      @parser.parse("[[foo|bar <nowiki>]]</nowiki>]]").should == expected

      expected = %Q{<p><a href="/wiki/foo">bar ]</a></p>\n}
      @parser.parse("[[foo|bar <nowiki>]</nowiki>]]").should == expected
    end
  end

  describe 'overriding the link prefix' do
    it 'should be able to override the link prefix' do
      @parser.internal_link_prefix = '/custom/'
      @parser.parse('[[foo]]').should == %Q{<p><a href="/custom/foo">foo</a></p>\n}
    end

    it 'should interpet a nil link prefix as meaning no prefix' do
      @parser.internal_link_prefix = nil
      @parser.parse('[[foo]]').should == %Q{<p><a href="foo">foo</a></p>\n}
    end
  end

  describe 'special links' do
    it 'should recognize links of the form "bug/10" as special links' do
      @parser.parse('[[bug/10]]').should == %Q{<p><a href="/bug/10">bug/10</a></p>\n}
      @parser.parse('[[issue/25]]').should == %Q{<p><a href="/issue/25">issue/25</a></p>\n}
      @parser.parse('[[post/7]]').should == %Q{<p><a href="/post/7">post/7</a></p>\n}
    end

    it 'should not recognize special links when "treat_slash_as_special" is set to false' do
      @parser.treat_slash_as_special = false
      @parser.parse('[[bug/10]]').should == %Q{<p><a href="/wiki/bug%2f10">bug/10</a></p>\n}
      @parser.parse('[[issue/25]]').should == %Q{<p><a href="/wiki/issue%2f25">issue/25</a></p>\n}
      @parser.parse('[[post/7]]').should == %Q{<p><a href="/wiki/post%2f7">post/7</a></p>\n}
    end

    it 'should accept custom link text in conjunction with special links' do
      @parser.parse('[[bug/10|bug #10]]').should == %Q{<p><a href="/bug/10">bug #10</a></p>\n}
    end

    it 'should ignore link prefix overrides when emitting special links' do
      @parser.internal_link_prefix = '/custom/'
      @parser.parse('[[bug/10]]').should == %Q{<p><a href="/bug/10">bug/10</a></p>\n}
    end

    it 'should not classify links as special merely because of the presence of a slash' do
      # we want the syntax to be tight to minimize false positives
      @parser.parse('[[foo/bar]]').should == %Q{<p><a href="/wiki/foo%2fbar">foo/bar</a></p>\n}
    end

    it 'should not accept special links which have a leading forward slash' do
      # this is a syntax error
      @parser.parse('[[/bug/10]]').should == %Q{<p><a href="/wiki/%2fbug%2f10">/bug/10</a></p>\n}
    end
  end

  describe 'invalid links' do
    it 'should not allow entities in the link text' do
      @parser.parse('[[a &euro; b]]').should == "<p>[[a &euro; b]]</p>\n"
    end

    it 'should not allow URIs in the link text' do
      expected = %Q{<p>[[hello <a href="http://example.com/" class="external">http://example.com/</a> world]]</p>\n}
      @parser.parse('[[hello http://example.com/ world]]').should == expected
    end

    it 'should handle embedded [[ inside links' do
      # note how first part "[[foo " in itself is invalid and so gets rejected and echoed literally
      expected = %Q{<p>[[foo <a href="/wiki/bar">bar</a></p>\n}
      @parser.parse('[[foo [[bar]]').should == expected
    end

    it 'should handled embedded ]] inside links' do
      # note how the link gets terminated early and the trailing part is rejected and echoed literally
      expected = %Q{<p><a href="/wiki/foo">foo</a>bar]]</p>\n}
      @parser.parse('[[foo ]]bar]]').should == expected
    end

    it 'should handle embedded [ inside links' do
      # [ is not allowed at all so the entire link is rendered invalid
      expected = "<p>[[foo [bar]]</p>\n"
      @parser.parse('[[foo [bar]]').should == expected
    end

    it 'should handle embedded ] inside links' do
      # [ is not allowed at all so the entire link is rendered invalid
      expected = "<p>[[foo ]bar]]</p>\n"
      @parser.parse('[[foo ]bar]]').should == expected
    end

    describe 'unterminated link targets (end-of-file)' do
      it 'should rollback and show the unterminated link' do
        @parser.parse('[[foo').should == %Q{<p>[[foo</p>\n}
      end

      it 'should not trim leading whitespace when rolling back' do
        @parser.parse('[[ foo').should    == %Q{<p>[[ foo</p>\n}
        @parser.parse('[[  foo').should   == %Q{<p>[[  foo</p>\n}
        @parser.parse('[[   foo').should  == %Q{<p>[[   foo</p>\n}
        @parser.parse('[[    foo').should == %Q{<p>[[    foo</p>\n}
      end

      it 'should not trim trailing whitespace when rolling back' do
        @parser.parse('[[foo ').should    == %Q{<p>[[foo </p>\n}
        @parser.parse('[[foo  ').should   == %Q{<p>[[foo  </p>\n}
        @parser.parse('[[foo   ').should  == %Q{<p>[[foo   </p>\n}
        @parser.parse('[[foo    ').should == %Q{<p>[[foo    </p>\n}
      end

      it 'should not trim leadig and trailing whitespace (combined) when rolling back' do
        @parser.parse('[[    foo ').should == %Q{<p>[[    foo </p>\n}
        @parser.parse('[[   foo  ').should == %Q{<p>[[   foo  </p>\n}
        @parser.parse('[[  foo   ').should == %Q{<p>[[  foo   </p>\n}
        @parser.parse('[[ foo    ').should == %Q{<p>[[ foo    </p>\n}
      end
    end

    describe 'unterminated link targets (end-of-line)' do
      it 'should rollback and show the unterminated link' do
        @parser.parse("[[foo\n").should == %Q{<p>[[foo</p>\n}
      end

      it 'should not trim leading whitespace when rolling back' do
        @parser.parse("[[ foo\n").should    == %Q{<p>[[ foo</p>\n}
        @parser.parse("[[  foo\n").should   == %Q{<p>[[  foo</p>\n}
        @parser.parse("[[   foo\n").should  == %Q{<p>[[   foo</p>\n}
        @parser.parse("[[    foo\n").should == %Q{<p>[[    foo</p>\n}
      end

      it 'should not trim trailing whitespace when rolling back' do
        @parser.parse("[[foo \n").should    == %Q{<p>[[foo </p>\n}
        @parser.parse("[[foo  \n").should   == %Q{<p>[[foo  </p>\n}
        @parser.parse("[[foo   \n").should  == %Q{<p>[[foo   </p>\n}
        @parser.parse("[[foo    \n").should == %Q{<p>[[foo    </p>\n}
      end

      it 'should not trim leading and trailing whitespace (combined) when rolling back' do
        @parser.parse("[[ foo    \n").should == %Q{<p>[[ foo    </p>\n}
        @parser.parse("[[  foo   \n").should == %Q{<p>[[  foo   </p>\n}
        @parser.parse("[[   foo  \n").should == %Q{<p>[[   foo  </p>\n}
        @parser.parse("[[    foo \n").should == %Q{<p>[[    foo </p>\n}
      end
    end

    describe 'missing link text' do
      it 'should use link target' do
        @parser.parse('[[foo|]]').should == %Q{<p><a href="/wiki/foo">foo</a></p>\n}
      end
    end

    describe 'link cut off at separator (end-of-file)' do
      it 'should rollback and show the unterminated link' do
        @parser.parse('[[foo|').should == %Q{<p>[[foo|</p>\n}
        @parser.parse('[[foo| ').should == %Q{<p>[[foo| </p>\n}
        @parser.parse('[[foo|  ').should == %Q{<p>[[foo|  </p>\n}
        @parser.parse('[[foo|   ').should == %Q{<p>[[foo|   </p>\n}
        @parser.parse('[[foo|    ').should == %Q{<p>[[foo|    </p>\n}
        @parser.parse('[[foo|     ').should == %Q{<p>[[foo|     </p>\n}
        @parser.parse('[[foo|      ').should == %Q{<p>[[foo|      </p>\n}
      end
    end

    describe 'link cut off at separator (end-of-line)' do
      it 'should rollback and show the unterminated link' do
        @parser.parse("[[foo|\n").should == %Q{<p>[[foo|</p>\n}
        @parser.parse("[[foo| \n").should == %Q{<p>[[foo| </p>\n}
        @parser.parse("[[foo|  \n").should == %Q{<p>[[foo|  </p>\n}
        @parser.parse("[[foo|   \n").should == %Q{<p>[[foo|   </p>\n}
        @parser.parse("[[foo|    \n").should == %Q{<p>[[foo|    </p>\n}
        @parser.parse("[[foo|     \n").should == %Q{<p>[[foo|     </p>\n}
        @parser.parse("[[foo|      \n").should == %Q{<p>[[foo|      </p>\n}
      end
    end

    describe 'unterminated link text (end-of-file)' do
      it 'should rollback and show the unterminated link' do
        @parser.parse('[[foo|hello').should == %Q{<p>[[foo|hello</p>\n}
        @parser.parse('[[foo|hello ').should == %Q{<p>[[foo|hello </p>\n}
        @parser.parse('[[foo|hello  ').should == %Q{<p>[[foo|hello  </p>\n}
        @parser.parse('[[foo|hello   ').should == %Q{<p>[[foo|hello   </p>\n}
        @parser.parse('[[foo|hello    ').should == %Q{<p>[[foo|hello    </p>\n}
        @parser.parse('[[foo|hello     ').should == %Q{<p>[[foo|hello     </p>\n}
        @parser.parse('[[foo|hello      ').should == %Q{<p>[[foo|hello      </p>\n}
      end
    end

    describe 'unterminated link text (end-of-line)' do
      it 'should rollback and show the unterminated link' do
        @parser.parse("[[foo|hello\n").should == %Q{<p>[[foo|hello</p>\n}
        @parser.parse("[[foo|hello \n").should == %Q{<p>[[foo|hello </p>\n}
        @parser.parse("[[foo|hello  \n").should == %Q{<p>[[foo|hello  </p>\n}
        @parser.parse("[[foo|hello   \n").should == %Q{<p>[[foo|hello   </p>\n}
        @parser.parse("[[foo|hello    \n").should == %Q{<p>[[foo|hello    </p>\n}
        @parser.parse("[[foo|hello     \n").should == %Q{<p>[[foo|hello     </p>\n}
        @parser.parse("[[foo|hello      \n").should == %Q{<p>[[foo|hello      </p>\n}
      end
    end
  end
end

describe Wikitext::Parser, 'internal links (space to underscore on)' do
  before do
    @parser = Wikitext::Parser.new
    @parser.space_to_underscore = true
  end

  it 'should pass through unexpected link end tokens literally' do
    @parser.parse('foo ]] bar').should == "<p>foo ]] bar</p>\n"                                     # in plain scope
    @parser.parse("foo '']]'' bar").should == "<p>foo <em>]]</em> bar</p>\n"                        # in EM scope
    @parser.parse("foo ''']]''' bar").should == "<p>foo <strong>]]</strong> bar</p>\n"              # in STRONG scope
    @parser.parse("foo ''''']]''''' bar").should == "<p>foo <strong><em>]]</em></strong> bar</p>\n" # in STRONG_EM scope
    @parser.parse('foo <tt>]]</tt> bar').should == "<p>foo <tt>]]</tt> bar</p>\n"                   # in TT scope
    @parser.parse('= foo ]] bar =').should == "<h1>foo ]] bar</h1>\n"                               # in H1 scope
    @parser.parse('== foo ]] bar ==').should == "<h2>foo ]] bar</h2>\n"                             # in H2 scope
    @parser.parse('=== foo ]] bar ===').should == "<h3>foo ]] bar</h3>\n"                           # in H3 scope
    @parser.parse('==== foo ]] bar ====').should == "<h4>foo ]] bar</h4>\n"                         # in H4 scope
    @parser.parse('===== foo ]] bar =====').should == "<h5>foo ]] bar</h5>\n"                       # in H5 scope
    @parser.parse('====== foo ]] bar ======').should == "<h6>foo ]] bar</h6>\n"                     # in H6 scope
    @parser.parse('> ]]').should == "<blockquote>\n  <p>]]</p>\n</blockquote>\n"                    # in BLOCKQUOTE scope
  end

  it 'should turn single words into links' do
    @parser.parse('[[foo]]').should == %Q{<p><a href="/wiki/foo">foo</a></p>\n}
  end

  it 'should turn multiple words into links, converting spaces into underscores' do
    @parser.parse('[[foo bar]]').should == %Q{<p><a href="/wiki/foo_bar">foo bar</a></p>\n}
  end

  it 'should trim leading whitespace' do
    @parser.parse('[[ foo]]').should == %Q{<p><a href="/wiki/foo">foo</a></p>\n}
    @parser.parse('[[  foo]]').should == %Q{<p><a href="/wiki/foo">foo</a></p>\n}
    @parser.parse('[[   foo]]').should == %Q{<p><a href="/wiki/foo">foo</a></p>\n}
    @parser.parse('[[    foo]]').should == %Q{<p><a href="/wiki/foo">foo</a></p>\n}
  end

  it 'should trim trailing whitespace' do
    @parser.parse('[[foo ]]').should == %Q{<p><a href="/wiki/foo">foo</a></p>\n}
    @parser.parse('[[foo  ]]').should == %Q{<p><a href="/wiki/foo">foo</a></p>\n}
    @parser.parse('[[foo   ]]').should == %Q{<p><a href="/wiki/foo">foo</a></p>\n}
    @parser.parse('[[foo    ]]').should == %Q{<p><a href="/wiki/foo">foo</a></p>\n}   # was a bug (exception)
    @parser.parse('[[foo     ]]').should == %Q{<p><a href="/wiki/foo">foo</a></p>\n}  # was a bug (crash)
  end

  it 'should trim leading and trailing whitespace (combined)' do
    @parser.parse('[[ foo    ]]').should == %Q{<p><a href="/wiki/foo">foo</a></p>\n}
    @parser.parse('[[  foo   ]]').should == %Q{<p><a href="/wiki/foo">foo</a></p>\n}
    @parser.parse('[[   foo  ]]').should == %Q{<p><a href="/wiki/foo">foo</a></p>\n}
    @parser.parse('[[    foo ]]').should == %Q{<p><a href="/wiki/foo">foo</a></p>\n}
  end

  it 'should convert embedded whitespace into underscores' do
    @parser.parse('[[ foo bar ]]').should == %Q{<p><a href="/wiki/foo_bar">foo bar</a></p>\n}
    @parser.parse('[[foo bar ]]').should == %Q{<p><a href="/wiki/foo_bar">foo bar</a></p>\n}
    @parser.parse('[[ foo bar ]]').should == %Q{<p><a href="/wiki/foo_bar">foo bar</a></p>\n}
  end

  it 'should encode and sanitize quotes' do
    # note how percent encoding is used in the href, and named entities in the link text
    @parser.parse('[[hello "world"]]').should == %Q{<p><a href="/wiki/hello_%22world%22">hello &quot;world&quot;</a></p>\n}
  end

  it 'should encode and sanitize ampersands' do
    @parser.parse('[[a & b]]').should == %Q{<p><a href="/wiki/a_%26_b">a &amp; b</a></p>\n}
  end

  it 'should allow ampersand entities (special exception)' do
    @parser.parse('[[a &amp; b]]').should == %Q{<p><a href="/wiki/a_%26_b">a &amp; b</a></p>\n}
  end

  it 'should allow quote entities (special exception)' do
    @parser.parse('[[a &quot; b]]').should == %Q{<p><a href="/wiki/a_%22_b">a &quot; b</a></p>\n}
  end

  it 'should handle mixed scenarios (quotes, ampersands, non-ASCII characers)' do
    expected = %Q{<p><a href="/wiki/foo%2c_%22bar%22_%26_baz_%e2%82%ac">foo, &quot;bar&quot; &amp; baz &#x20ac;</a></p>\n}
    @parser.parse('[[foo, "bar" & baz €]]').should == expected
  end

  it 'should handle links in paragraph flows' do
    expected = %Q{<p>foo <a href="/wiki/bar">bar</a> baz</p>\n}
    @parser.parse('foo [[bar]] baz').should == expected # was a bug
  end

  describe 'custom link text' do
    it 'should recognize link text placed after the separator' do
      @parser.parse('[[foo|bar]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
    end

    it 'should trim whitespace to the left of the separator' do
      @parser.parse('[[foo |bar]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
      @parser.parse('[[foo  |bar]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
      @parser.parse('[[foo   |bar]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
      @parser.parse('[[foo    |bar]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
      @parser.parse('[[foo     |bar]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
      @parser.parse('[[foo      |bar]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
    end

    it 'should trim whitespace to the right of the separator' do
      @parser.parse('[[foo| bar]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
      @parser.parse('[[foo|  bar]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
      @parser.parse('[[foo|   bar]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
      @parser.parse('[[foo|    bar]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
      @parser.parse('[[foo|     bar]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
      @parser.parse('[[foo|      bar]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
    end

    it 'should trim whitespace on both sides of the separator (at the same time)' do
      @parser.parse('[[foo      | bar]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
      @parser.parse('[[foo     |  bar]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
      @parser.parse('[[foo    |   bar]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
      @parser.parse('[[foo   |    bar]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
      @parser.parse('[[foo  |     bar]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
      @parser.parse('[[foo |      bar]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
    end

    it 'should trim trailing whitespace from the link text' do
      @parser.parse('[[foo|bar ]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
      @parser.parse('[[foo|bar  ]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
      @parser.parse('[[foo|bar   ]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
      @parser.parse('[[foo|bar    ]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
      @parser.parse('[[foo|bar     ]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
      @parser.parse('[[foo|bar      ]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
    end

    it 'should trim leading and trailing whitespace from the link text' do
      @parser.parse('[[foo|      bar ]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
      @parser.parse('[[foo|     bar  ]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
      @parser.parse('[[foo|    bar   ]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
      @parser.parse('[[foo|   bar    ]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
      @parser.parse('[[foo|  bar     ]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
      @parser.parse('[[foo| bar      ]]').should == %Q{<p><a href="/wiki/foo">bar</a></p>\n}
    end

    it 'should treat a separator inside the link text as part of the link text' do
      @parser.parse('[[foo|bar|baz]]').should == %Q{<p><a href="/wiki/foo">bar|baz</a></p>\n}
    end

    it 'should treat separators outside of links as normal text' do
      @parser.parse('foo|bar').should == %Q{<p>foo|bar</p>\n}
    end

    it 'should allow em markup in the custom link text' do
      expected = %Q{<p><a href="/wiki/foo">bar <em>baz</em></a></p>\n}
      @parser.parse("[[foo|bar ''baz'']]").should == expected
    end

    it 'should automatically close unclosed em markup in the custom link text' do
      expected = %Q{<p><a href="/wiki/foo">bar <em>baz</em></a></p>\n}
      @parser.parse("[[foo|bar ''baz]]").should == expected
    end

    it 'should allow strong markup in the custom link text' do
      expected = %Q{<p><a href="/wiki/foo">bar <strong>baz</strong></a></p>\n}
      @parser.parse("[[foo|bar '''baz''']]").should == expected
    end

    it 'should automatically close unclosed strong markup in the custom link text' do
      expected = %Q{<p><a href="/wiki/foo">bar <strong>baz</strong></a></p>\n}
      @parser.parse("[[foo|bar '''baz]]").should == expected
    end

    it 'should allow strong/em markup in the custom link text' do
      expected = %Q{<p><a href="/wiki/foo">bar <strong><em>baz</em></strong></a></p>\n}
      @parser.parse("[[foo|bar '''''baz''''']]").should == expected
    end

    it 'should automatically close unclosed strong/em markup in the custom link text' do
      expected = %Q{<p><a href="/wiki/foo">bar <strong><em>baz</em></strong></a></p>\n}
      @parser.parse("[[foo|bar '''''baz]]").should == expected
    end

    it 'should allow tt markup in the custom link text' do
      expected = %Q{<p><a href="/wiki/foo">bar <tt>baz</tt></a></p>\n}
      @parser.parse('[[foo|bar <tt>baz</tt>]]').should == expected
    end

    it 'should automatically close unclosed tt markup in the custom link text' do
      expected = %Q{<p><a href="/wiki/foo">bar <tt>baz</tt></a></p>\n}
      @parser.parse('[[foo|bar <tt>baz]]').should == expected
    end

    it 'should allow named entities in the custom link text' do
      expected = %Q{<p><a href="/wiki/foo">bar &copy;</a></p>\n}
      @parser.parse('[[foo|bar &copy;]]').should == expected

      # explicitly test &quot; because it is tokenized separately from the other named entities
      expected = %Q{<p><a href="/wiki/foo">bar &quot;</a></p>\n}
      @parser.parse('[[foo|bar &quot;]]').should == expected

      # explicitly test &amp; because it is tokenized separately from the other named entities
      expected = %Q{<p><a href="/wiki/foo">bar &amp;</a></p>\n}
      @parser.parse('[[foo|bar &amp;]]').should == expected
    end

    it 'should allow decimal entities in the custom link text' do
      expected = %Q{<p><a href="/wiki/foo">bar &#8364;</a></p>\n}
      @parser.parse('[[foo|bar &#8364;]]').should == expected
    end

    it 'should allow hexadecimal entities in the custom link text' do
      expected = %Q{<p><a href="/wiki/foo">bar &#x20ac;</a></p>\n}
      @parser.parse('[[foo|bar &#x20ac;]]').should == expected
    end

    it 'should sanitize non-ASCII characters in the custom link text' do
      expected = %Q{<p><a href="/wiki/foo">bar &#x20ac;</a></p>\n}
      @parser.parse('[[foo|bar €]]').should == expected
    end

    it 'should sanitize characters that have special meaning in HTML in the custom link text' do
      expected = %Q{<p><a href="/wiki/foo">bar &lt;</a></p>\n}
      @parser.parse('[[foo|bar <]]').should == expected

      expected = %Q{<p><a href="/wiki/foo">bar &gt;</a></p>\n}
      @parser.parse('[[foo|bar >]]').should == expected

      expected = %Q{<p><a href="/wiki/foo">bar &amp;</a></p>\n}
      @parser.parse('[[foo|bar &]]').should == expected

      expected = %Q{<p><a href="/wiki/foo">bar &quot;baz&quot;</a></p>\n}
      @parser.parse('[[foo|bar "baz"]]').should == expected
    end

    it 'should allow nowiki markup in the custom link text' do
      expected = %Q{<p><a href="/wiki/foo">bar [[</a></p>\n}
      @parser.parse("[[foo|bar <nowiki>[[</nowiki>]]").should == expected

      expected = %Q{<p><a href="/wiki/foo">bar [</a></p>\n}
      @parser.parse("[[foo|bar <nowiki>[</nowiki>]]").should == expected

      expected = %Q{<p><a href="/wiki/foo">bar ]]</a></p>\n}
      @parser.parse("[[foo|bar <nowiki>]]</nowiki>]]").should == expected

      expected = %Q{<p><a href="/wiki/foo">bar ]</a></p>\n}
      @parser.parse("[[foo|bar <nowiki>]</nowiki>]]").should == expected
    end
  end

  describe 'overriding the link prefix' do
    it 'should be able to override the link prefix' do
      @parser.internal_link_prefix = '/custom/'
      @parser.parse('[[foo]]').should == %Q{<p><a href="/custom/foo">foo</a></p>\n}
    end

    it 'should interpet a nil link prefix as meaning no prefix' do
      @parser.internal_link_prefix = nil
      @parser.parse('[[foo]]').should == %Q{<p><a href="foo">foo</a></p>\n}
    end
  end

  describe 'special links' do
    it 'should recognize links of the form "bug/10" as special links' do
      @parser.parse('[[bug/10]]').should == %Q{<p><a href="/bug/10">bug/10</a></p>\n}
      @parser.parse('[[issue/25]]').should == %Q{<p><a href="/issue/25">issue/25</a></p>\n}
      @parser.parse('[[post/7]]').should == %Q{<p><a href="/post/7">post/7</a></p>\n}
    end

    it 'should not recognize special links when "treat_slash_as_special" is set to false' do
      @parser.treat_slash_as_special = false
      @parser.parse('[[bug/10]]').should == %Q{<p><a href="/wiki/bug%2f10">bug/10</a></p>\n}
      @parser.parse('[[issue/25]]').should == %Q{<p><a href="/wiki/issue%2f25">issue/25</a></p>\n}
      @parser.parse('[[post/7]]').should == %Q{<p><a href="/wiki/post%2f7">post/7</a></p>\n}
    end

    it 'should accept custom link text in conjunction with special links' do
      @parser.parse('[[bug/10|bug #10]]').should == %Q{<p><a href="/bug/10">bug #10</a></p>\n}
    end

    it 'should ignore link prefix overrides when emitting special links' do
      @parser.internal_link_prefix = '/custom/'
      @parser.parse('[[bug/10]]').should == %Q{<p><a href="/bug/10">bug/10</a></p>\n}
    end

    it 'should not classify links as special merely because of the presence of a slash' do
      # we want the syntax to be tight to minimize false positives
      @parser.parse('[[foo/bar]]').should == %Q{<p><a href="/wiki/foo%2fbar">foo/bar</a></p>\n}
    end

    it 'should not accept special links which have a leading forward slash' do
      # this is a syntax error
      @parser.parse('[[/bug/10]]').should == %Q{<p><a href="/wiki/%2fbug%2f10">/bug/10</a></p>\n}
    end
  end

  describe 'invalid links' do
    it 'should not allow entities in the link text' do
      @parser.parse('[[a &euro; b]]').should == "<p>[[a &euro; b]]</p>\n"
    end

    it 'should not allow URIs in the link text' do
      expected = %Q{<p>[[hello <a href="http://example.com/" class="external">http://example.com/</a> world]]</p>\n}
      @parser.parse('[[hello http://example.com/ world]]').should == expected
    end

    it 'should handle embedded [[ inside links' do
      # note how first part "[[foo " in itself is invalid and so gets rejected and echoed literally
      expected = %Q{<p>[[foo <a href="/wiki/bar">bar</a></p>\n}
      @parser.parse('[[foo [[bar]]').should == expected
    end

    it 'should handled embedded ]] inside links' do
      # note how the link gets terminated early and the trailing part is rejected and echoed literally
      expected = %Q{<p><a href="/wiki/foo">foo</a>bar]]</p>\n}
      @parser.parse('[[foo ]]bar]]').should == expected
    end

    it 'should handle embedded [ inside links' do
      # [ is not allowed at all so the entire link is rendered invalid
      expected = "<p>[[foo [bar]]</p>\n"
      @parser.parse('[[foo [bar]]').should == expected
    end

    it 'should handle embedded ] inside links' do
      # [ is not allowed at all so the entire link is rendered invalid
      expected = "<p>[[foo ]bar]]</p>\n"
      @parser.parse('[[foo ]bar]]').should == expected
    end

    describe 'unterminated link targets (end-of-file)' do
      it 'should rollback and show the unterminated link' do
        @parser.parse('[[foo').should == %Q{<p>[[foo</p>\n}
      end

      it 'should not trim leading whitespace when rolling back' do
        @parser.parse('[[ foo').should    == %Q{<p>[[ foo</p>\n}
        @parser.parse('[[  foo').should   == %Q{<p>[[  foo</p>\n}
        @parser.parse('[[   foo').should  == %Q{<p>[[   foo</p>\n}
        @parser.parse('[[    foo').should == %Q{<p>[[    foo</p>\n}
      end

      it 'should not trim trailing whitespace when rolling back' do
        @parser.parse('[[foo ').should    == %Q{<p>[[foo </p>\n}
        @parser.parse('[[foo  ').should   == %Q{<p>[[foo  </p>\n}
        @parser.parse('[[foo   ').should  == %Q{<p>[[foo   </p>\n}
        @parser.parse('[[foo    ').should == %Q{<p>[[foo    </p>\n}
      end

      it 'should not trim leadig and trailing whitespace (combined) when rolling back' do
        @parser.parse('[[    foo ').should == %Q{<p>[[    foo </p>\n}
        @parser.parse('[[   foo  ').should == %Q{<p>[[   foo  </p>\n}
        @parser.parse('[[  foo   ').should == %Q{<p>[[  foo   </p>\n}
        @parser.parse('[[ foo    ').should == %Q{<p>[[ foo    </p>\n}
      end
    end

    describe 'unterminated link targets (end-of-line)' do
      it 'should rollback and show the unterminated link' do
        @parser.parse("[[foo\n").should == %Q{<p>[[foo</p>\n}
      end

      it 'should not trim leading whitespace when rolling back' do
        @parser.parse("[[ foo\n").should    == %Q{<p>[[ foo</p>\n}
        @parser.parse("[[  foo\n").should   == %Q{<p>[[  foo</p>\n}
        @parser.parse("[[   foo\n").should  == %Q{<p>[[   foo</p>\n}
        @parser.parse("[[    foo\n").should == %Q{<p>[[    foo</p>\n}
      end

      it 'should not trim trailing whitespace when rolling back' do
        @parser.parse("[[foo \n").should    == %Q{<p>[[foo </p>\n}
        @parser.parse("[[foo  \n").should   == %Q{<p>[[foo  </p>\n}
        @parser.parse("[[foo   \n").should  == %Q{<p>[[foo   </p>\n}
        @parser.parse("[[foo    \n").should == %Q{<p>[[foo    </p>\n}
      end

      it 'should not trim leading and trailing whitespace (combined) when rolling back' do
        @parser.parse("[[ foo    \n").should == %Q{<p>[[ foo    </p>\n}
        @parser.parse("[[  foo   \n").should == %Q{<p>[[  foo   </p>\n}
        @parser.parse("[[   foo  \n").should == %Q{<p>[[   foo  </p>\n}
        @parser.parse("[[    foo \n").should == %Q{<p>[[    foo </p>\n}
      end
    end

    describe 'missing link text' do
      it 'should use link target' do
        @parser.parse('[[foo|]]').should == %Q{<p><a href="/wiki/foo">foo</a></p>\n}
      end
    end

    describe 'link cut off at separator (end-of-file)' do
      it 'should rollback and show the unterminated link' do
        @parser.parse('[[foo|').should == %Q{<p>[[foo|</p>\n}
        @parser.parse('[[foo| ').should == %Q{<p>[[foo| </p>\n}
        @parser.parse('[[foo|  ').should == %Q{<p>[[foo|  </p>\n}
        @parser.parse('[[foo|   ').should == %Q{<p>[[foo|   </p>\n}
        @parser.parse('[[foo|    ').should == %Q{<p>[[foo|    </p>\n}
        @parser.parse('[[foo|     ').should == %Q{<p>[[foo|     </p>\n}
        @parser.parse('[[foo|      ').should == %Q{<p>[[foo|      </p>\n}
      end
    end

    describe 'link cut off at separator (end-of-line)' do
      it 'should rollback and show the unterminated link' do
        @parser.parse("[[foo|\n").should == %Q{<p>[[foo|</p>\n}
        @parser.parse("[[foo| \n").should == %Q{<p>[[foo| </p>\n}
        @parser.parse("[[foo|  \n").should == %Q{<p>[[foo|  </p>\n}
        @parser.parse("[[foo|   \n").should == %Q{<p>[[foo|   </p>\n}
        @parser.parse("[[foo|    \n").should == %Q{<p>[[foo|    </p>\n}
        @parser.parse("[[foo|     \n").should == %Q{<p>[[foo|     </p>\n}
        @parser.parse("[[foo|      \n").should == %Q{<p>[[foo|      </p>\n}
      end
    end

    describe 'unterminated link text (end-of-file)' do
      it 'should rollback and show the unterminated link' do
        @parser.parse('[[foo|hello').should == %Q{<p>[[foo|hello</p>\n}
        @parser.parse('[[foo|hello ').should == %Q{<p>[[foo|hello </p>\n}
        @parser.parse('[[foo|hello  ').should == %Q{<p>[[foo|hello  </p>\n}
        @parser.parse('[[foo|hello   ').should == %Q{<p>[[foo|hello   </p>\n}
        @parser.parse('[[foo|hello    ').should == %Q{<p>[[foo|hello    </p>\n}
        @parser.parse('[[foo|hello     ').should == %Q{<p>[[foo|hello     </p>\n}
        @parser.parse('[[foo|hello      ').should == %Q{<p>[[foo|hello      </p>\n}
      end
    end

    describe 'unterminated link text (end-of-line)' do
      it 'should rollback and show the unterminated link' do
        @parser.parse("[[foo|hello\n").should == %Q{<p>[[foo|hello</p>\n}
        @parser.parse("[[foo|hello \n").should == %Q{<p>[[foo|hello </p>\n}
        @parser.parse("[[foo|hello  \n").should == %Q{<p>[[foo|hello  </p>\n}
        @parser.parse("[[foo|hello   \n").should == %Q{<p>[[foo|hello   </p>\n}
        @parser.parse("[[foo|hello    \n").should == %Q{<p>[[foo|hello    </p>\n}
        @parser.parse("[[foo|hello     \n").should == %Q{<p>[[foo|hello     </p>\n}
        @parser.parse("[[foo|hello      \n").should == %Q{<p>[[foo|hello      </p>\n}
      end
    end
  end
end
