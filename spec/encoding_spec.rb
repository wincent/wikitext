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
