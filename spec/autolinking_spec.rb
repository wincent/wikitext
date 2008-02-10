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

describe Wikitext::Parser, 'autolinking' do
  before do
    @parser = Wikitext::Parser.new
  end

  it 'should default to autolinking on' do
    @parser.autolink.should == true
  end

  describe 'on' do
    it 'should convert HTTP URIs into hyperlinks' do
      uri = 'http://example.com/'
      @parser.parse(uri).should == %Q{<p><a href="http://example.com/" class="external">http://example.com/</a></p>\n}
    end

    it 'should convert HTTPS URIs into hyperlinks' do
      pending
      uri = 'https://example.com/'
      @parser.parse(uri).should == %Q{<p><a href="https://example.com/" class="external">https://example.com/</a></p>\n}
    end

    it 'should convert FTP URIs into hyperlinks' do
      uri = 'ftp://example.com/'
      @parser.parse(uri).should == %Q{<p><a href="ftp://example.com/" class="external">ftp://example.com/</a></p>\n}
    end

    it 'should convert mailto URIs into hyperlinks' do
      uri = 'mailto:user@example.com'
      @parser.parse(uri).should == %Q{<p><a href="mailto:user@example.com" class="external">mailto:user@example.com</a></p>\n}
    end

    it 'should convert SVN URIs into hyperlinks' do
      uri = 'svn://example.com/'
      @parser.parse(uri).should == %Q{<p><a href="svn://example.com/" class="external">svn://example.com/</a></p>\n}
    end

    it 'should apple the external_link_class CSS class if set' do
      uri = 'http://example.com/'
      @parser.external_link_class = 'bar'
      @parser.parse(uri).should == %Q{<p><a href="http://example.com/" class="bar">http://example.com/</a></p>\n}
    end

    it 'should apply no CSS if external_link_class is set to nil' do
      uri = 'http://example.com/'
      @parser.external_link_class = nil
      @parser.parse(uri).should == %Q{<p><a href="http://example.com/">http://example.com/</a></p>\n}
    end

    it 'should pass through URIs unchanged inside <nowiki></nowiki> spans' do
      @parser.parse("<nowiki>http://example.com/</nowiki>").should == "<p>http://example.com/</p>\n"
    end

    it 'should autolink URIs inside <pre></pre> spans' do
      input     = ' http://example.com/'
      expected  = %Q{<pre><a href="http://example.com/" class="external">http://example.com/</a></pre>\n}
      @parser.parse(input).should == expected
      @parser.external_link_class = nil
      expected  = %Q{<pre><a href="http://example.com/">http://example.com/</a></pre>\n}
      @parser.parse(input).should == expected
    end

    it 'should convert emails into hyperlinks' do
      uri = 'user@example.com'
      @parser.parse(uri).should == %Q{<p><a href="mailto:user@example.com" class="mailto">user@example.com</a></p>\n}
    end

    it 'should apply the mailto CSS class if set' do
      uri = 'user@example.com'
      @parser.mailto_class = 'foo'
      @parser.parse(uri).should == %Q{<p><a href="mailto:user@example.com" class="foo">user@example.com</a></p>\n}
    end

    it 'should apply no CSS if the mailto class is set to nil' do
      uri = 'user@example.com'
      @parser.mailto_class = nil
      @parser.parse(uri).should == %Q{<p><a href="mailto:user@example.com">user@example.com</a></p>\n}
    end

    it 'should pass through emails unchanged inside <nowiki></nowiki> spans' do
      @parser.parse("<nowiki>user@example.com</nowiki>").should == "<p>user@example.com</p>\n"  # was a crasher
    end

    it 'should pass through emails unchanged inside <pre></pre> blocks' do
      @parser.parse(" user@example.com").should == "<pre>user@example.com</pre>\n"  # was a crasher
    end
  end

  describe 'off' do
    before do
      @parser.autolink = false
    end

    it 'should accept "autolink = false"' do
      @parser.autolink.should == false
    end

    it 'should not convert URIs into hyperlinks' do
      @parser.parse('http://example.com/').should == "<p>http://example.com/</p>\n"
    end

    it 'should not convert emails into hyperlinks' do
      @parser.parse('user@example.com').should == "<p>user@example.com</p>\n"
    end
  end
end
