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

  it 'should turn space-to-underscore off by default' do
    @parser.space_to_underscore.should == false
  end
end

describe Wikitext::Parser, 'parsing non-ASCII input' do
  before do
    @parser = Wikitext::Parser.new
  end

  it 'should convert non-ASCII characters to numeric entities' do
    @parser.parse('€').should == "<p>&#x20ac;</p>\n"
  end
end

describe Wikitext::Parser, 'parsing characters which have special meaning in HTML' do
  before do
    @parser = Wikitext::Parser.new
  end

  it 'should convert "<" into the corresponding named entity' do
    @parser.parse('<').should == "<p>&lt;</p>\n"
  end

  it 'should convert ">" into the corresponding named entity' do
    # can't put ">" in the first column as that would indicate a blockquote
    @parser.parse("foo >").should == "<p>foo &gt;</p>\n"
  end

  it 'should convert "&" into the corresponding named entity' do
    @parser.parse('&').should == "<p>&amp;</p>\n"
  end

  it 'should convert \'"\' into the corresponding named entity' do
    @parser.parse('"').should == "<p>&quot;</p>\n"
  end
end
