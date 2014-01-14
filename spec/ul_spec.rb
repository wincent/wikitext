# Copyright 2007-2014 Wincent Colaiuta. All rights reserved.
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
require 'wikitext'

describe Wikitext::Parser, 'parsing unordered lists' do
  before do
    @parser = Wikitext::Parser.new
  end

  it 'should recognize a single item list' do
    @parser.parse('*foo').should == "<ul>\n  <li>foo</li>\n</ul>\n"
  end

  it 'should allow and consume optional space after the last <ul> marker' do
    @parser.parse('* foo').should == "<ul>\n  <li>foo</li>\n</ul>\n"    # exactly one space consumed
    @parser.parse('*  foo').should == "<ul>\n  <li>foo</li>\n</ul>\n"   # multiple spaces consumed
  end

  it 'should consider a space after an <ul> marker to indicate that it will be the last marker' do
    @parser.parse('* * foo').should == "<ul>\n  <li>* foo</li>\n</ul>\n"
  end

  it 'should only recognize <ul> markers if they or a direct ancestor start in the left column' do
    @parser.parse(' * foo').should == "<pre>* foo</pre>\n"
  end

  it 'should recognize <ul> markers nested inside blockquote blocks' do
    expected = dedent <<-END
      <blockquote>
        <ul>
          <li>foo</li>
        </ul>
      </blockquote>
    END
    @parser.parse('> * foo').should == expected
  end

  it 'should display excess <ul> markers as literals' do
    # this provides feedback to the user
    @parser.parse('** foo').should == "<ul>\n  <li>* foo</li>\n</ul>\n"
    @parser.parse('*** foo').should == "<ul>\n  <li>** foo</li>\n</ul>\n"
  end

  it 'should recognize a multi-item, single-level list' do
    expected = dedent <<-END
      <ul>
        <li>foo</li>
        <li>bar</li>
      </ul>
    END
    @parser.parse("* foo\n* bar").should == expected
  end

  it 'should recognize a multi-item, nested list (two levels)' do
    # indentation of nested lists is tricky
    # the last </li> appears too far to the left
    # the difficult is that sometimes li has to act like a block level element (like blockquote, does emit before dedent)
    # and at other times it has to act like p (doesn't emit before dedent)
    # so basically when nested we need to do an emitting dedent
    # and when not we need to do a non-emitting one
    expected = dedent <<-END
      <ul>
        <li>foo
          <ul>
            <li>bar</li>
          </ul>
        </li>
      </ul>
    END
  @parser.parse("* foo\n** bar").should == expected
  end

  it 'should recognize a multi-item, nested list (three levels)' do
    expected = dedent <<-END
      <ul>
        <li>foo
          <ul>
            <li>bar
              <ul>
                <li>baz</li>
              </ul>
            </li>
          </ul>
        </li>
      </ul>
    END
    @parser.parse("* foo\n** bar\n*** baz").should == expected
  end

  it 'should recognize lists in which nesting level increases and then is maintained' do
    expected = dedent <<-END
      <ul>
        <li>foo
          <ul>
            <li>bar</li>
            <li>baz</li>
          </ul>
        </li>
      </ul>
    END
    @parser.parse("* foo\n** bar\n** baz").should == expected
  end

  it 'should recognize lists in which nesting level increases and then decreases' do
    expected = dedent <<-END
      <ul>
        <li>foo
          <ul>
            <li>bar</li>
          </ul>
        </li>
        <li>baz</li>
      </ul>
    END
    @parser.parse("* foo\n** bar\n* baz").should == expected
  end

  it 'should be terminated by subsequent paragraph at the same level' do
    expected = dedent <<-END
      <ul>
        <li>foo</li>
      </ul>
      <p>bar</p>
    END
    @parser.parse("* foo\nbar").should == expected
  end

  it 'should be terminated by subsequent blockquote at the same level' do
    expected = dedent <<-END
      <ul>
        <li>foo</li>
      </ul>
      <blockquote>
        <p>bar</p>
      </blockquote>
    END
    @parser.parse("* foo\n> bar").should == expected
  end

  it 'should be terminated by subsequent heading at the same level' do
    @parser.parse("* foo\n====== bar ======").should == "<ul>\n  <li>foo</li>\n</ul>\n<h6>bar</h6>\n"
    @parser.parse("* foo\n===== bar =====").should == "<ul>\n  <li>foo</li>\n</ul>\n<h5>bar</h5>\n"
    @parser.parse("* foo\n==== bar ====").should == "<ul>\n  <li>foo</li>\n</ul>\n<h4>bar</h4>\n"
    @parser.parse("* foo\n=== bar ===").should == "<ul>\n  <li>foo</li>\n</ul>\n<h3>bar</h3>\n"
    @parser.parse("* foo\n== bar ==").should == "<ul>\n  <li>foo</li>\n</ul>\n<h2>bar</h2>\n"
    @parser.parse("* foo\n= bar =").should == "<ul>\n  <li>foo</li>\n</ul>\n<h1>bar</h1>\n"
  end

  it 'should be terminated by subsequent <pre> block at the same level' do
    @parser.parse("* foo\n bar").should == "<ul>\n  <li>foo</li>\n</ul>\n<pre>bar</pre>\n"
  end

  it 'should be terminated by subsequent ordered list at the same level' do
    expected = dedent 6,<<-END
      <ul>
        <li>foo</li>
      </ul>
      <ol>
        <li>bar</li>
      </ol>
    END
    @parser.parse("* foo\n# bar").should == expected
  end

  it 'should recognize lists which contain nested ordered lists' do
    expected = dedent <<-END
      <ul>
        <li>foo
          <ol>
            <li>bar</li>
          </ol>
        </li>
      </ul>
    END
    @parser.parse("* foo\n*# bar").should == expected

    input = dedent <<-END
      * foo
      *# bar
      *# baz
    END
    expected = dedent <<-END
      <ul>
        <li>foo
          <ol>
            <li>bar</li>
            <li>baz</li>
          </ol>
        </li>
      </ul>
    END
    @parser.parse(input).should == expected
  end

  it 'should automatically close open TT_START elements on reaching the end of the line' do
    # this (and the same for all other span-level elements) was a bug
    input = dedent <<-END
      * <tt>hello
      * world
    END
    expected = dedent <<-END
      <ul>
        <li><code>hello</code></li>
        <li>world</li>
      </ul>
    END
    @parser.parse(input).should == expected
  end

  it 'should automatically close open TT elements on reaching the end of the line' do
    input = dedent <<-END
      * `hello
      * world
    END
    expected = dedent <<-END
      <ul>
        <li><code>hello</code></li>
        <li>world</li>
      </ul>
    END
    @parser.parse(input).should == expected
  end

  it 'should automatically close open EM_START elements on reaching the end of the line' do
    input = dedent <<-END
      * <em>hello
      * world
    END
    expected = dedent <<-END
      <ul>
        <li><em>hello</em></li>
        <li>world</li>
      </ul>
    END
    @parser.parse(input).should == expected
  end

  it 'should automatically close open EM elements on reaching the end of the line' do
    input = dedent <<-END
      * ''hello
      * world
    END
    expected = dedent <<-END
      <ul>
        <li><em>hello</em></li>
        <li>world</li>
      </ul>
    END
    @parser.parse(input).should == expected
  end

  it 'should automatically close open STRONG_START elements on reaching the end of the line' do
    input = dedent <<-END
      * <strong>hello
      * world
    END
    expected = dedent <<-END
      <ul>
        <li><strong>hello</strong></li>
        <li>world</li>
      </ul>
    END
    @parser.parse(input).should == expected
  end

  it 'should automatically close open STRONG elements on reaching the end of the line' do
    input = dedent <<-END
      * '''hello
      * world
    END
    expected = dedent <<-END
      <ul>
        <li><strong>hello</strong></li>
        <li>world</li>
      </ul>
    END
    @parser.parse(input).should == expected
  end

  it 'should automatically close open STRONG_EM elements on reaching the end of the line' do
    input = dedent <<-END
      * '''''hello
      * world
    END
    expected = dedent <<-END
      <ul>
        <li><strong><em>hello</em></strong></li>
        <li>world</li>
      </ul>
    END
    @parser.parse(input).should == expected
  end
end
