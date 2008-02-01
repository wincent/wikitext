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
end
