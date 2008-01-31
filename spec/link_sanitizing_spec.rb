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

describe Wikitext, 'sanitizing a link target' do
  it 'should complain if passed nil' do
    lambda { Wikitext.sanitize_link_target(nil) }.should raise_error
  end

  it 'should complain if passed <' do
    lambda { Wikitext.sanitize_link_target('<') }.should raise_error(RangeError, /</)
  end

  it 'should complain if passed >' do
    lambda { Wikitext.sanitize_link_target('>') }.should raise_error(RangeError, />/)
  end

  it 'should do nothing on zero-length input' do
    Wikitext.sanitize_link_target('').should == ''
  end

  it 'should do nothing to spaces' do
    Wikitext.sanitize_link_target('hello world').should == 'hello world'
  end

  it 'should convert double quotes into named entities' do
    Wikitext.sanitize_link_target('hello "world"').should == 'hello &quot;world&quot;'
  end

  it 'should convert ampersands into named entities' do
    Wikitext.sanitize_link_target('hello & goodbye').should == 'hello &amp; goodbye'
  end

  it 'should convert non-ASCII hexadecimal entities' do
    Wikitext.sanitize_link_target('cañon').should == 'ca&#x00f1;on'
  end

  it 'should handle mixed scenarios (ampersands, double-quotes and non-ASCII)' do
    Wikitext.sanitize_link_target('foo, "bar" & baz €').should == 'foo, &quot;bar&quot; &amp; baz &#x20ac;'
  end
end
