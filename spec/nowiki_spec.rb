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

describe Wikitext::Parser, 'parsing <nowiki> spans' do
  before do
    @parser = Wikitext::Parser.new
  end

  it 'should not echo paired <nowiki> and </nowiki> tags' do
    @parser.parse('foo <nowiki>bar</nowiki> baz').should == "<p>foo bar baz</p>\n"
  end

  it 'should automatically handle missing closing tags (at end of outpt)' do
    # note that CRLF doesn't auto-terminate a <nowiki> span, unlike other inline spans
    @parser.parse('foo <nowiki>bar').should == "<p>foo bar</p>\n"
  end

  it 'should unconditionally echo newlines inside <nowiki> spans' do
    @parser.parse("<nowiki>foo\nbar</nowiki>").should == "<p>foo\nbar</p>\n"
    @parser.parse("<nowiki>foo\n\nbar</nowiki>").should == "<p>foo\n\nbar</p>\n"
    @parser.parse("<nowiki>foo\n\n\nbar</nowiki>").should == "<p>foo\n\n\nbar</p>\n"
  end

  it 'should convert unexpected closing tags into entities' do
    @parser.parse('foo </nowiki>bar').should == "<p>foo &lt;/nowiki&gt;bar</p>\n"
  end

  it 'should convert nested opening tags into entities' do
    @parser.parse('<nowiki>foo<nowiki>bar</nowiki>baz').should == "<p>foo&lt;nowiki&gt;barbaz</p>\n"
  end

  it 'should have no effect inside <pre> blocks' do
    @parser.parse(' <nowiki>foo</nowiki>').should == "<pre>&lt;nowiki&gt;foo&lt;/nowiki&gt;</pre>\n"
  end

  it 'should pass short BLOCKQUOTE tokens through without any special meaning' do
    @parser.parse("<nowiki>\n></nowiki>").should == "<p>\n&gt;</p>\n"
  end

  it 'should pass long BLOCKQUOTE tokens through without any special meaning' do
    @parser.parse("<nowiki>\n> </nowiki>").should == "<p>\n&gt; </p>\n"
  end

  it 'should pass <tt> and </tt> tags through without any special meaning' do
    @parser.parse('<nowiki>foo <tt>bar</tt></nowiki>').should == "<p>foo &lt;tt&gt;bar&lt;/tt&gt;</p>\n"
  end

  it 'should pass <em> and </em> tags through without any special meaning' do
    @parser.parse("<nowiki>foo ''bar''</nowiki>").should == "<p>foo ''bar''</p>\n"
  end

  it 'should pass <strong> and </strong> tags through without any special meaning' do
    @parser.parse("<nowiki>foo '''bar'''</nowiki>").should == "<p>foo '''bar'''</p>\n"
  end

  it 'should pass combined <strong>/<em> and </strong>/</em> tags through without any special meaning' do
    @parser.parse("<nowiki>foo '''''bar'''''</nowiki>").should == "<p>foo '''''bar'''''</p>\n"
  end

  it 'should pass <h1> tags through without any special meaning' do
    @parser.parse("<nowiki>\n= foo</nowiki>").should == "<p>\n= foo</p>\n"
  end

  it 'should pass </h1> tags through without any special meaning' do
    @parser.parse("<nowiki>foo =\n</nowiki>").should == "<p>foo =\n</p>\n"
  end

  it 'should pass <h2> tags through without any special meaning' do
    @parser.parse("<nowiki>\n== foo</nowiki>").should == "<p>\n== foo</p>\n"
  end

  it 'should pass </h2> tags through without any special meaning' do
    @parser.parse("<nowiki>foo ==\n</nowiki>").should == "<p>foo ==\n</p>\n"
  end

  it 'should pass <h3> tags through without any special meaning' do
    @parser.parse("<nowiki>\n=== foo</nowiki>").should == "<p>\n=== foo</p>\n"
  end

  it 'should pass </h3> tags through without any special meaning' do
    @parser.parse("<nowiki>foo ===\n</nowiki>").should == "<p>foo ===\n</p>\n"
  end

  it 'should pass <h4> tags through without any special meaning' do
    @parser.parse("<nowiki>\n==== foo</nowiki>").should == "<p>\n==== foo</p>\n"
  end

  it 'should pass </h4> tags through without any special meaning' do
    @parser.parse("<nowiki>foo ====\n</nowiki>").should == "<p>foo ====\n</p>\n"
  end

  it 'should pass <h5> tags through without any special meaning' do
    @parser.parse("<nowiki>\n===== foo</nowiki>").should == "<p>\n===== foo</p>\n"
  end

  it 'should pass </h5> tags through without any special meaning' do
    @parser.parse("<nowiki>foo =====\n</nowiki>").should == "<p>foo =====\n</p>\n"
  end

  it 'should pass <h6> tags through without any special meaning' do
    @parser.parse("<nowiki>\n====== foo</nowiki>").should == "<p>\n====== foo</p>\n"
  end

  it 'should pass </h6> tags through without any special meaning' do
    @parser.parse("<nowiki>foo ======\n</nowiki>").should == "<p>foo ======\n</p>\n"
  end

  it 'should pass link start tokens through unchanged' do
    @parser.parse('<nowiki>[[</nowiki>').should == "<p>[[</p>\n"
  end

  it 'should pass link end tokens through unchanged' do
    @parser.parse('<nowiki>]]</nowiki>').should == "<p>]]</p>\n"
  end

  it 'should pass external link start tokens through unchanged' do
    @parser.parse('<nowiki>[</nowiki>').should == "<p>[</p>\n"
  end

  it 'should pass external link end tokens through unchanged' do
    @parser.parse('<nowiki>]</nowiki>').should == "<p>]</p>\n"
  end

  it 'should pass named entities through unchanged' do
    @parser.parse('<nowiki>&euro;</nowiki>').should == "<p>&euro;</p>\n"
  end

  it 'should pass numeric (decimal) entities through unchanged' do
    @parser.parse('<nowiki>&#8364;</nowiki>').should == "<p>&#8364;</p>\n"
  end

  it 'should pass numeric (hexadecimal) entities through unchanged' do
    @parser.parse('<nowiki>&#x20ac;</nowiki>').should == "<p>&#x20ac;</p>\n"
    @parser.parse('<nowiki>&#X20Ac;</nowiki>').should == "<p>&#x20ac;</p>\n"
  end

  it 'should convert non-ASCII characters to numeric entities' do
    @parser.parse('<nowiki>€</nowiki>').should == "<p>&#x20ac;</p>\n"
  end
end
