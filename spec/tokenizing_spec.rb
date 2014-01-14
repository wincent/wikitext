# Copyright 2008-2014 Wincent Colaiuta. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
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

describe Wikitext::Parser::Token do
  before do
    @tokens = Wikitext::Parser::Token.types
  end

  it 'should report the available token types as a hash' do
    @tokens.should be_kind_of(Hash)
  end

  it 'should report token names as symbols and values as numbers' do
    @tokens.each do |k, v|
      v.should be_kind_of(Symbol)
      k.should be_kind_of(Integer)
    end
  end

  it 'should report unique token names and values' do
    keys = @tokens.keys
    keys.uniq.length.should == keys.length
    values = @tokens.values
    values.uniq.length.should == values.length
  end
end

describe Wikitext::Parser, 'tokenizing' do
  before do
    @parser = Wikitext::Parser.new
  end

  it 'should do nothing if passed nil' do
    @parser.tokenize(nil).should == nil
  end

  it "should complain if passed an object that doesn't quack like a string" do
    lambda { @parser.tokenize({}) }.should raise_error
  end

  it 'should tokenize strings containing a single symbol' do
    @tokens = @parser.tokenize('foo')
    @tokens.length.should == 2
    @tokens[0].token_type.should    == :alnum
    @tokens[0].string_value.should  == 'foo'
    @tokens[1].token_type.should    == :end_of_file
    @tokens[1].string_value.should  == ''
  end

  it 'should tokenize strings containing multiple symbols' do
    @tokens = @parser.tokenize('foo http://example.com/')
    @tokens.length.should == 4
    @tokens[0].token_type.should    == :alnum
    @tokens[0].string_value.should  == 'foo'
    @tokens[1].token_type.should    == :space
    @tokens[1].string_value.should  == ' '
    @tokens[2].token_type.should    == :uri
    @tokens[2].string_value.should  == 'http://example.com/'
    @tokens[3].token_type.should    == :end_of_file
    @tokens[3].string_value.should  == ''
  end

  it 'should tokenize runs of printable characters as as single symbol' do
    @tokens = @parser.tokenize('foo')
    @tokens.length.should == 2
    @tokens[0].token_type.should    == :alnum
    @tokens[0].string_value.should  == 'foo'
    @tokens[0].line_start.should    == 1
    @tokens[0].column_start.should  == 1
    @tokens[0].line_stop.should     == 1
    @tokens[0].column_stop.should   == 4
    @tokens[1].token_type.should    == :end_of_file
    @tokens[1].string_value.should  == ''
  end

  it 'should tokenize END_OF_FILE tokens as zero-width tokens' do
    @tokens = @parser.tokenize('')
    @tokens.length.should == 1
    @tokens[0].token_type.should    == :end_of_file
    @tokens[0].line_start.should    == 1
    @tokens[0].column_start.should  == 1
    @tokens[0].line_stop.should     == 1
    @tokens[0].column_stop.should   == 1
    @tokens[0].string_value.should  == ''
  end

  it 'should be able to tokenize strings containing "}"' do
    # was a bug: we were throwing an exception "failed before finding a token" because our PRINTABLE rule omitted this code point
    # later on the "}" became the RIGHT_CURLY token, so the spec has been modified accordingly
    lambda { @tokens = @parser.tokenize('}') }.should_not raise_error
    @tokens.length.should == 2
    @tokens[0].token_type.should    == :right_curly # was PRINTABLE
    @tokens[0].string_value.should  == '}'
    @tokens[0].line_start.should    == 1
    @tokens[0].column_start.should  == 1
    @tokens[0].line_stop.should     == 1
    @tokens[0].column_stop.should   == 2
    @tokens[1].token_type.should    == :end_of_file
    @tokens[1].string_value.should  == ''
  end

  it 'should be able to tokenize the full range of printable ASCII' do
    # see the previous example: we just want to make sure that our PRINTABLE rule is adequate
    printable_ascii = (0x20..0x7e).to_a.pack('C*')
    lambda { @parser.tokenize(printable_ascii) }.should_not raise_error
  end

  it 'should be able to tokenize large blocks of text' do
    large_block_of_text = dedent <<-END
      paragraph
      second line
      
      new paragraph
      
      = a heading =
      
      > a blockquote
      > second line of blockquote
      >
      > new paragraph within blockquote
      
      == another heading ==
      
      paragraph within ''multiple '''styles''''' and <tt>tt span</tt>
      
      similar, but with '''styles in ''different'' order'''
      
      again, a '''different ''order'''''
      
      * list item 1
      ** nested list item 1
      ** nested list item 2
      ** nested list item 3
      * list item 2
      
       // this is a code block
       notice how it can contain ''markup''
       which would '''otherwise''' have <tt>special</tt> meaning
       although explicit entities &copy; are passed through unchanged
      
      a normal paragraph again
      
      This is where we show a link to an article on [[GCC]].
      Related to that, [[GCC|a link]] to the same
      article but with custom link text.
      
      External links [http://example.com work too].
      As well as autolinks as seen http://example.com/
      here.
      
      Look at how we handle bad syntax. [[This is an unterminated
      link. And [http://example.com/ is another.
      
      # this is an ordered list
      # which continues
      ## and has another ordered list
      ## nested inside it
      # and then falls back
      #* and then nests another list
      #* this time an unordered one
      #** itself containing a nested list
      #** which continues
      #**# and finally nests yet another ordered list
      #**# which continues
      #* drops back quite a way
      # and finally all the way
      #****** and finishes with an invalid item
      
      === heading with missing closing tag
      * list
      # new list
    END
    @tokens = @parser.tokenize(large_block_of_text)
    @tokens.length.should > 0
  end
end
