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
    expected = dedent 6, <<-END
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
    expected = dedent 6, <<-END
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
    expected = dedent 6, <<-END
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
    expected = dedent 6, <<-END
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
    expected = dedent 6, <<-END
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
    expected = dedent 6, <<-END
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
    expected = dedent 6, <<-END
      <ul>
        <li>foo</li>
      </ul>
      <p>bar</p>
    END
    @parser.parse("* foo\nbar").should == expected
  end

  it 'should be terminated by subsequent blockquote at the same level' do
    expected = dedent 6, <<-END
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
    expected = dedent 6, <<-END
      <ul>
        <li>foo
          <ol>
            <li>bar</li>
          </ol>
        </li>
      </ul>
    END
    @parser.parse("* foo\n*# bar").should == expected

    input = dedent 6, <<-END
      * foo
      *# bar
      *# baz
    END
    expected = dedent 6, <<-END
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
    input = dedent 6, <<-END
      * <tt>hello
      * world
    END
    expected = dedent 6, <<-END
      <ul>
        <li><tt>hello</tt></li>
        <li>world</li>
      </ul>
    END
    @parser.parse(input).should == expected
  end

  it 'should automatically close open TT elements on reaching the end of the line' do
    input = dedent 6, <<-END
      * `hello
      * world
    END
    expected = dedent 6, <<-END
      <ul>
        <li><tt>hello</tt></li>
        <li>world</li>
      </ul>
    END
    @parser.parse(input).should == expected
  end

  it 'should automatically close open EM_START elements on reaching the end of the line' do
    input = dedent 6, <<-END
      * <em>hello
      * world
    END
    expected = dedent 6, <<-END
      <ul>
        <li><em>hello</em></li>
        <li>world</li>
      </ul>
    END
    @parser.parse(input).should == expected
  end

  it 'should automatically close open EM elements on reaching the end of the line' do
    input = dedent 6, <<-END
      * ''hello
      * world
    END
    expected = dedent 6, <<-END
      <ul>
        <li><em>hello</em></li>
        <li>world</li>
      </ul>
    END
    @parser.parse(input).should == expected
  end

  it 'should automatically close open STRONG_START elements on reaching the end of the line' do
    input = dedent 6, <<-END
      * <strong>hello
      * world
    END
    expected = dedent 6, <<-END
      <ul>
        <li><strong>hello</strong></li>
        <li>world</li>
      </ul>
    END
    @parser.parse(input).should == expected
  end

  it 'should automatically close open STRONG elements on reaching the end of the line' do
    input = dedent 6, <<-END
      * '''hello
      * world
    END
    expected = dedent 6, <<-END
      <ul>
        <li><strong>hello</strong></li>
        <li>world</li>
      </ul>
    END
    @parser.parse(input).should == expected
  end

  it 'should automatically close open STRONG_EM elements on reaching the end of the line' do
    input = dedent 6, <<-END
      * '''''hello
      * world
    END
    expected = dedent 6, <<-END
      <ul>
        <li><strong><em>hello</em></strong></li>
        <li>world</li>
      </ul>
    END
    @parser.parse(input).should == expected
  end
end
