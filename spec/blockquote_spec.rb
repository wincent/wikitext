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

describe Wikitext::Parser, 'parsing blockquotes' do
  before do
    @parser = Wikitext::Parser.new
  end

  it 'should treat ">" in first column as a blockquote marker' do
    @parser.parse('>foo').should == "<blockquote>\n<p>foo</p>\n</blockquote>\n"
  end

  it 'should accept (and ignore) one optional space after the ">"' do
    @parser.parse('> foo').should == "<blockquote>\n<p>foo</p>\n</blockquote>\n"
  end

  it 'should recognize consecutive ">" as continuance of blockquote section' do
    @parser.parse("> foo\n> bar").should == "<blockquote>\n<p>foo bar</p>\n</blockquote>\n"
  end

  it 'should not give ">" special treatment when not on the far left' do
    @parser.parse('foo > bar').should == "<p>foo &gt; bar</p>\n"
  end

  it 'should allow nesting of blockquotes' do
    @parser.parse('> > foo').should == "<blockquote>\n<blockquote>\n<p>foo</p>\n</blockquote>\n</blockquote>\n"
  end

  it 'should allow opening of a nested blockquote after other content' do
    @parser.parse("> foo\n> > bar").should == "<blockquote>\n<p>foo</p>\n<blockquote>\n<p>bar</p>\n</blockquote>\n</blockquote>\n"
  end

  it 'should allow opening of a nested blockquote before other content' do
    @parser.parse("> > foo\n> bar").should == "<blockquote>\n<blockquote>\n<p>foo</p>\n</blockquote>\n<p>bar</p>\n</blockquote>\n"
  end

  it 'should accept an empty blockquote' do
    @parser.parse('>').should == "<blockquote>\n</blockquote>\n"
  end

  it 'should jump out of blockquote mode on seeing a normal line of text' do
    @parser.parse("> foo\nbar").should == "<blockquote>\n<p>foo</p>\n</blockquote>\n<p>bar</p>\n"
  end

  it 'should allow nesting of h1 blocks' do
    @parser.parse('> = foo =').should == "<blockquote>\n<h1>foo</h1>\n</blockquote>\n"
  end

  it 'should allow nesting of h2 blocks' do
    @parser.parse('> == foo ==').should == "<blockquote>\n<h2>foo</h2>\n</blockquote>\n"
  end

  it 'should allow nesting of h3 blocks' do
    @parser.parse('> === foo ===').should == "<blockquote>\n<h3>foo</h3>\n</blockquote>\n"
  end

  it 'should allow nesting of h4 blocks' do
    @parser.parse('> ==== foo ====').should == "<blockquote>\n<h4>foo</h4>\n</blockquote>\n"
  end

  it 'should allow nesting of h5 blocks' do
    @parser.parse('> ===== foo =====').should == "<blockquote>\n<h5>foo</h5>\n</blockquote>\n"
  end

  it 'should allow nesting of h6 blocks' do
    @parser.parse('> ====== foo ======').should == "<blockquote>\n<h6>foo</h6>\n</blockquote>\n"
  end

  # TODO: tests for nesting other types of blocks
end
