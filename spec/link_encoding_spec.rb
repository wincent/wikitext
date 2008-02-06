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
require 'uri'

describe Wikitext, 'encoding a link target' do
  it 'should complain if passed nil' do
    lambda { Wikitext::Parser.encode_link_target(nil) }.should raise_error
  end

  it 'should do nothing on zero-length input' do
    Wikitext::Parser.encode_link_target('').should == ''
  end

  it 'should convert embedded spaces into "%20"' do
    Wikitext::Parser.encode_link_target('hello world').should == 'hello%20world'
  end

  it 'should eat leading spaces' do
    Wikitext::Parser.encode_link_target(' hello world').should == 'hello%20world'
    Wikitext::Parser.encode_link_target('  hello world').should == 'hello%20world'
    Wikitext::Parser.encode_link_target('   hello world').should == 'hello%20world'
    Wikitext::Parser.encode_link_target('    hello world').should == 'hello%20world'
  end

  it 'should eat trailing spaces' do
    Wikitext::Parser.encode_link_target('hello world ').should == 'hello%20world'
    Wikitext::Parser.encode_link_target('hello world  ').should == 'hello%20world'
    Wikitext::Parser.encode_link_target('hello world   ').should == 'hello%20world'
    Wikitext::Parser.encode_link_target('hello world    ').should == 'hello%20world'
  end

  it 'should eat leading and trailing spaces combined' do
    Wikitext::Parser.encode_link_target('    hello world ').should == 'hello%20world'
    Wikitext::Parser.encode_link_target('   hello world  ').should == 'hello%20world'
    Wikitext::Parser.encode_link_target('  hello world   ').should == 'hello%20world'
    Wikitext::Parser.encode_link_target(' hello world    ').should == 'hello%20world'
  end

  it 'should return nothing for input consisting entirely of spaces' do
    Wikitext::Parser.encode_link_target(' ').should == ''
    Wikitext::Parser.encode_link_target('  ').should == ''
    Wikitext::Parser.encode_link_target('   ').should == ''
    Wikitext::Parser.encode_link_target('    ').should == ''
  end

  it 'should convert reserved symbols into percent escapes' do
    Wikitext::Parser.encode_link_target('http://www.apple.com/q?foo').should == 'http%3a%2f%2fwww.apple.com%2fq%3ffoo'
  end

  it 'should convert non-ASCII into UTF-8 and then apply percent escapes' do
    Wikitext::Parser.encode_link_target('cañon').should == 'ca%c3%b1on'
  end

  it 'should handle mixed scenarios (commas, double-quotes and UTF-8)' do
    Wikitext::Parser.encode_link_target('foo, "bar" & baz €').should == 'foo%2c%20%22bar%22%20%26%20baz%20%e2%82%ac'
  end

  it 'should get the same answer as URI.escape' do
    reserved = Regexp.new("[^#{URI::PATTERN::UNRESERVED}]")
    ['foo bar', 'http://www.google.com/search?q=hello&foo=bar', '€'].each do |string|
      Wikitext::Parser.encode_link_target(string).should == URI.escape(string, reserved).downcase
    end
  end
end

