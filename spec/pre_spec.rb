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

describe Wikitext::Parser, 'parsing PRE blocks' do
  before do
    @parser = Wikitext::Parser.new
  end

  it 'should recognize a single-line <pre> block' do
    @parser.parse(' foo').should == "<pre>foo</pre>\n"
  end

  it 'should recognize a multiline <pre> block' do
    @parser.parse(" foo\n bar").should == "<pre>foo\nbar</pre>\n"
  end

  it 'should allow nesting inside a <blockquote> block' do
    # nesting inside single blockquotes
    @parser.parse(">  foo").should == "<blockquote>\n  <pre>foo</pre>\n</blockquote>\n"

    # same, but continued over multiple lines
    @parser.parse(">  foo\n>  bar").should == "<blockquote>\n  <pre>foo\nbar</pre>\n</blockquote>\n"

    # nesting inside double blockquotes
    expected = dedent 6, <<-END
      <blockquote>
        <blockquote>
          <pre>foo</pre>
        </blockquote>
      </blockquote>
    END
    @parser.parse("> >  foo").should == expected

    # same, but continued over multiple lines
    expected = dedent 6, <<-END
      <blockquote>
        <blockquote>
          <pre>foo
      bar</pre>
        </blockquote>
      </blockquote>
    END
    @parser.parse("> >  foo\n> >  bar").should == expected
  end

  it 'should automatically close preceding blocks at the same depth' do
    @parser.parse("> foo\n bar").should == "<blockquote>\n  <p>foo</p>\n</blockquote>\n<pre>bar</pre>\n"
    expected = dedent 6, <<-END
      <blockquote>
        <blockquote>
          <p>foo</p>
        </blockquote>
      </blockquote>
      <pre>bar</pre>
    END
    @parser.parse("> > foo\n bar").should == expected
  end

  it 'should pass <tt> and </tt> tags through without any special meaning' do
    @parser.parse(' foo <tt>bar</tt>').should == "<pre>foo &lt;tt&gt;bar&lt;/tt&gt;</pre>\n"
  end

  it 'should pass <em> and </em> tags through without any special meaning' do
    @parser.parse(" foo ''bar''").should == "<pre>foo ''bar''</pre>\n"
  end

  it 'should pass <strong> and </strong> tags through without any special meaning' do
    @parser.parse(" foo '''bar'''").should == "<pre>foo '''bar'''</pre>\n"
  end

  it 'should pass combined <strong>/<em> and </strong>/</em> tags through without any special meaning' do
    @parser.parse(" foo '''''bar'''''").should == "<pre>foo '''''bar'''''</pre>\n"
  end

  it 'should pass named entities through unchanged' do
    @parser.parse(' &euro;').should == "<pre>&euro;</pre>\n"
  end

  it 'should pass numeric (decimal) entities through unchanged' do
    @parser.parse(' &#8364;').should == "<pre>&#8364;</pre>\n"
  end

  it 'should pass numeric (hexadecimal) entities through unchanged' do
    @parser.parse(' &#x20ac;').should == "<pre>&#x20ac;</pre>\n"
    @parser.parse(' &#X20aC;').should == "<pre>&#x20ac;</pre>\n"
  end

  it 'should convert non-ASCII characters to numeric entities' do
    @parser.parse(' €').should == "<pre>&#x20ac;</pre>\n"
  end
end

