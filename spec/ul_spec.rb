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
    @parser.parse('*foo').should == "<ul>\n<li>foo</li>\n</ul>\n"
  end

  it 'should allow and consume optional space after the last <ul> marker' do
    @parser.parse('* foo').should == "<ul>\n<li>foo</li>\n</ul>\n"    # exactly one space consumed
    @parser.parse('*  foo').should == "<ul>\n<li>foo</li>\n</ul>\n"   # multiple spaces consumed
  end

  it 'should consider a space after an <ul> marker to indicate that it will be the last marker' do
    @parser.parse('* * foo').should == "<ul>\n<li>* foo</li>\n</ul>\n"
  end

  it 'should only recognize <ul> markers if they or a direct ancestor start in the left column' do
    @parser.parse(' * foo').should == "<pre>* foo</pre>\n"
  end

  it 'should recognize <ul> markers nested inside blockquote blocks' do
    @parser.parse('> * foo').should == "<blockquote><ul>\n<li>foo</li>\n</ul>\n</blockquote>\n"
  end

  it 'should display excess <ul> markers as literals' do
    # this provides feedback to the user
    @parser.parse('** foo').should == "<ul>\n<li>* foo</li>\n</ul>\n"
    @parser.parse('*** foo').should == "<ul>\n<li>** foo</li>\n</ul>\n"
  end

  it 'should recognize a multi-item, single-level list' do
    @parser.parse("* foo\n* bar").should == "<ul>\n<li>foo</li>\n<li>bar</li>\n</ul>\n"
  end

  it 'should recognize a multi-item, nested list (two levels)' do
    @parser.parse("* foo\n** bar").should == "<ul>\n<li>foo\n<ul>\n<li>bar</li>\n</ul>\n</li>\n</ul>\n"
  end

  it 'should recognize a multi-item, nested list (three levels)' do
    @parser.parse("* foo\n** bar\n*** baz").should == "<ul>\n<li>foo\n<ul>\n<li>bar\n<ul>\n<li>baz</li>\n</ul>\n</li>\n</ul>\n</li>\n</ul>\n"
  end

  it 'should recognize lists in which nesting level increases and then is maintained' do
    @parser.parse("* foo\n** bar\n** baz").should == "<ul>\n<li>foo\n<ul>\n<li>bar</li>\n<li>baz</li>\n</ul>\n</li>\n</ul>\n"
  end

  it 'should recognize lists in which nesting level increases and then decreases' do
    @parser.parse("* foo\n** bar\n* baz").should == "<ul>\n<li>foo\n<ul>\n<li>bar</li>\n</ul>\n</li>\n<li>baz</li>\n</ul>\n"
  end

  it 'should be terminated by subsequent paragraph at the same level' do
    @parser.parse("* foo\nbar").should == "<ul>\n<li>foo</li>\n</ul>\n<p>bar</p>\n"
  end

  it 'should be terminated by subsequent blockquote at the same level' do
    @parser.parse("* foo\n> bar").should == "<ul>\n<li>foo</li>\n</ul>\n<blockquote><p>bar</p>\n</blockquote>\n"
  end

  it 'should be terminated by subsequent heading at the same level' do
    @parser.parse("* foo\n====== bar ======").should == "<ul>\n<li>foo</li>\n</ul>\n<h6>bar</h6>\n"
    @parser.parse("* foo\n===== bar =====").should == "<ul>\n<li>foo</li>\n</ul>\n<h5>bar</h5>\n"
    @parser.parse("* foo\n==== bar ====").should == "<ul>\n<li>foo</li>\n</ul>\n<h4>bar</h4>\n"
    @parser.parse("* foo\n=== bar ===").should == "<ul>\n<li>foo</li>\n</ul>\n<h3>bar</h3>\n"
    @parser.parse("* foo\n== bar ==").should == "<ul>\n<li>foo</li>\n</ul>\n<h2>bar</h2>\n"
    @parser.parse("* foo\n= bar =").should == "<ul>\n<li>foo</li>\n</ul>\n<h1>bar</h1>\n"
  end

  it 'should be terminated by subsequent <pre> block at the same level' do
    @parser.parse("* foo\n bar").should == "<ul>\n<li>foo</li>\n</ul>\n<pre>bar</pre>\n"
  end

  it 'should be terminated by subsequent ordered list at the same level' do
    @parser.parse("* foo\n# bar").should == "<ul>\n<li>foo</li>\n</ul>\n<ol>\n<li>bar</li>\n</ol>\n"
  end

  it 'should recognize lists which contain nested ordered lists' do
    @parser.parse("* foo\n*# bar").should == "<ul>\n<li>foo\n<ol>\n<li>bar</li>\n</ol>\n</li>\n</ul>\n"
    input = <<INPUT
* foo
*# bar
*# baz
INPUT
    expected = <<EXPECTED
<ul>
<li>foo
<ol>
<li>bar</li>
<li>baz</li>
</ol>
</li>
</ul>
EXPECTED
    @parser.parse(input).should == expected
  end
end
