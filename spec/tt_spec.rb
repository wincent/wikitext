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

describe Wikitext::Parser, 'parsing <tt> spans' do
  before do
    @parser = Wikitext::Parser.new
  end

  it 'should recognize paired <tt> and </tt> tags' do
    @parser.parse('foo <tt>bar</tt> baz').should == "<p>foo <tt>bar</tt> baz</p>\n"
  end

  it 'should automatically insert missing closing tags' do
    @parser.parse('foo <tt>bar').should == "<p>foo <tt>bar</tt></p>\n"
  end

  it 'should automatically close unclosed spans upon hitting newline' do
    @parser.parse("foo <tt>bar\nbaz").should == "<p>foo <tt>bar</tt> baz</p>\n"
  end

  it 'should convert unexpected closing tags into entities' do
    @parser.parse('foo </tt>bar').should == "<p>foo &lt;/tt&gt;bar</p>\n"
  end

  it 'should handle (illegal) nested <tt> spans' do
    @parser.parse('foo <tt>bar <tt>inner</tt></tt> baz').should == "<p>foo <tt>bar &lt;tt&gt;inner</tt>&lt;/tt&gt; baz</p>\n"
  end

  it 'should handle (illegal) interleaved spans' do
    @parser.parse("foo <tt>bar '''inner</tt> baz'''").should == "<p>foo <tt>bar <strong>inner</strong></tt> baz<strong></strong></p>\n"
  end

  it 'should have no effect inside <pre> blocks' do
    @parser.parse(' <tt>foo</tt>').should == "<pre>&lt;tt&gt;foo&lt;/tt&gt;</pre>\n"
  end

  it 'should have no effect inside <nowiki> spans' do
    @parser.parse('<nowiki><tt>foo</tt></nowiki>').should == "<p>&lt;tt&gt;foo&lt;/tt&gt;</p>\n"
  end
end

describe Wikitext::Parser, 'parsing backtick spans' do
  before do
    @parser = Wikitext::Parser.new
  end

  it 'should recognize paired backticks' do
    @parser.parse('foo `bar` baz').should == "<p>foo <tt>bar</tt> baz</p>\n"
  end

  it 'should automatically insert missing closing backtick' do
    @parser.parse('foo `bar').should == "<p>foo <tt>bar</tt></p>\n"
  end

  it 'should automatically close unclosed spans upon hitting newline' do
    @parser.parse("foo `bar\nbaz").should == "<p>foo <tt>bar</tt> baz</p>\n"
  end

  it 'should handle (illegal) interleaved spans' do
    @parser.parse("foo `bar '''inner` baz'''").should == "<p>foo <tt>bar <strong>inner</strong></tt> baz<strong></strong></p>\n"
  end

  it 'should have no effect inside <pre> blocks' do
    @parser.parse(' `foo`').should == "<pre>`foo`</pre>\n"
  end

  it 'should have no effect inside <nowiki> spans' do
    @parser.parse('<nowiki>`foo`</nowiki>').should == "<p>`foo`</p>\n"
  end
end

