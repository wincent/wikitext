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

describe Wikitext::Parser, 'parsing paragraphs' do
  before do
    @parser = Wikitext::Parser.new
  end

  it 'should wrap bare text in paragraph tags' do
    @parser.parse('foo').should == "<p>foo</p>\n"
  end

  it 'should treat consecutive line breaks as paragraph breaks' do
    @parser.parse("foo\n\nbar").should == "<p>foo</p>\n<p>bar</p>\n"
  end

  it 'should not insert excess empty paragraphs or spaces when processing multiple consecutive line breaks' do
    @parser.parse("foo\n\n\n\nbar").should == "<p>foo</p>\n<p>bar</p>\n"
  end

  it 'should not translate single line breaks into spaces when they appear at the start of a paragraph' do
    @parser.parse("\nfoo").should == "<p>foo</p>\n"
  end

  it 'should not translate single line breaks into spaces when they appear at the end of a paragraph' do
    @parser.parse("foo\n").should == "<p>foo</p>\n"
  end
end

