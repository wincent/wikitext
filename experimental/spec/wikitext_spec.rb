#!/usr/bin/env ruby
# Copyright 2008 Wincent Colaiuta
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

describe Wikitext, 'token_types method' do
  before do
    @tokens = Wikitext::token_types
  end

  it 'should report the available token types as a hash' do
    @tokens.should be_kind_of(Hash)
  end

  it 'should report token names as symbols and values as numbers' do
    @tokens.each do |k, v|
      k.should be_kind_of(Integer)
      v.should be_kind_of(Symbol)
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
    @types  = Wikitext::token_types
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
    @tokens[0].token_type.should == :printable
    @tokens[1].token_type.should == :end_of_file
  end

  it 'should tokenize strings containing multiple symbols' do
    @tokens = @parser.tokenize('foo http://example.com/')
    @tokens.length.should == 4
    @tokens[0].token_type.should == :printable
    @tokens[1].token_type.should == :space
    @tokens[2].token_type.should == :uri
    @tokens[3].token_type.should == :end_of_file
  end

  it 'should tokenize runs of printable characters as as single symbol' do
    @tokens = @parser.tokenize('foo')
    @tokens.length.should == 2
    @tokens[0].token_type.should    == :printable
    @tokens[0].line_start.should    == 1
    @tokens[0].column_start.should  == 1
    @tokens[0].line_stop.should     == 1
    @tokens[0].column_stop.should   == 4
    @tokens[1].token_type.should    == :end_of_file
  end

  it 'should tokenize END_OF_FILE tokens as zero-width tokens' do
    @tokens = @parser.tokenize('')
    @tokens.length.should == 1
    @tokens[0].token_type.should    == :end_of_file
    @tokens[0].line_start.should    == 1
    @tokens[0].column_start.should  == 1
    @tokens[0].line_stop.should     == 1
    @tokens[0].column_stop.should   == 1
  end
end
