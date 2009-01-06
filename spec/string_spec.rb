#!/usr/bin/env ruby
# Copyright 2009 Wincent Colaiuta
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
require 'wikitext/string'

describe String, 'wikitext extensions' do
  it 'should provide a to_wikitext method on all strings' do
    "'''strong'''".to_wikitext.should == "<p><strong>strong</strong></p>\n"
  end

  it 'should provide a w method on all strings' do
    "'''strong'''".w.should == "<p><strong>strong</strong></p>\n"
  end

  it 'should provide access to the shared parser instance for customization' do
    String.shared_wikitext_parser.should be_instance_of(Wikitext::Parser)
  end

  it 'should persist customizations made to shared parser instance' do
    # fisrt pass
    parser                   = String.shared_wikitext_parser
    original_prefix          = '/images/'
    parser.img_prefix        = original_prefix
    parser.img_prefix.should == original_prefix
    '{{foo.png}}'.w.should   == %Q{<p><img src="/images/foo.png" alt="foo.png" /></p>\n}

    # second pass
    new_prefix               = '/totally-different-prefix/'
    parser.img_prefix        = new_prefix
    parser.img_prefix.should == new_prefix
    '{{bar.png}}'.w.should   == %Q{<p><img src="/totally-different-prefix/bar.png" alt="bar.png" /></p>\n}
  end
end
