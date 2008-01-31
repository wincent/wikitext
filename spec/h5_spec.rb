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

describe Wikitext::Parser, 'parsing <h5> blocks' do
  before do
    @parser = Wikitext::Parser.new
  end

  it 'should recognize paired <h5> and </h5> tags' do
    @parser.parse('=====foo=====').should == "<h5>foo</h5>\n"
  end

  it 'should strip leading and trailing whitespace from the title' do
    @parser.parse('===== foo =====').should == "<h5>foo</h5>\n"
    @parser.parse('=====  foo  =====').should == "<h5>foo</h5>\n"
    @parser.parse('=====   foo   =====').should == "<h5>foo</h5>\n"
  end

  it 'should accept titles with missing closing tags' do
    @parser.parse('===== foo').should == "<h5>foo</h5>\n"
  end

  it 'should allow header tags to appear within titles' do
    @parser.parse('===== foo = bar =====').should == "<h5>foo = bar</h5>\n"
    @parser.parse('===== foo == bar =====').should == "<h5>foo == bar</h5>\n"
    @parser.parse('===== foo === bar =====').should == "<h5>foo === bar</h5>\n"
    @parser.parse('===== foo ==== bar =====').should == "<h5>foo ==== bar</h5>\n"
    @parser.parse('===== foo ===== bar =====').should == "<h5>foo ===== bar</h5>\n"
    @parser.parse('===== foo ====== bar =====').should == "<h5>foo ====== bar</h5>\n"
  end

  it 'should show excess characters in closing tags' do
    # the visual feedback alerts the user to the error
    @parser.parse('===== foo ======').should == "<h5>foo ======</h5>\n"
  end

  it 'should be nestable inside blockquote blocks' do
    @parser.parse('> ===== foo =====').should == "<blockquote><h5>foo</h5>\n</blockquote>\n"
  end

  it 'should have no special meaning inside <nowiki> spans' do
    @parser.parse("<nowiki>\n===== foo =====</nowiki>").should == "<p>\n===== foo =====</p>\n"
  end
end
