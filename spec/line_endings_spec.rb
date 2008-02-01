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

describe Wikitext::Parser do
  before do
    @parser = Wikitext::Parser.new
  end

  it 'should default to UNIX line endings' do
    @parser.line_ending.should == "\n"
  end

  it 'should greedily match Mac line endings in input if possible' do
    # note how the embedded "\r\n" is interpreted as a single token (and converted to exactly one space)
    # it is not treated as two separate tokens
    @parser.parse("foo\r\nbar").should == "<p>foo bar</p>\n"
  end
end

describe Wikitext::Parser, 'with UNIX line endings' do
  before do
    @parser = Wikitext::Parser.new
    @parser.line_ending = "\n"
  end

  it 'should convert line endings within spans to a single space' do
    @parser.parse("foo\nbar").should == "<p>foo bar</p>\n"
  end

  it 'should normalize non-UNIX line endings' do
    @parser.parse("foo\rbar").should == "<p>foo bar</p>\n"
    @parser.parse("foo\r\nbar").should == "<p>foo bar</p>\n"
  end
end

describe Wikitext::Parser, 'with Windows line endings' do
  before do
    @parser = Wikitext::Parser.new
    @parser.line_ending = "\r"
  end

  it 'should convert line endings within spans to a single space' do
    @parser.parse("foo\rbar").should == "<p>foo bar</p>\r"
  end

  it 'should normalize non-Windows line endings' do
    @parser.parse("foo\nbar").should == "<p>foo bar</p>\r"
    @parser.parse("foo\r\nbar").should == "<p>foo bar</p>\r"
  end
end

describe Wikitext::Parser, 'with Mac line endings' do
  before do
    @parser = Wikitext::Parser.new
    @parser.line_ending = "\r\n"
  end

  it 'should convert line endings within spans to a single space' do
    @parser.parse("foo\r\nbar").should == "<p>foo bar</p>\r\n"
  end

  it 'should normalize non-Mac line endings' do
    @parser.parse("foo\nbar").should == "<p>foo bar</p>\r\n"
    @parser.parse("foo\rbar").should == "<p>foo bar</p>\r\n"
  end
end
