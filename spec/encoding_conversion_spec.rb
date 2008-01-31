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
#require 'uri'

module UTF8
  module Invalid
    TWO_BYTES_MISSING_SECOND_BYTE     = [0b11011111].pack('C*')
    TWO_BYTES_MALFORMED_SECOND_BYTE   = [0b11011111, 0b00001111].pack('C*') # should be 10......
    OVERLONG                          = [0b11000000, 0b10000000].pack('C*') # lead byte is 110..... but code point is <= 127
    OVERLONG_ALT                      = [0b11000001, 0b10000000].pack('C*') # lead byte is 110..... but code point is <= 127
    THREE_BYTES_MISSING_SECOND_BYTE   = [0b11100000].pack('C*')
    THREE_BYTES_MISSING_THIRD_BYTE    = [0b11100000, 0b10000000].pack('C*')
    THREE_BYTES_MALFORMED_SECOND_BYTE = [0b11100000, 0b00001111, 0b10000000].pack('C*') # should be 10......
    THREE_BYTES_MALFORMED_THIRD_BYTE  = [0b11100000, 0b10000000, 0b00001111].pack('C*') # should be 10......
    EXCEEDS_ENCODABLE_RANGE_FOR_UCS2  = [0b11110000].pack('C*')
    UNEXPECTED_BYTE                   = [0b11111000].pack('C*')
  end # module Invalid
end # module UTF8

describe Wikitext, 'with invalidly encoded input' do
  before do
    @parser = Wikitext::Parser.new
  end

  it 'should raise an exception for missing second byte' do
    lambda { @parser.parse(UTF8::Invalid::TWO_BYTES_MISSING_SECOND_BYTE) }.should raise_error(RangeError, /truncated/)
    lambda {
      @parser.parse('good text' + UTF8::Invalid::TWO_BYTES_MISSING_SECOND_BYTE)
    }.should raise_error(RangeError, /truncated/)
  end

  it 'should raise an exception for malformed second byte' do
    lambda { @parser.parse(UTF8::Invalid::TWO_BYTES_MALFORMED_SECOND_BYTE) }.should raise_error(RangeError, /malformed/)
    lambda {
      @parser.parse('good text' + UTF8::Invalid::TWO_BYTES_MALFORMED_SECOND_BYTE)
    }.should raise_error(RangeError, /malformed/)
  end

  it 'should raise an exception for overlong sequence' do
    lambda { @parser.parse(UTF8::Invalid::OVERLONG) }.should raise_error(RangeError, /overlong/)
    lambda {
      @parser.parse('good text' + UTF8::Invalid::OVERLONG)
    }.should raise_error(RangeError, /overlong/)

    # alternate
    lambda { @parser.parse(UTF8::Invalid::OVERLONG_ALT) }.should raise_error(RangeError, /overlong/)
    lambda {
      @parser.parse('good text' + UTF8::Invalid::OVERLONG_ALT)
    }.should raise_error(RangeError, /overlong/)
  end

  it 'should raise an exception for missing second byte in three-byte sequence' do
    lambda { @parser.parse(UTF8::Invalid::THREE_BYTES_MISSING_SECOND_BYTE) }.should raise_error(RangeError, /truncated/)
    lambda {
      @parser.parse('good text' + UTF8::Invalid::THREE_BYTES_MISSING_SECOND_BYTE)
    }.should raise_error(RangeError, /truncated/)
  end

  it 'should raise an exception for missing third byte in three-byte sequence' do
    lambda { @parser.parse(UTF8::Invalid::THREE_BYTES_MISSING_THIRD_BYTE) }.should raise_error(RangeError, /truncated/)
    lambda {
      @parser.parse('good text' + UTF8::Invalid::THREE_BYTES_MISSING_THIRD_BYTE)
    }.should raise_error(RangeError, /truncated/)
  end

  it 'should raise an exception for malformed second byte in three-byte sequence' do
    lambda { @parser.parse(UTF8::Invalid::THREE_BYTES_MALFORMED_SECOND_BYTE) }.should raise_error(RangeError, /malformed/)
    lambda {
      @parser.parse('good text' + UTF8::Invalid::THREE_BYTES_MALFORMED_SECOND_BYTE)
    }.should raise_error(RangeError, /malformed/)
  end

  it 'should raise an exception for malformed third byte in three-byte sequence' do
    lambda { @parser.parse(UTF8::Invalid::THREE_BYTES_MALFORMED_THIRD_BYTE) }.should raise_error(RangeError, /malformed/)
    lambda {
      @parser.parse('good text' + UTF8::Invalid::THREE_BYTES_MALFORMED_THIRD_BYTE)
    }.should raise_error(RangeError, /malformed/)
  end

  it 'should raise an exception for characters outside of the encodable range of UCS-2' do
    lambda { @parser.parse(UTF8::Invalid::EXCEEDS_ENCODABLE_RANGE_FOR_UCS2) }.should raise_error(RangeError, /exceeds/)
    lambda {
      @parser.parse('good text' + UTF8::Invalid::EXCEEDS_ENCODABLE_RANGE_FOR_UCS2)
    }.should raise_error(RangeError, /exceeds/)
  end

  it 'should raise an exception for unexpected bytes' do
    lambda { @parser.parse(UTF8::Invalid::UNEXPECTED_BYTE) }.should raise_error(RangeError, /unexpected byte/)
    lambda {
      @parser.parse('good text' + UTF8::Invalid::UNEXPECTED_BYTE)
    }.should raise_error(RangeError, /unexpected byte/)
  end
