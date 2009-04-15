#!/usr/bin/env ruby
# encoding: utf-8
# Copyright 2007-2009 Wincent Colaiuta. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.

# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

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
    Wikitext::Parser.encode_link_target('     hello world').should == 'hello%20world'
    Wikitext::Parser.encode_link_target('      hello world').should == 'hello%20world'
  end

  it 'should eat trailing spaces' do
    Wikitext::Parser.encode_link_target('hello world ').should == 'hello%20world'
    Wikitext::Parser.encode_link_target('hello world  ').should == 'hello%20world'
    Wikitext::Parser.encode_link_target('hello world   ').should == 'hello%20world'
    Wikitext::Parser.encode_link_target('hello world    ').should == 'hello%20world'
    Wikitext::Parser.encode_link_target('hello world     ').should == 'hello%20world'
    Wikitext::Parser.encode_link_target('hello world      ').should == 'hello%20world'
  end

  it 'should eat leading and trailing spaces combined' do
    Wikitext::Parser.encode_link_target('     hello world ').should == 'hello%20world'
    Wikitext::Parser.encode_link_target('     hello world ').should == 'hello%20world'
    Wikitext::Parser.encode_link_target('    hello world  ').should == 'hello%20world'
    Wikitext::Parser.encode_link_target('   hello world   ').should == 'hello%20world'
    Wikitext::Parser.encode_link_target('  hello world    ').should == 'hello%20world'
    Wikitext::Parser.encode_link_target(' hello world     ').should == 'hello%20world'
  end

  it 'should return nothing for input consisting entirely of spaces' do
    Wikitext::Parser.encode_link_target(' ').should == ''
    Wikitext::Parser.encode_link_target('  ').should == ''
    Wikitext::Parser.encode_link_target('   ').should == ''
    Wikitext::Parser.encode_link_target('    ').should == ''
    Wikitext::Parser.encode_link_target('     ').should == ''
    Wikitext::Parser.encode_link_target('      ').should == ''
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

  # "special" links don't get transformed in any way
  describe 'special links' do

    # as of version 1.4.0 the encode_link_target function no longer handles special links
    it 'should (no longer) recognize links which match /\A[a-z]+\/\d+\z/ as being special' do
      string = 'foo/10'
      Wikitext::Parser.encode_special_link_target(string).should == 'foo%2f10'
      Wikitext::Parser.encode_link_target(string).should == 'foo%2f10'
    end

    it "should not recognize links which don't match at /\A/ as being special" do
      string = '  foo/10'
      Wikitext::Parser.encode_special_link_target(string).should_not == string
      string = '..foo/10'
      Wikitext::Parser.encode_special_link_target(string).should_not == string
      string = '12foo/10'
      Wikitext::Parser.encode_special_link_target(string).should_not == string
    end

    it "should not recognize links which don't match at /\z/ as being special" do
      string = 'foo/10  '
      Wikitext::Parser.encode_special_link_target(string).should_not == string
      string = 'foo/10__'
      Wikitext::Parser.encode_special_link_target(string).should_not == string
      string = 'foo/10##'
      Wikitext::Parser.encode_special_link_target(string).should_not == string
      string = 'foo/10ab'
      Wikitext::Parser.encode_special_link_target(string).should_not == string
    end

    it "should not recognize links which don't match at /[a-z]/ (case differences) as being special" do
      string = 'FOO/10'
      Wikitext::Parser.encode_special_link_target(string).should_not == string
    end

    it "should not recognize links which don't match at /[0-9]/ (case differences) as being special" do
      string = 'foo/xx'
      Wikitext::Parser.encode_special_link_target(string).should_not == string
    end

    it "should not recognize links which don't match at /\// as being special" do
      string = 'foo 10'
      Wikitext::Parser.encode_special_link_target(string).should_not == string
    end
  end
end

