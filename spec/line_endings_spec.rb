#!/usr/bin/env ruby
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