end

# this stuff is implicitly tested above, but test it here explicitly anyway
describe Wikitext, 'converting from UTF-8 to UCS-2' do
  before do
    @a = 'a string containing only ASCII codepoints'
    @b = 'a string with non-ASCII codepoints: áàâäÁÀÂÄ€ñ'
  end

  it 'should complain if passed nil' do
    lambda { Wikitext.utf8_to_ucs2(nil) }.should raise_error
  end

  it 'should do nothing on zero-length input' do
    Wikitext.utf8_to_ucs2('').should == ''
  end

  it 'should get the same answer as iconv' do
    Wikitext.utf8_to_ucs2(@a).should == Iconv.iconv('UCS-2LE', 'UTF-8', @a).shift
    Wikitext.utf8_to_ucs2(@b).should == Iconv.iconv('UCS-2LE', 'UTF-8', @b).shift
  end

  it 'should be able to round-trip' do
    Wikitext.ucs2_to_utf8(Wikitext.utf8_to_ucs2(@a)).should == @a
    Wikitext.ucs2_to_utf8(Wikitext.utf8_to_ucs2(@b)).should == @b
  end

  # these tests intimately tied to the error messages because I want to carefully check all error pathways
  it 'should reject invalidly encoded input' do
    lambda { Wikitext.utf8_to_ucs2(UTF8::Invalid::TWO_BYTES_MISSING_SECOND_BYTE) }.should   raise_error(RangeError, /truncated/)
    lambda { Wikitext.utf8_to_ucs2(UTF8::Invalid::TWO_BYTES_MALFORMED_SECOND_BYTE) }.should raise_error(RangeError, /malformed/)
    lambda { Wikitext.utf8_to_ucs2(UTF8::Invalid::OVERLONG) }.should                        raise_error(RangeError, /overlong/)
    lambda { Wikitext.utf8_to_ucs2(UTF8::Invalid::OVERLONG_ALT) }.should                    raise_error(RangeError, /overlong/)
    lambda { Wikitext.utf8_to_ucs2(UTF8::Invalid::THREE_BYTES_MISSING_SECOND_BYTE) }.should raise_error(RangeError, /truncated/)
    lambda { Wikitext.utf8_to_ucs2(UTF8::Invalid::THREE_BYTES_MISSING_THIRD_BYTE) }.should  raise_error(RangeError, /truncated/)
    lambda { Wikitext.utf8_to_ucs2(UTF8::Invalid::THREE_BYTES_MALFORMED_SECOND_BYTE) }.should raise_error(RangeError, /malformed/)
    lambda { Wikitext.utf8_to_ucs2(UTF8::Invalid::THREE_BYTES_MALFORMED_THIRD_BYTE) }.should raise_error(RangeError, /malformed/)
    lambda { Wikitext.utf8_to_ucs2(UTF8::Invalid::EXCEEDS_ENCODABLE_RANGE_FOR_UCS2) }.should raise_error(RangeError, /exceeds/)
    lambda { Wikitext.utf8_to_ucs2(UTF8::Invalid::UNEXPECTED_BYTE) }.should raise_error(RangeError, /unexpected byte/)
  end
end

module UCS2
  module Invalid
    # invalid code points lie between 0xd800 and 0xdfff inclusive
    INVALID_CODE_POINT                = [0x00, 0xd9].pack('C*') # 0xdf40: written this way for little-endian systems
  end # module Invalid
end # module UCS2

# this stuff is implicitly tested above, but test it here explicitly anyway
describe Wikitext, 'converting from UCS-2 to UTF-8' do
  before do
    @a = Iconv.iconv('UCS-2LE', 'UTF-8', 'a string containing only ASCII codepoints').shift
    @b = Iconv.iconv('UCS-2LE', 'UTF-8', 'a string with non-ASCII codepoints: áàâäÁÀÂÄ€ñ').shift
  end

  it 'should complain if passed nil' do
    lambda { Wikitext.ucs2_to_utf8(nil) }.should raise_error
  end

  it 'should do nothing on zero-length input' do
    Wikitext.ucs2_to_utf8('').should == ''
  end

  it 'should get the same answer as iconv' do
    Wikitext.ucs2_to_utf8(@a).should == Iconv.iconv('UTF-8', 'UCS-2LE', @a).shift
    Wikitext.ucs2_to_utf8(@b).should == Iconv.iconv('UTF-8', 'UCS-2LE', @b).shift
  end

  it 'should be able to round-trip' do
    Wikitext.utf8_to_ucs2(Wikitext.ucs2_to_utf8(@a)).should == @a
    Wikitext.utf8_to_ucs2(Wikitext.ucs2_to_utf8(@b)).should == @b
  end

  # this test intimately tied to the error messages because I want to carefully check all error pathways
  it 'should reject invalidly encoded input' do
    lambda { Wikitext.ucs2_to_utf8(UCS2::Invalid::INVALID_CODE_POINT) }.should raise_error(RangeError, /code point not valid/)
  end
end
