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

describe Wikitext::Parser, 'parsing <strong> spans' do
  before do
    @parser = Wikitext::Parser.new
  end

  describe 'marked up using wikitext shorthand' do
    it 'should recognize paired <strong> tokens' do
      @parser.parse("foo '''bar''' baz").should == "<p>foo <strong>bar</strong> baz</p>\n"
    end

    it 'should automatically insert missing closing tags' do
      @parser.parse("foo '''bar").should == "<p>foo <strong>bar</strong></p>\n"
    end

    it 'should automatically close unclosed spans upon hitting newlines' do
      @parser.parse("foo '''bar\nbaz").should == "<p>foo <strong>bar</strong> baz</p>\n"
    end

    it 'should handle (illegal) interleaved spans' do
      @parser.parse("foo '''bar ''inner''' baz''").should == "<p>foo <strong>bar <em>inner</em></strong> baz<em></em></p>\n"
    end

    it 'should have no effect inside <pre> blocks' do
      @parser.parse(" '''foo'''").should == "<pre>'''foo'''</pre>\n"
    end

    it 'should have no effect inside <nowiki> spans' do
      @parser.parse("<nowiki>'''foo'''</nowiki>").should == "<p>'''foo'''</p>\n"
    end

    it "should have no effect if a strong (<strong>) span is already open" do
      @parser.parse("foo <strong>'''bar'''</strong> baz").should == "<p>foo <strong>'''bar'''</strong> baz</p>\n"
    end
  end

  describe 'marked up using HTML tags' do
    it 'should recognized paired <strong> tokens' do
      @parser.parse("foo <strong>bar</strong> baz").should == "<p>foo <strong>bar</strong> baz</p>\n"
    end

    it 'should recognize <strong> tokens case-insensitively' do
      @parser.parse("foo <STRong>bar</STRONG> baz").should == "<p>foo <strong>bar</strong> baz</p>\n"
      @parser.parse("foo <strONG>bar</STRong> baz").should == "<p>foo <strong>bar</strong> baz</p>\n"
      @parser.parse("foo <STRONG>bar</strONG> baz").should == "<p>foo <strong>bar</strong> baz</p>\n"
    end

    it 'should automatically insert missing closing tags' do
      @parser.parse("foo <strong>bar").should == "<p>foo <strong>bar</strong></p>\n"
    end

    it 'should automatically close unclosed spans upon hitting newlines' do
      @parser.parse("foo <strong>bar\nbaz").should == "<p>foo <strong>bar</strong> baz</p>\n"
    end

    it 'should handle (illegal) interleaved spans' do
      expected = "<p>foo <strong>bar <em>inner</em></strong> baz&lt;/em&gt;</p>\n"
      @parser.parse("foo <strong>bar <em>inner</strong> baz</em>").should == expected

      expected = "<p>foo <strong>bar <em>inner</em></strong> baz<em></em></p>\n"
      @parser.parse("foo <strong>bar ''inner</strong> baz''").should == expected
    end

    it 'should handle (illegal) nested <strong> spans' do
      expected = "<p>foo <strong>bar &lt;strong&gt;inner</strong>&lt;/strong&gt; baz</p>\n"
      @parser.parse('foo <strong>bar <strong>inner</strong></strong> baz').should == expected
    end

    it 'should have no effect inside <pre> blocks' do
      @parser.parse(" <strong>foo</strong>").should == "<pre>&lt;strong&gt;foo&lt;/strong&gt;</pre>\n"
    end

    it 'should have no effect inside <nowiki> spans' do
      @parser.parse("<nowiki><strong>foo</strong></nowiki>").should == "<p>&lt;strong&gt;foo&lt;/strong&gt;</p>\n"
    end

    it "should have no effect if an strong (''') span is already open" do
      expected = "<p>foo <strong>&lt;strong&gt;bar&lt;/strong&gt;</strong> baz</p>\n"
      @parser.parse("foo '''<strong>bar</strong>''' baz").should == expected
    end
  end
end
