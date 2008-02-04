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
require 'iconv'

module UTF8
  module Invalid
    TWO_BYTES_MISSING_SECOND_BYTE       = [0b11011111].pack('C*')
    TWO_BYTES_MALFORMED_SECOND_BYTE     = [0b11011111, 0b00001111].pack('C*') # should be 10......
    OVERLONG                            = [0b11000000, 0b10000000].pack('C*') # lead byte is 110..... but code point is <= 127
    OVERLONG_ALT                        = [0b11000001, 0b10000000].pack('C*') # lead byte is 110..... but code point is <= 127
    THREE_BYTES_MISSING_SECOND_BYTE     = [0b11100000].pack('C*')
    THREE_BYTES_MISSING_THIRD_BYTE      = [0b11100000, 0b10000000].pack('C*')
    THREE_BYTES_MALFORMED_SECOND_BYTE   = [0b11100000, 0b00001111, 0b10000000].pack('C*') # should be 10......
    THREE_BYTES_MALFORMED_THIRD_BYTE    = [0b11100000, 0b10000000, 0b00001111].pack('C*') # should be 10......
    FOUR_BYTES_MISSING_SECOND_BYTE      = [0b11110000].pack('C*')
    FOUR_BYTES_MISSING_THIRD_BYTE       = [0b11110000, 0x10111111].pack('C*')
    FOUR_BYTES_MISSING_FOURTH_BYTE      = [0b11110000, 0x10111111, 0x10111111].pack('C*')
    FOUR_BYTES_ILLEGAL_FIRST_BYTE       = [0b11110101, 0x10111111, 0x10111111, 0x10111111].pack('C*')
    FOUR_BYTES_ILLEGAL_FIRST_BYTE_ALT   = [0b11110101, 0x10111111, 0x10111111, 0x10111111].pack('C*')
    FOUR_BYTES_ILLEGAL_FIRST_BYTE_ALT2  = [0b11110101, 0x10111111, 0x10111111, 0x10111111].pack('C*')
    UNEXPECTED_BYTE                     = [0b11111000].pack('C*')
  end # module Invalid
end # module UTF8

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
