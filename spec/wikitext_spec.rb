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

describe Wikitext::Parser do
  before do
    @parser = Wikitext::Parser.new
  end

  it 'should autolink by default' do
    @parser.autolink.should == true
  end

  it 'should use linefeed as default line ending' do
    @parser.line_ending.should == "\n"
  end

  it 'should use "external" as default external link class' do
    @parser.external_link_class.should == 'external'
  end

  it 'should use "mailto" as default mailto class' do
    @parser.mailto_class.should == 'mailto'
  end

  it 'should use "/wiki/" as default internal link prefix' do
    @parser.internal_link_prefix.should == '/wiki/'
  end

  it 'should use "/images/" as default img prefix' do
    @parser.img_prefix.should == '/images/'
  end

  it 'should turn space-to-underscore off by default' do
    @parser.space_to_underscore.should == false
  end

  it 'should treat slash as special by default' do
    @parser.treat_slash_as_special.should == true
  end

  describe 'overriding defaults at initialization time' do
    it 'should allow overriding of autolink' do
      Wikitext::Parser.new(:autolink => false).autolink.should == false
    end

    it 'should allow overriding of line ending' do
      Wikitext::Parser.new(:line_ending => "\r").line_ending.should == "\r"
    end

    it 'should allow overriding of external link class' do
      Wikitext::Parser.new(:external_link_class => 'foo').external_link_class.should == 'foo'
    end

    it 'should allow overriding of mailto class' do
      Wikitext::Parser.new(:mailto_class => 'bar').mailto_class.should == 'bar'
    end

    it 'should allow overriding of internal link prefix' do
      Wikitext::Parser.new(:internal_link_prefix => '/baz/').internal_link_prefix.should == '/baz/'
    end

    it 'should allow overriding of the img prefix' do
      Wikitext::Parser.new(:img_prefix => '/bar/').img_prefix.should == '/bar/'
      Wikitext::Parser.new(:img_prefix => '').img_prefix.should == ''
      Wikitext::Parser.new(:img_prefix => nil).img_prefix.should == nil
    end

    it 'should allow overriding of space-to-underscore' do
      Wikitext::Parser.new(:space_to_underscore => true).space_to_underscore.should == true
    end

    it 'should allow overriding of treat slash as special' do
      Wikitext::Parser.new(:treat_slash_as_special => false).treat_slash_as_special.should == false
    end
  end
end

describe Wikitext::Parser, 'parsing non-ASCII input' do
  before do
    @parser = Wikitext::Parser.new
  end

  it 'should convert non-ASCII characters to numeric entities' do
    @parser.parse('€').should == "<p>&#x20ac;</p>\n"
  end
end

describe Wikitext::Parser, 'parsing characters which have special meaning in HTML' do
  before do
    @parser = Wikitext::Parser.new
  end

  it 'should convert "<" into the corresponding named entity' do
    @parser.parse('<').should == "<p>&lt;</p>\n"
  end

  it 'should convert ">" into the corresponding named entity' do
    # can't put ">" in the first column as that would indicate a blockquote
    @parser.parse("foo >").should == "<p>foo &gt;</p>\n"
  end

  it 'should convert "&" into the corresponding named entity' do
    @parser.parse('&').should == "<p>&amp;</p>\n"
  end

  it 'should convert \'"\' into the corresponding named entity' do
    @parser.parse('"').should == "<p>&quot;</p>\n"
  end
end
