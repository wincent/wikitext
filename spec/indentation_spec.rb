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

# this is the inverse of the dedent method in spec_helper.rb
# it's only in this file because it isn't needed anywhere else
def indent spaces, string
  string.gsub /^/, ' ' * spaces
end

describe Wikitext::Parser, 'indentation' do
  before do
    @parser         = Wikitext::Parser.new
    @input          = '* foo'
    @default_output = dedent <<-END
      <ul>
        <li>foo</li>
      </ul>
    END
  end

  it 'should default to no additional indentation' do
    @parser.parse('* foo').should == @default_output
  end

  it 'should add additional indentation as indicated by the "indent" option' do
    @parser.parse('* foo', :indent => 1).should == indent(1, @default_output)
    @parser.parse('* foo', :indent => 2).should == indent(2, @default_output)
    @parser.parse('* foo', :indent => 3).should == indent(3, @default_output)
    @parser.parse('* foo', :indent => 4).should == indent(4, @default_output)
    @parser.parse('* foo', :indent => 5).should == indent(5, @default_output)
    @parser.parse('* foo', :indent => 6).should == indent(6, @default_output)
  end

  it 'should complain if the "indent" option is nil' do
    lambda { @parser.parse('* foo', :indent => nil) }.should raise_error(TypeError)
  end

  it 'should complain if the "indent" options is not an integer' do
    lambda { @parser.parse('* foo', :indent => 'bar') }.should raise_error(TypeError)
    lambda { @parser.parse('* foo', :indent => /baz/) }.should raise_error(TypeError)
  end

  it 'should treat a negative "indent" as though it were zero' do
    @parser.parse('* foo', :indent => -4).should == @default_output
  end

  it 'should coerce a float "indent" into an integer' do
    @parser.parse('* foo', :indent => 0.0).should == @default_output
    @parser.parse('* foo', :indent => 2.0).should == <<-END
  <ul>
    <li>foo</li>
  </ul>
END
  end
end