describe Wikitext::Parser, 'parsing PRE_START/PRE_END blocks' do
  before do
    @parser = Wikitext::Parser.new
  end

  it 'should accept PRE_START/PRE_END as an alternative to the standard syntax' do
    @parser.parse('<pre>foo</pre>').should == "<pre>foo</pre>\n"
  end

  it 'should pass through PRE unchanged in PRE_START/PRE_END blocks' do
    input = dedent 6, <<-END
      <pre>line 1
       next line</pre>
    END
    expected = dedent 6, <<-END
      <pre>line 1
       next line</pre>
    END
    @parser.parse(input).should == expected
  end

  it 'should pass through short BLOCKQUOTE tokens as named entities in PRE_START/PRE_END blocks' do
    input = dedent 6, <<-END
      <pre>line 1
      >next line</pre>
    END
    expected = dedent 6, <<-END
      <pre>line 1
      &gt;next line</pre>
    END
    @parser.parse(input).should == expected
  end

  it 'should pass through long BLOCKQUOTE tokens as named entities in PRE_START/PRE_END blocks' do
    input = dedent 6, <<-END
      <pre>line 1
      > next line</pre>
    END
    expected = dedent 6, <<-END
      <pre>line 1
      &gt; next line</pre>
    END
    @parser.parse(input).should == expected
  end

  it 'should pass through EM unchanged in PRE_START/PRE_END blocks' do
    @parser.parse("<pre>''</pre>").should == "<pre>''</pre>\n"
  end

  it 'should pass through STRONG unchanged in PRE_START/PRE_END blocks' do
    @parser.parse("<pre>'''</pre>").should == "<pre>'''</pre>\n"
  end

  it 'should pass through STRONG_EM unchanged in PRE_START/PRE_END blocks' do
    @parser.parse("<pre>'''''</pre>").should == "<pre>'''''</pre>\n"
  end

  it 'should pass through EM_START escaped in PRE_START/PRE_END blocks' do
    @parser.parse("<pre><em></pre>").should == "<pre>&lt;em&gt;</pre>\n"
  end

  it 'should pass through EM_END escaped in PRE_START/PRE_END blocks' do
    @parser.parse("<pre></em></pre>").should == "<pre>&lt;/em&gt;</pre>\n"
  end

  it 'should pass through STRONG_START escaped in PRE_START/PRE_END blocks' do
    @parser.parse("<pre><strong></pre>").should == "<pre>&lt;strong&gt;</pre>\n"
  end

  it 'should pass through STRONG_END escaped in PRE_START/PRE_END blocks' do
    @parser.parse("<pre></strong></pre>").should == "<pre>&lt;/strong&gt;</pre>\n"
  end

  it 'should pass through TT unchanged in PRE_START/PRE_END blocks' do
    @parser.parse("<pre>`</pre>").should == "<pre>`</pre>\n"
  end

  it 'should pass through TT_START escaped in PRE_START/PRE_END blocks' do
    @parser.parse("<pre><tt></pre>").should == "<pre>&lt;tt&gt;</pre>\n"
  end

  it 'should pass through TT_END escaped in PRE_START/PRE_END blocks' do
    @parser.parse("<pre></tt></pre>").should == "<pre>&lt;/tt&gt;</pre>\n"
  end

  it 'should pass through UL unchanged in PRE_START/PRE_END blocks' do
    @parser.parse("<pre>\n#</pre>").should == "<pre>\n#</pre>\n"
  end

  it 'should pass through OL unchanged in PRE_START/PRE_END blocks' do
    @parser.parse("<pre>\n*</pre>").should == "<pre>\n*</pre>\n"
  end

  it 'should ignore PRE_START inside <nowiki> spans' do
    @parser.parse('<nowiki><pre></nowiki>').should == "<p>&lt;pre&gt;</p>\n"
  end

  it 'should ignore PRE_END inside <nowiki> spans' do
    @parser.parse('<nowiki></pre></nowiki>').should == "<p>&lt;/pre&gt;</p>\n"
  end

  it 'should ignore PRE_START inside standard PRE blocks' do
    @parser.parse(' <pre>').should == "<pre>&lt;pre&gt;</pre>\n"
  end

  it 'should ignore PRE_END inside standard PRE blocks' do
    @parser.parse(' </pre>').should == "<pre>&lt;/pre&gt;</pre>\n"
  end

  it 'should ignore PRE_START inside already open PRE_START blocks' do
    @parser.parse('<pre><pre></pre>').should == "<pre>&lt;pre&gt;</pre>\n"
  end

  it 'should ignore PRE_START inside BLOCKQUOTE blocks' do
    expected = dedent 6, <<-END
      <blockquote>
        <p>&lt;pre&gt;</p>
      </blockquote>
    END
    @parser.parse('> <pre>').should == expected
  end

  it 'should ignore PRE_END inside BLOCKQUOTE blocks' do
    expected = dedent 6, <<-END
      <blockquote>
        <p>&lt;/pre&gt;</p>
      </blockquote>
    END
    @parser.parse('> </pre>').should == expected
  end

  it 'should ignore PRE_START inside UL blocks' do
    expected = dedent 6, <<-END
      <ul>
        <li>&lt;pre&gt;</li>
      </ul>
    END
    @parser.parse('* <pre>').should == expected
  end

  it 'should ignore PRE_END inside UL blocks' do
    expected = dedent 6, <<-END
      <ul>
        <li>&lt;/pre&gt;</li>
      </ul>
    END
    @parser.parse('* </pre>').should == expected
  end

  it 'should ignore PRE_START inside OL blocks' do
    expected = dedent 6, <<-END
      <ol>
        <li>&lt;pre&gt;</li>
      </ol>
    END
    @parser.parse('# <pre>').should == expected
  end

  it 'should ignore PRE_END inside OL blocks' do
    expected = dedent 6, <<-END
      <ol>
        <li>&lt;/pre&gt;</li>
      </ol>
    END
    @parser.parse('# </pre>').should == expected
  end

  it 'should ignore PRE_START inside H1 blocks' do
    @parser.parse('= <pre> =').should == "<h1>&lt;pre&gt;</h1>\n"
  end

  it 'should ignore PRE_END inside H1 blocks' do
    @parser.parse('= </pre> =').should == "<h1>&lt;/pre&gt;</h1>\n"
  end

  it 'should ignore PRE_START inside H2 blocks' do
    @parser.parse('== <pre> ==').should == "<h2>&lt;pre&gt;</h2>\n"
  end

  it 'should ignore PRE_END inside H2 blocks' do
    @parser.parse('== </pre> ==').should == "<h2>&lt;/pre&gt;</h2>\n"
  end

  it 'should ignore PRE_START inside H3 blocks' do
    @parser.parse('=== <pre> ===').should == "<h3>&lt;pre&gt;</h3>\n"
  end

  it 'should ignore PRE_END inside H3 blocks' do
    @parser.parse('=== </pre> ===').should == "<h3>&lt;/pre&gt;</h3>\n"
  end

  it 'should ignore PRE_START inside H4 blocks' do
    @parser.parse('==== <pre> ====').should == "<h4>&lt;pre&gt;</h4>\n"
  end

  it 'should ignore PRE_END inside H4 blocks' do
    @parser.parse('==== </pre> ====').should == "<h4>&lt;/pre&gt;</h4>\n"
  end

  it 'should ignore PRE_START inside H5 blocks' do
    @parser.parse('===== <pre> =====').should == "<h5>&lt;pre&gt;</h5>\n"
  end

  it 'should ignore PRE_END inside H5 blocks' do
    @parser.parse('===== </pre> =====').should == "<h5>&lt;/pre&gt;</h5>\n"
  end

  it 'should ignore PRE_START inside H6 blocks' do
    @parser.parse('====== <pre> ======').should == "<h6>&lt;pre&gt;</h6>\n"
  end

  it 'should ignore PRE_END inside H6 blocks' do
    @parser.parse('====== </pre> ======').should == "<h6>&lt;/pre&gt;</h6>\n"
  end

  it 'should start a <pre> block on seeing PRE_START partway through a P block' do
    # the trailing space after "hello" is preserved just like it would be if the input were "hello " and nothing else
    expected = dedent 6, <<-END
      <p>hello </p>
      <pre>world</pre>
    END
    @parser.parse('hello <pre>world</pre>').should == expected
  end

  it 'should close any open spans while starting a <pre> block on seeing PRE_START partway through a P block' do
    # ''
    expected = dedent 6, <<-END
      <p>hello <em>my </em></p>
      <pre>world</pre>
    END
    @parser.parse("hello ''my <pre>world</pre>").should == expected

    # '''
    expected = dedent 6, <<-END
      <p>hello <strong>my </strong></p>
      <pre>world</pre>
    END
    @parser.parse("hello '''my <pre>world</pre>").should == expected

    # '''''
    expected = dedent 6, <<-END
      <p>hello <strong><em>my </em></strong></p>
      <pre>world</pre>
    END
    @parser.parse("hello '''''my <pre>world</pre>").should == expected

    # `
    expected = dedent 6, <<-END
      <p>hello <tt>my </tt></p>
      <pre>world</pre>
    END
    @parser.parse("hello `my <pre>world</pre>").should == expected

    # <em>
    expected = dedent 6, <<-END
      <p>hello <em>my </em></p>
      <pre>world</pre>
    END
    @parser.parse("hello <em>my <pre>world</pre>").should == expected

    # <strong>
    expected = dedent 6, <<-END
      <p>hello <strong>my </strong></p>
      <pre>world</pre>
    END
    @parser.parse("hello <strong>my <pre>world</pre>").should == expected

    # <strong><em>
    expected = dedent 6, <<-END
      <p>hello <strong><em>my </em></strong></p>
      <pre>world</pre>
    END
    @parser.parse("hello <strong><em>my <pre>world</pre>").should == expected

    # <tt>
    expected = dedent 6, <<-END
      <p>hello <tt>my </tt></p>
      <pre>world</pre>
    END
    @parser.parse("hello <tt>my <pre>world</pre>").should == expected
  end

  it 'should rollback open internal link spans on encountering a PRE_START in the link target' do
    expected = dedent 6, <<-END
      <p>[[hello </p>
      <pre>world</pre>
      <p>]]</p>
    END
    @parser.parse('[[hello <pre>world</pre>]]').should == expected
  end

  it 'should rollback open internal link spans on encountering a PRE_START in the link text' do
    expected = dedent 6, <<-END
      <p>[[hello | there</p>
      <pre>world</pre>
      <p>]]</p>
    END
    @parser.parse('[[hello | there<pre>world</pre>]]').should == expected
  end

  it 'should automatically close open PRE_START blocks on hitting the end-of-file' do
    @parser.parse('<pre>foo').should == "<pre>foo</pre>\n"
  end
end
