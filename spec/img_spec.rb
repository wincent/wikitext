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

describe Wikitext::Parser, 'embedding img tags' do
  before do
    @parser = Wikitext::Parser.new
  end

  it 'should convert valid markup into inline image tags' do
    @parser.parse('{{foo.png}}').should == %Q{<p><img src="/images/foo.png" alt="foo.png" /></p>\n}
  end

  it 'should appear embedded in an inline flow' do
    @parser.parse('before {{foo.png}} after').should == %Q{<p>before <img src="/images/foo.png" alt="foo.png" /> after</p>\n}
  end

  it 'should pass single curly braces through unaltered' do
    @parser.parse('{foo.png}').should == %Q{<p>{foo.png}</p>\n}
  end

  it 'should have no effect inside PRE blocks' do
    @parser.parse(' {{foo.png}}').should == %Q{<pre>{{foo.png}}</pre>\n}
  end

  it 'should have no effect inside PRE_START blocks' do
    @parser.parse('<pre>{{foo.png}}</pre>').should == %Q{<pre>{{foo.png}}</pre>\n}
  end

  it 'should have no effect inside NO_WIKI spans' do
    @parser.parse('<nowiki>{{foo.png}}</nowiki>').should == %Q{<p>{{foo.png}}</p>\n}
  end

  it 'should be passed through in internal link targets' do
    @parser.parse('[[{{foo.png}}]]').should == %Q{<p><a href="/wiki/%7b%7bfoo.png%7d%7d">{{foo.png}}</a></p>\n}
  end

  it 'should be passed through in internal link text' do
    @parser.parse('[[article|{{foo.png}}]]').should == %Q{<p><a href="/wiki/article">{{foo.png}}</a></p>\n}
  end

  it 'should not be allowed as an external link target' do
    expected = %Q{<p>[<img src="/images/foo.png" alt="foo.png" /> the link]</p>\n}
    @parser.parse('[{{foo.png}} the link]').should == expected
  end

  it 'should be passed through in external link text' do
    expected = %Q{<p><a href="http://example.com/" class="external">{{foo.png}}</a></p>\n}
    @parser.parse('[http://example.com/ {{foo.png}}]').should == expected
  end

  it 'should not allow embedded quotes' do
    @parser.parse('{{"fun".png}}').should == %Q{<p>{{&quot;fun&quot;.png}}</p>\n}
  end

  it 'should not allow embedded spaces' do
    @parser.parse('{{foo bar.png}}').should == %Q{<p>{{foo bar.png}}</p>\n}
  end

  it 'should not allow characters beyond printable ASCII' do
    @parser.parse('{{500€.png}}').should == %Q{<p>{{500&#x20ac;.png}}</p>\n}
  end

  it 'should allow overrides of the image prefix at initialization time' do
    parser = Wikitext::Parser.new(:img_prefix => '/gfx/')
    parser.parse('{{foo.png}}').should == %Q{<p><img src="/gfx/foo.png" alt="foo.png" /></p>\n}
  end

  it 'should suppress the image prefix if passed an empty string at initialization time' do
    parser = Wikitext::Parser.new(:img_prefix => '')
    parser.parse('{{foo.png}}').should == %Q{<p><img src="foo.png" alt="foo.png" /></p>\n}
  end

  it 'should suppress image prefix if passed nil at initialization time' do
    parser = Wikitext::Parser.new(:img_prefix => nil)
    parser.parse('{{foo.png}}').should == %Q{<p><img src="foo.png" alt="foo.png" /></p>\n}
  end

  it 'should allow overrides of the image prefix after initialization' do
    @parser.img_prefix = '/gfx/'
    @parser.parse('{{foo.png}}').should == %Q{<p><img src="/gfx/foo.png" alt="foo.png" /></p>\n}
  end

  it 'should suppress image if prefix set to an empty string after initialization' do
    @parser.img_prefix = ''
    @parser.parse('{{foo.png}}').should == %Q{<p><img src="foo.png" alt="foo.png" /></p>\n}
  end

  it 'should suppress image if prefix set to nil after initialization' do
    @parser.img_prefix = nil
    @parser.parse('{{foo.png}}').should == %Q{<p><img src="foo.png" alt="foo.png" /></p>\n}
  end
end
