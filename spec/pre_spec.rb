# encoding: utf-8
# Copyright 2007-2010 Wincent Colaiuta. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.

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

  it 'should handle "empty" lines in the middle of multiline PRE blocks' do
    input = dedent <<-END
       foo
       
       bar
    END
    expected = dedent <<-END
      <pre>foo
      
      bar</pre>
    END
    @parser.parse(input).should == expected
  end

  it 'should render an empty block for an empty PRE block' do
    @parser.parse(' ').should == "<pre></pre>\n"
  end

  it 'should sanely handle a leading empty line' do
    @parser.parse(" \n foo").should == "<pre>\nfoo</pre>\n"
  end

  it 'should sanely handle a trailing empty line' do
    @parser.parse(" foo\n \n").should == "<pre>foo\n</pre>\n"
    @parser.parse(" foo\n ").should == "<pre>foo\n</pre>\n"
  end

  it 'should allow nesting inside a <blockquote> block' do
    # nesting inside single blockquotes
    @parser.parse(">  foo").should == "<blockquote>\n  <pre>foo</pre>\n</blockquote>\n"

    # same, but continued over multiple lines
    @parser.parse(">  foo\n>  bar").should == "<blockquote>\n  <pre>foo\nbar</pre>\n</blockquote>\n"

    # nesting inside double blockquotes
    expected = dedent <<-END
      <blockquote>
        <blockquote>
          <pre>foo</pre>
        </blockquote>
      </blockquote>
    END
    @parser.parse("> >  foo").should == expected

    # same, but continued over multiple lines
    expected = dedent <<-END
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
    expected = dedent <<-END
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
    input = dedent <<-END
      <pre>line 1
       next line</pre>
    END
    expected = dedent <<-END
      <pre>line 1
       next line</pre>
    END
    @parser.parse(input).should == expected
  end

  it 'should pass through short BLOCKQUOTE tokens as named entities in PRE_START/PRE_END blocks' do
    input = dedent <<-END
      <pre>line 1
      >next line</pre>
    END
    expected = dedent <<-END
      <pre>line 1
      &gt;next line</pre>
    END
    @parser.parse(input).should == expected
  end

  it 'should pass through long BLOCKQUOTE tokens as named entities in PRE_START/PRE_END blocks' do
    input = dedent <<-END
      <pre>line 1
      > next line</pre>
    END
    expected = dedent <<-END
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
    expected = dedent <<-END
      <blockquote>
        <p>&lt;pre&gt;</p>
      </blockquote>
    END
    @parser.parse('> <pre>').should == expected
  end

  it 'should ignore PRE_END inside BLOCKQUOTE blocks' do
    expected = dedent <<-END
      <blockquote>
        <p>&lt;/pre&gt;</p>
      </blockquote>
    END
    @parser.parse('> </pre>').should == expected
  end

  it 'should break out of UL blocks on seeing PRE_START' do
    expected = dedent <<-END
      <ul>
        <li>foo</li>
      </ul>
      <pre>bar</pre>
    END
    @parser.parse('* foo<pre>bar</pre>').should == expected
  end

  it 'should ignore PRE_END inside UL blocks' do
    expected = dedent <<-END
      <ul>
        <li>&lt;/pre&gt;</li>
      </ul>
    END
    @parser.parse('* </pre>').should == expected
  end

  it 'should break out of OL blocks on seeing PRE_START' do
    expected = dedent <<-END
      <ol>
        <li>foo</li>
      </ol>
      <pre>bar</pre>
    END
    @parser.parse('# foo<pre>bar</pre>').should == expected
  end

  it 'should ignore PRE_END inside OL blocks' do
    expected = dedent <<-END
      <ol>
        <li>&lt;/pre&gt;</li>
      </ol>
    END
    @parser.parse('# </pre>').should == expected
  end

  it 'should break out of H1 blocks on seeing PRE_START' do
    expected = dedent <<-END
      <h1>foo</h1>
      <pre>bar</pre>
      <p> =</p>
    END
    @parser.parse('= foo<pre>bar</pre> =').should == expected
  end

  it 'should ignore PRE_END inside H1 blocks' do
    @parser.parse('= </pre> =').should == "<h1>&lt;/pre&gt;</h1>\n"
  end

  it 'should break out of H2 blocks on seeing PRE_START' do
    expected = dedent <<-END
      <h2>foo</h2>
      <pre>bar</pre>
      <p> ==</p>
    END
    @parser.parse('== foo<pre>bar</pre> ==').should == expected
  end

  it 'should ignore PRE_END inside H2 blocks' do
    @parser.parse('== </pre> ==').should == "<h2>&lt;/pre&gt;</h2>\n"
  end

  it 'should break out of H3 blocks on seeing PRE_START' do
    expected = dedent <<-END
      <h3>foo</h3>
      <pre>bar</pre>
      <p> ===</p>
    END
    @parser.parse('=== foo<pre>bar</pre> ===').should == expected
  end

  it 'should ignore PRE_END inside H3 blocks' do
    @parser.parse('=== </pre> ===').should == "<h3>&lt;/pre&gt;</h3>\n"
  end

  it 'should break out of H4 blocks on seeing PRE_START' do
    expected = dedent <<-END
      <h4>foo</h4>
      <pre>bar</pre>
      <p> ====</p>
    END
    @parser.parse('==== foo<pre>bar</pre> ====').should == expected
  end

  it 'should ignore PRE_END inside H4 blocks' do
    @parser.parse('==== </pre> ====').should == "<h4>&lt;/pre&gt;</h4>\n"
  end

  it 'should break out of H5 blocks on seeing PRE_START' do
    expected = dedent <<-END
      <h5>foo</h5>
      <pre>bar</pre>
      <p> =====</p>
    END
    @parser.parse('===== foo<pre>bar</pre> =====').should == expected
  end

  it 'should ignore PRE_END inside H5 blocks' do
    @parser.parse('===== </pre> =====').should == "<h5>&lt;/pre&gt;</h5>\n"
  end

  it 'should break out of H6 blocks on seeing PRE_START' do
    expected = dedent <<-END
      <h6>foo</h6>
      <pre>bar</pre>
      <p> ======</p>
    END
    @parser.parse('====== foo<pre>bar</pre> ======').should == expected
  end

  it 'should ignore PRE_END inside H6 blocks' do
    @parser.parse('====== </pre> ======').should == "<h6>&lt;/pre&gt;</h6>\n"
  end

  it 'should start a <pre> block on seeing PRE_START partway through a P block' do
    # the trailing space after "hello" is preserved just like it would be if the input were "hello " and nothing else
    expected = dedent <<-END
      <p>hello </p>
      <pre>world</pre>
    END
    @parser.parse('hello <pre>world</pre>').should == expected
  end

  it 'should close any open spans while starting a <pre> block on seeing PRE_START partway through a P block' do
    # ''
    expected = dedent <<-END
      <p>hello <em>my </em></p>
      <pre>world</pre>
    END
    @parser.parse("hello ''my <pre>world</pre>").should == expected

    # '''
    expected = dedent <<-END
      <p>hello <strong>my </strong></p>
      <pre>world</pre>
    END
    @parser.parse("hello '''my <pre>world</pre>").should == expected

    # '''''
    expected = dedent <<-END
      <p>hello <strong><em>my </em></strong></p>
      <pre>world</pre>
    END
    @parser.parse("hello '''''my <pre>world</pre>").should == expected

    # `
    expected = dedent <<-END
      <p>hello <code>my </code></p>
      <pre>world</pre>
    END
    @parser.parse("hello `my <pre>world</pre>").should == expected

    # <em>
    expected = dedent <<-END
      <p>hello <em>my </em></p>
      <pre>world</pre>
    END
    @parser.parse("hello <em>my <pre>world</pre>").should == expected

    # <strong>
    expected = dedent <<-END
      <p>hello <strong>my </strong></p>
      <pre>world</pre>
    END
    @parser.parse("hello <strong>my <pre>world</pre>").should == expected

    # <strong><em>
    expected = dedent <<-END
      <p>hello <strong><em>my </em></strong></p>
      <pre>world</pre>
    END
    @parser.parse("hello <strong><em>my <pre>world</pre>").should == expected

    # <tt>
    expected = dedent <<-END
      <p>hello <code>my </code></p>
      <pre>world</pre>
    END
    @parser.parse("hello <tt>my <pre>world</pre>").should == expected
  end

  it 'should rollback open internal link spans on encountering a PRE_START in the link target' do
    expected = dedent <<-END
      <p>[[hello </p>
      <pre>world</pre>
      <p>]]</p>
    END
    @parser.parse('[[hello <pre>world</pre>]]').should == expected
  end

  it 'should rollback open internal link spans on encountering a PRE_START in the link text' do
    expected = dedent <<-END
      <p>[[hello | there</p>
      <pre>world</pre>
      <p>]]</p>
    END
    @parser.parse('[[hello | there<pre>world</pre>]]').should == expected
  end

  it 'should automatically close open PRE_START blocks on hitting the end-of-file' do
    @parser.parse('<pre>foo').should == "<pre>foo</pre>\n"
  end

  it 'should handle an optional "lang" attribute' do
    @parser.parse('<pre lang="ruby">foo</pre>').should == %Q{<pre class="ruby-syntax">foo</pre>\n}
  end

  it 'should reject excess internal whitespace in PRE_START tags which have a "lang" attribute' do
    @parser.parse('<pre  lang="ruby">foo</pre>').should == %Q{<p>&lt;pre  lang=&quot;ruby&quot;&gt;foo&lt;/pre&gt;</p>\n}
    @parser.parse('<pre lang ="ruby">foo</pre>').should == %Q{<p>&lt;pre lang =&quot;ruby&quot;&gt;foo&lt;/pre&gt;</p>\n}
    @parser.parse('<pre lang= "ruby">foo</pre>').should == %Q{<p>&lt;pre lang= &quot;ruby&quot;&gt;foo&lt;/pre&gt;</p>\n}
    @parser.parse('<pre lang="ruby" >foo</pre>').should == %Q{<p>&lt;pre lang=&quot;ruby&quot; &gt;foo&lt;/pre&gt;</p>\n}
  end

  it 'should reject non-alpha characters in "lang" attribute' do
    @parser.parse('<pre lang="obj-c">foo</pre>').should == %Q{<p>&lt;pre lang=&quot;obj-c&quot;&gt;foo&lt;/pre&gt;</p>\n}
  end

  it 'should reject empty "lang" attributes' do
    @parser.parse('<pre lang="">foo</pre>').should == %Q{<p>&lt;pre lang=&quot;&quot;&gt;foo&lt;/pre&gt;</p>\n}
  end
end
