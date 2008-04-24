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

describe Wikitext::Parser, 'fulltext tokenizing' do
  before do
    @parser = Wikitext::Parser.new
  end

  it 'should default to a minimum fulltext token length of 3' do
    @parser.minimum_fulltext_token_length.should == 3
  end

  it 'should accept overrides of minimum fulltext token length at initialization time' do
    parser = Wikitext::Parser.new(:minimum_fulltext_token_length => 10)
    parser.minimum_fulltext_token_length.should == 10
    parser.fulltext_tokenize('short loooooooooong').should == ['loooooooooong']
  end

  it 'should return nil for nil input' do
    @parser.fulltext_tokenize(nil).should be_nil
  end

  it 'should return an empty array for empty string input' do
    @parser.fulltext_tokenize('').should == []
  end

  it 'should return an empty array for an input string that contains nothing tokenizable' do
    @parser.fulltext_tokenize('#!?()/&').should == []
  end

  it 'should tokenize simple words' do
    @parser.fulltext_tokenize('foo bar baz').should == ['foo', 'bar', 'baz']
  end

  it 'should omit tokens shorter than the minimum required length' do
    @parser.fulltext_tokenize('a b baz longer').should == ['baz', 'longer']
  end

  it 'should accept overrides of minimum length at parse time' do
    @parser.fulltext_tokenize('a bc baz longer', :minimum => 2).should == ['bc', 'baz', 'longer']
  end

  it 'should treat a minimum length of 0 as meaning "no minimum length"' do
    @parser.fulltext_tokenize('a bc baz longer', :minimum => 0).should == ['a', 'bc', 'baz', 'longer']
  end

  it 'should interpret a minimum length of nil as meaning "default minumum length" (3)' do
    @parser.minimum_fulltext_token_length = 10
    @parser.fulltext_tokenize('a bc baz longer', :minimum => nil).should == ['baz', 'longer']
  end

  it 'should tokenize URLs' do
    @parser.fulltext_tokenize('foo http://example.com/ bar').should == ['foo', 'http://example.com/', 'bar']
  end

  it 'should tokenize email addresses' do
    @parser.fulltext_tokenize('foo user@example.com bar').should == ['foo', 'user@example.com', 'bar']
  end

  it 'should ignore punctuation' do
    @parser.fulltext_tokenize("don't forget!").should == ['don', 'forget']
  end

  it 'should ignore non-ASCII' do
    # note that a search for "información lingüística" will still work, but might return some false positives
    @parser.fulltext_tokenize('buscando información lingüística').should == ['buscando', 'informaci', 'ling', 'stica']
  end

  it 'should ignore wikitext markup' do
    @parser.fulltext_tokenize("this <nowiki>that</nowiki> [[foo bar]]").should == ['this', 'that', 'foo', 'bar']
  end

  it 'should tokenize alphanumerics' do
    @parser.fulltext_tokenize("password99 2008").should == ['password99', '2008']
  end
end
