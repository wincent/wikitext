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

describe Wikitext::Parser, 'parsing entities' do
  before do
    @parser = Wikitext::Parser.new
  end

  it 'should pass numeric (decimal) entities through unchanged' do
    @parser.parse('&#8364;').should == "<p>&#8364;</p>\n"
  end

  it 'should normalize case variations in hexadecimal entities' do
    @parser.parse('&#x20ac;').should == "<p>&#x20ac;</p>\n"
    @parser.parse('&#x20AC;').should == "<p>&#x20ac;</p>\n"
    @parser.parse('&#X20ac;').should == "<p>&#x20ac;</p>\n"
    @parser.parse('&#X20AC;').should == "<p>&#x20ac;</p>\n"
  end

  it 'should pass named entities through unchanged' do
    @parser.parse('&Aacute;').should == "<p>&Aacute;</p>\n" # these are two different entities
    @parser.parse('&aacute;').should == "<p>&aacute;</p>\n" # ie. they are case sensitive
    @parser.parse('&euro;').should == "<p>&euro;</p>\n"
  end
end
