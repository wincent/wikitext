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

describe Wikitext::Parser, 'parsing <pre> blocks' do
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
    @parser.parse(">  foo").should == "<blockquote><pre>foo</pre>\n</blockquote>\n"

    # same, but continued over multiple lines
    @parser.parse(">  foo\n>  bar").should == "<blockquote><pre>foo\nbar</pre>\n</blockquote>\n"

    # nesting inside double blockquotes
    @parser.parse("> >  foo").should == "<blockquote><blockquote><pre>foo</pre>\n</blockquote>\n</blockquote>\n"

    # same, but continued over multiple lines
    @parser.parse("> >  foo\n> >  bar").should == "<blockquote><blockquote><pre>foo\nbar</pre>\n</blockquote>\n</blockquote>\n"
  end

  it 'should automatically close preceding blocks at the same depth' do
    @parser.parse("> foo\n bar").should == "<blockquote><p>foo</p>\n</blockquote>\n<pre>bar</pre>\n"
    @parser.parse("> > foo\n bar").should == "<blockquote><blockquote><p>foo</p>\n</blockquote>\n</blockquote>\n<pre>bar</pre>\n"
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
