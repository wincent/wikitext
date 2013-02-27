# Copyright 2007-2013 Wincent Colaiuta. All rights reserved.
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

require 'spec_helper'

describe Wikitext, 'sanitizing a link target' do
  it 'should complain if passed nil' do
    lambda { Wikitext::Parser.sanitize_link_target(nil) }.should raise_error
  end

  it 'should complain if passed <' do
    lambda { Wikitext::Parser.sanitize_link_target('<') }.should raise_error(RangeError, /</)
  end

  it 'should complain if passed >' do
    lambda { Wikitext::Parser.sanitize_link_target('>') }.should raise_error(RangeError, />/)
  end

  it 'should do nothing on zero-length input' do
    Wikitext::Parser.sanitize_link_target('').should == ''
  end

  it 'should do nothing to embedded spaces' do
    Wikitext::Parser.sanitize_link_target('hello world').should == 'hello world'
  end

  it 'should eat leading spaces' do
    Wikitext::Parser.sanitize_link_target(' hello world').should == 'hello world'
    Wikitext::Parser.sanitize_link_target('  hello world').should == 'hello world'
    Wikitext::Parser.sanitize_link_target('   hello world').should == 'hello world'
    Wikitext::Parser.sanitize_link_target('    hello world').should == 'hello world'
    Wikitext::Parser.sanitize_link_target('     hello world').should == 'hello world'
    Wikitext::Parser.sanitize_link_target('      hello world').should == 'hello world'
  end

  it 'should eat trailing spaces' do
    Wikitext::Parser.sanitize_link_target('hello world ').should == 'hello world'
    Wikitext::Parser.sanitize_link_target('hello world  ').should == 'hello world'
    Wikitext::Parser.sanitize_link_target('hello world   ').should == 'hello world'
    Wikitext::Parser.sanitize_link_target('hello world    ').should == 'hello world'
    Wikitext::Parser.sanitize_link_target('hello world     ').should == 'hello world'   # was a crasher
    Wikitext::Parser.sanitize_link_target('hello world      ').should == 'hello world'  # was a crasher

    # same but with lots of entities to force a reallocation (we were crashing under reallocation)
    expected = '&quot;&quot;&quot;&quot;&quot;&quot;&quot;&quot;&quot;&quot;'
    Wikitext::Parser.sanitize_link_target('""""""""""      ').should == expected
  end

  it 'should eat leading and trailing spaces combined' do
    Wikitext::Parser.sanitize_link_target(' hello world     ').should == 'hello world'
    Wikitext::Parser.sanitize_link_target('  hello world    ').should == 'hello world'
    Wikitext::Parser.sanitize_link_target('   hello world   ').should == 'hello world'
    Wikitext::Parser.sanitize_link_target('    hello world  ').should == 'hello world'
    Wikitext::Parser.sanitize_link_target('     hello world  ').should == 'hello world'
    Wikitext::Parser.sanitize_link_target('      hello world ').should == 'hello world'
  end

  it 'should return nothing for input consisting entirely of spaces' do
    Wikitext::Parser.sanitize_link_target(' ').should == ''
    Wikitext::Parser.sanitize_link_target('  ').should == ''
    Wikitext::Parser.sanitize_link_target('   ').should == ''
    Wikitext::Parser.sanitize_link_target('    ').should == ''
    Wikitext::Parser.sanitize_link_target('     ').should == ''
    Wikitext::Parser.sanitize_link_target('      ').should == ''
  end

  it 'should convert double quotes into named entities' do
    Wikitext::Parser.sanitize_link_target('hello "world"').should == 'hello &quot;world&quot;'
  end

  it 'should convert ampersands into named entities' do
    Wikitext::Parser.sanitize_link_target('hello & goodbye').should == 'hello &amp; goodbye'
  end

  it 'should convert non-ASCII hexadecimal entities' do
    Wikitext::Parser.sanitize_link_target('cañon').should == 'ca&#x00f1;on'
  end

  it 'should handle mixed scenarios (ampersands, double-quotes and non-ASCII)' do
    Wikitext::Parser.sanitize_link_target('foo, "bar" & baz €').should == 'foo, &quot;bar&quot; &amp; baz &#x20ac;'
  end

  # here we're exercising the wiki_utf8_to_utf32 function
  describe 'with invalidly encoded input' do
    it 'should raise an exception for missing second byte' do
      lambda {
        Wikitext::Parser.sanitize_link_target(UTF8::Invalid::TWO_BYTES_MISSING_SECOND_BYTE)
      }.should raise_error(Wikitext::Parser::Error, /truncated/)
      lambda {
        Wikitext::Parser.sanitize_link_target('good text' + UTF8::Invalid::TWO_BYTES_MISSING_SECOND_BYTE)
      }.should raise_error(Wikitext::Parser::Error, /truncated/)
    end

    it 'should raise an exception for malformed second byte' do
      lambda {
        Wikitext::Parser.sanitize_link_target(UTF8::Invalid::TWO_BYTES_MALFORMED_SECOND_BYTE)
      }.should raise_error(Wikitext::Parser::Error, /malformed/)
      lambda {
        Wikitext::Parser.sanitize_link_target('good text' + UTF8::Invalid::TWO_BYTES_MALFORMED_SECOND_BYTE)
      }.should raise_error(Wikitext::Parser::Error, /malformed/)
    end

    it 'should raise an exception for overlong sequence' do
      lambda {
        Wikitext::Parser.sanitize_link_target(UTF8::Invalid::OVERLONG)
      }.should raise_error(Wikitext::Parser::Error, /overlong/)
      lambda {
        Wikitext::Parser.sanitize_link_target('good text' + UTF8::Invalid::OVERLONG)
      }.should raise_error(Wikitext::Parser::Error, /overlong/)

      # alternate
      lambda {
        Wikitext::Parser.sanitize_link_target(UTF8::Invalid::OVERLONG_ALT)
      }.should raise_error(Wikitext::Parser::Error, /overlong/)
      lambda {
        Wikitext::Parser.sanitize_link_target('good text' + UTF8::Invalid::OVERLONG_ALT)
      }.should raise_error(Wikitext::Parser::Error, /overlong/)
    end

    it 'should raise an exception for missing second byte in three-byte sequence' do
      lambda {
        Wikitext::Parser.sanitize_link_target(UTF8::Invalid::THREE_BYTES_MISSING_SECOND_BYTE)
      }.should raise_error(Wikitext::Parser::Error, /truncated/)
      lambda {
        Wikitext::Parser.sanitize_link_target('good text' + UTF8::Invalid::THREE_BYTES_MISSING_SECOND_BYTE)
      }.should raise_error(Wikitext::Parser::Error, /truncated/)
    end

    it 'should raise an exception for missing third byte in three-byte sequence' do
      lambda {
        Wikitext::Parser.sanitize_link_target(UTF8::Invalid::THREE_BYTES_MISSING_THIRD_BYTE)
      }.should raise_error(Wikitext::Parser::Error, /truncated/)
      lambda {
        Wikitext::Parser.sanitize_link_target('good text' + UTF8::Invalid::THREE_BYTES_MISSING_THIRD_BYTE)
      }.should raise_error(Wikitext::Parser::Error, /truncated/)
    end

    it 'should raise an exception for malformed second byte in three-byte sequence' do
      lambda {
        Wikitext::Parser.sanitize_link_target(UTF8::Invalid::THREE_BYTES_MALFORMED_SECOND_BYTE)
      }.should raise_error(Wikitext::Parser::Error, /malformed/)
      lambda {
        Wikitext::Parser.sanitize_link_target('good text' + UTF8::Invalid::THREE_BYTES_MALFORMED_SECOND_BYTE)
      }.should raise_error(Wikitext::Parser::Error, /malformed/)
    end

    it 'should raise an exception for malformed third byte in three-byte sequence' do
      lambda {
        Wikitext::Parser.sanitize_link_target(UTF8::Invalid::THREE_BYTES_MALFORMED_THIRD_BYTE)
      }.should raise_error(Wikitext::Parser::Error, /malformed/)
      lambda {
        Wikitext::Parser.sanitize_link_target('good text' + UTF8::Invalid::THREE_BYTES_MALFORMED_THIRD_BYTE)
      }.should raise_error(Wikitext::Parser::Error, /malformed/)
    end

    it 'should raise an exception for missing second byte in four-byte sequence' do
      lambda {
        Wikitext::Parser.sanitize_link_target(UTF8::Invalid::FOUR_BYTES_MISSING_SECOND_BYTE)
      }.should raise_error(Wikitext::Parser::Error, /truncated/)
      lambda {
        Wikitext::Parser.sanitize_link_target('good text' + UTF8::Invalid::FOUR_BYTES_MISSING_SECOND_BYTE)
      }.should raise_error(Wikitext::Parser::Error, /truncated/)
    end

    it 'should raise an exception for missing third byte in four-byte sequence' do
      lambda {
        Wikitext::Parser.sanitize_link_target(UTF8::Invalid::FOUR_BYTES_MISSING_THIRD_BYTE)
      }.should raise_error(Wikitext::Parser::Error, /truncated/)
      lambda {
        Wikitext::Parser.sanitize_link_target('good text' + UTF8::Invalid::FOUR_BYTES_MISSING_THIRD_BYTE)
      }.should raise_error(Wikitext::Parser::Error, /truncated/)
    end

    it 'should raise an exception for missing fourth byte in four-byte sequence' do
      lambda {
        Wikitext::Parser.sanitize_link_target(UTF8::Invalid::FOUR_BYTES_MISSING_FOURTH_BYTE)
      }.should raise_error(Wikitext::Parser::Error, /truncated/)
      lambda {
        Wikitext::Parser.sanitize_link_target('good text' + UTF8::Invalid::FOUR_BYTES_MISSING_FOURTH_BYTE)
      }.should raise_error(Wikitext::Parser::Error, /truncated/)
    end

    it 'should raise an exception for illegal first byte in four-byte sequence' do
      lambda {
        Wikitext::Parser.sanitize_link_target(UTF8::Invalid::FOUR_BYTES_ILLEGAL_FIRST_BYTE)
      }.should raise_error(Wikitext::Parser::Error, /overlong/)
      lambda {
        Wikitext::Parser.sanitize_link_target('good text' + UTF8::Invalid::FOUR_BYTES_ILLEGAL_FIRST_BYTE)
      }.should raise_error(Wikitext::Parser::Error, /overlong/)

      lambda {
        Wikitext::Parser.sanitize_link_target(UTF8::Invalid::FOUR_BYTES_ILLEGAL_FIRST_BYTE_ALT)
      }.should raise_error(Wikitext::Parser::Error, /overlong/)
      lambda {
        Wikitext::Parser.sanitize_link_target('good text' + UTF8::Invalid::FOUR_BYTES_ILLEGAL_FIRST_BYTE_ALT)
      }.should raise_error(Wikitext::Parser::Error, /overlong/)

      lambda {
        Wikitext::Parser.sanitize_link_target(UTF8::Invalid::FOUR_BYTES_ILLEGAL_FIRST_BYTE_ALT2)
      }.should raise_error(Wikitext::Parser::Error, /overlong/)
      lambda {
        Wikitext::Parser.sanitize_link_target('good text' + UTF8::Invalid::FOUR_BYTES_ILLEGAL_FIRST_BYTE_ALT2)
      }.should raise_error(Wikitext::Parser::Error, /overlong/)
    end

    it 'should raise an exception for unexpected bytes' do
      lambda {
        Wikitext::Parser.sanitize_link_target(UTF8::Invalid::UNEXPECTED_BYTE)
      }.should raise_error(Wikitext::Parser::Error, /unexpected/)
      lambda {
        Wikitext::Parser.sanitize_link_target('good text' + UTF8::Invalid::UNEXPECTED_BYTE)
      }.should raise_error(Wikitext::Parser::Error, /unexpected/)
    end
  end
end
