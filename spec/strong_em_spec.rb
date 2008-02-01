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

describe Wikitext::Parser, 'parsing combined <strong>/<em> spans' do
  before do
    @parser = Wikitext::Parser.new
  end

  it 'should recognize paired "<strong><em>" tokens' do
    @parser.parse("foo '''''bar''''' baz").should == "<p>foo <strong><em>bar</em></strong> baz</p>\n"
  end

  it 'should automatically insert missing closing tags' do
    @parser.parse("foo '''''bar").should == "<p>foo <strong><em>bar</em></strong></p>\n"    # totally missing
    @parser.parse("foo '''''bar''").should == "<p>foo <strong><em>bar</em></strong></p>\n"  # only <strong> missing
    @parser.parse("foo '''''bar'''").should == "<p>foo <strong><em>bar</em></strong></p>\n" # only <em> missing
  end

  it 'should automatically close unclosed spans upon hitting newlines' do
    @parser.parse("foo '''''bar\nbaz").should == "<p>foo <strong><em>bar</em></strong> baz</p>\n"     # totally missing
    @parser.parse("foo '''''bar''\nbaz").should == "<p>foo <strong><em>bar</em></strong> baz</p>\n"   # only <strong> missing
    @parser.parse("foo '''''bar'''\nbaz").should == "<p>foo <strong><em>bar</em></strong> baz</p>\n"  # only <em> missing
  end

  it 'should allow combined "<strong><em>" tokens to interact with separate <strong> and <em> tokens' do
    @parser.parse("foo '''bar ''baz'''''").should == "<p>foo <strong>bar <em>baz</em></strong></p>\n"
    @parser.parse("foo ''bar '''baz'''''").should == "<p>foo <em>bar <strong>baz</strong></em></p>\n"
    @parser.parse("'''''foo'' bar''' baz").should == "<p><strong><em>foo</em> bar</strong> baz</p>\n"
  end

  it 'should handle (illegal) interleaved spans' do
    # ''''' means "<strong><em>" so when we see ''' we try to close the <strong> first, which makes for illegal nesting
    @parser.parse("'''''foo''' bar'' baz").should == "<p><strong><em>foo</em></strong> bar<em> baz</em></p>\n"

    # note that if you really want ''''' to be parsed as "<em><strong>" you have to use whitespace to disambiguate
    @parser.parse("'' '''foo''' bar'' baz").should == "<p><em> <strong>foo</strong> bar</em> baz</p>\n"
  end

  it 'should have no effect inside <pre> blocks' do
    @parser.parse(" '''''foo'''''").should == "<pre>'''''foo'''''</pre>\n"
  end

  it 'should have no effect inside <nowiki> spans' do
    @parser.parse("<nowiki>'''''foo'''''</nowiki>").should == "<p>'''''foo'''''</p>\n"
  end
end
