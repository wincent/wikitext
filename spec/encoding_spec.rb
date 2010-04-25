#!/usr/bin/env ruby
# Copyright 2007-2010 Wincent Colaiuta. All rights reserved.
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
require 'iconv'

describe Wikitext, 'with invalidly encoded input' do
  before do
    @parser = Wikitext::Parser.new
  end

  it 'should raise an exception for missing second byte' do
    lambda { @parser.parse(UTF8::Invalid::TWO_BYTES_MISSING_SECOND_BYTE) }.should raise_error(Wikitext::Parser::Error)
    lambda {
      @parser.parse('good text' + UTF8::Invalid::TWO_BYTES_MISSING_SECOND_BYTE)
    }.should raise_error(Wikitext::Parser::Error)
  end

  it 'should raise an exception for malformed second byte' do
    lambda { @parser.parse(UTF8::Invalid::TWO_BYTES_MALFORMED_SECOND_BYTE) }.should raise_error(Wikitext::Parser::Error)
    lambda {
      @parser.parse('good text' + UTF8::Invalid::TWO_BYTES_MALFORMED_SECOND_BYTE)
    }.should raise_error(Wikitext::Parser::Error)
  end

  it 'should raise an exception for overlong sequence' do
    lambda { @parser.parse(UTF8::Invalid::OVERLONG) }.should raise_error(Wikitext::Parser::Error)
    lambda {
      @parser.parse('good text' + UTF8::Invalid::OVERLONG)
    }.should raise_error(Wikitext::Parser::Error)

    # alternate
    lambda { @parser.parse(UTF8::Invalid::OVERLONG_ALT) }.should raise_error(Wikitext::Parser::Error)
    lambda {
      @parser.parse('good text' + UTF8::Invalid::OVERLONG_ALT)
    }.should raise_error(Wikitext::Parser::Error)
  end

  it 'should raise an exception for missing second byte in three-byte sequence' do
    lambda { @parser.parse(UTF8::Invalid::THREE_BYTES_MISSING_SECOND_BYTE) }.should raise_error(Wikitext::Parser::Error)
    lambda {
      @parser.parse('good text' + UTF8::Invalid::THREE_BYTES_MISSING_SECOND_BYTE)
    }.should raise_error(Wikitext::Parser::Error)
  end

  it 'should raise an exception for missing third byte in three-byte sequence' do
    lambda { @parser.parse(UTF8::Invalid::THREE_BYTES_MISSING_THIRD_BYTE) }.should raise_error(Wikitext::Parser::Error)
    lambda {
      @parser.parse('good text' + UTF8::Invalid::THREE_BYTES_MISSING_THIRD_BYTE)
    }.should raise_error(Wikitext::Parser::Error)
  end

  it 'should raise an exception for malformed second byte in three-byte sequence' do
    lambda { @parser.parse(UTF8::Invalid::THREE_BYTES_MALFORMED_SECOND_BYTE) }.should raise_error(Wikitext::Parser::Error)
    lambda {
      @parser.parse('good text' + UTF8::Invalid::THREE_BYTES_MALFORMED_SECOND_BYTE)
    }.should raise_error(Wikitext::Parser::Error)
  end

  it 'should raise an exception for malformed third byte in three-byte sequence' do
    lambda { @parser.parse(UTF8::Invalid::THREE_BYTES_MALFORMED_THIRD_BYTE) }.should raise_error(Wikitext::Parser::Error)
    lambda {
      @parser.parse('good text' + UTF8::Invalid::THREE_BYTES_MALFORMED_THIRD_BYTE)
    }.should raise_error(Wikitext::Parser::Error)
  end

  it 'should raise an exception for missing second byte in four-byte sequence' do
    lambda { @parser.parse(UTF8::Invalid::FOUR_BYTES_MISSING_SECOND_BYTE) }.should raise_error(Wikitext::Parser::Error)
    lambda {
      @parser.parse('good text' + UTF8::Invalid::FOUR_BYTES_MISSING_SECOND_BYTE)
    }.should raise_error(Wikitext::Parser::Error)
  end

  it 'should raise an exception for missing third byte in four-byte sequence' do
    lambda { @parser.parse(UTF8::Invalid::FOUR_BYTES_MISSING_THIRD_BYTE) }.should raise_error(Wikitext::Parser::Error)
    lambda {
      @parser.parse('good text' + UTF8::Invalid::FOUR_BYTES_MISSING_THIRD_BYTE)
    }.should raise_error(Wikitext::Parser::Error)
  end

  it 'should raise an exception for missing fourth byte in four-byte sequence' do
    lambda { @parser.parse(UTF8::Invalid::FOUR_BYTES_MISSING_FOURTH_BYTE) }.should raise_error(Wikitext::Parser::Error)
    lambda {
      @parser.parse('good text' + UTF8::Invalid::FOUR_BYTES_MISSING_FOURTH_BYTE)
    }.should raise_error(Wikitext::Parser::Error)
  end

  it 'should raise an exception for illegal first byte in four-byte sequence' do
    lambda { @parser.parse(UTF8::Invalid::FOUR_BYTES_ILLEGAL_FIRST_BYTE) }.should raise_error(Wikitext::Parser::Error)
    lambda {
      @parser.parse('good text' + UTF8::Invalid::FOUR_BYTES_ILLEGAL_FIRST_BYTE)
    }.should raise_error(Wikitext::Parser::Error)

    lambda { @parser.parse(UTF8::Invalid::FOUR_BYTES_ILLEGAL_FIRST_BYTE_ALT) }.should raise_error(Wikitext::Parser::Error)
    lambda {
      @parser.parse('good text' + UTF8::Invalid::FOUR_BYTES_ILLEGAL_FIRST_BYTE_ALT)
    }.should raise_error(Wikitext::Parser::Error)

    lambda { @parser.parse(UTF8::Invalid::FOUR_BYTES_ILLEGAL_FIRST_BYTE_ALT2) }.should raise_error(Wikitext::Parser::Error)
    lambda {
      @parser.parse('good text' + UTF8::Invalid::FOUR_BYTES_ILLEGAL_FIRST_BYTE_ALT2)
    }.should raise_error(Wikitext::Parser::Error)
  end

  it 'should raise an exception for unexpected bytes' do
    lambda { @parser.parse(UTF8::Invalid::UNEXPECTED_BYTE) }.should raise_error(Wikitext::Parser::Error)
    lambda {
      @parser.parse('good text' + UTF8::Invalid::UNEXPECTED_BYTE)
    }.should raise_error(Wikitext::Parser::Error)
  end
end
