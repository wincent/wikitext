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

describe Wikitext::Parser, 'parsing <em> spans' do
  before do
    @parser = Wikitext::Parser.new
  end

  describe 'marked up using wikitext shorthand' do
    it 'should recognize paired <em> tokens' do
      @parser.parse("foo ''bar'' baz").should == "<p>foo <em>bar</em> baz</p>\n"
    end

    it 'should automatically insert missing closing tags' do
      @parser.parse("foo ''bar").should == "<p>foo <em>bar</em></p>\n"
    end

    it 'should automatically close unclosed spans upon hitting newlines' do
      @parser.parse("foo ''bar\nbaz").should == "<p>foo <em>bar</em> baz</p>\n"
    end

    it 'should handle (illegal) interleaved spans' do
      @parser.parse("foo ''bar '''inner'' baz'''").should == "<p>foo <em>bar <strong>inner</strong></em> baz<strong></strong></p>\n"
    end

    it 'should have no effect inside <pre> blocks' do
      @parser.parse(" ''foo''").should == "<pre>''foo''</pre>\n"
    end

    it 'should have no effect inside <nowiki> spans' do
      @parser.parse("<nowiki>''foo''</nowiki>").should == "<p>''foo''</p>\n"
    end
  end

  describe 'marked up using HTML tags' do
    it 'should recognized paired <em> tokens' do
      @parser.parse("foo <em>bar</em> baz").should == "<p>foo <em>bar</em> baz</p>\n"
    end

    it 'should recognize <em> tokens case-insensitively' do
      @parser.parse("foo <EM>bar</EM> baz").should == "<p>foo <em>bar</em> baz</p>\n"
      @parser.parse("foo <Em>bar</eM> baz").should == "<p>foo <em>bar</em> baz</p>\n"
      @parser.parse("foo <eM>bar</Em> baz").should == "<p>foo <em>bar</em> baz</p>\n"
    end

  end
end
